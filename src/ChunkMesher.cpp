
#include <tuple>
#include <utility>
#include "DebugOptions.hpp"
#include "cpp23_ranges.hpp"


#include "Chunk.hpp"
#include "CoordIteration.hpp"
#include "Breakpoints.hpp"
#include "Direction.hpp"
#include "FormatSpecs.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkConstants.hpp"

// include/Chunk
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Engine.hpp"
#include "LM.hpp"
#include "Concurrency.hpp"

#include "Logger.hpp"
#include "Assertion.hpp"

#include "Vertex.hpp"

#include "ChunkMesher_RawData.hpp"


#include "ChunkNoiseDebug.hpp"
#include "ThreadTracker.hpp"

struct BlockMeshContext{
    u32& vtx_count;
    std::vector<Vertex>& out_vertices;
    std::vector<u32>& out_indices;
    const Block& block;
    const ChunkBlockPos& chunk_local_block;
    const TextureAtlas* atlas;
    const ChunkStore& blocks;
    const_span<std::optional<ChunkSlice2D>> surrounding_chunks;
#ifdef CHUNK_NOISE_DEBUG
    PerColumnDebugStore<NoiseParams> noise{};
#endif
};

std::array<Block, Direction_Count> get_surrounding_blocks(const BlockMeshContext& ctx);


const auto& get_cross_quad_data(i8 idx) {
    return AT(cross_vtx::quads,idx);
}
const auto& get_cube_quad_data(Direction dir) {
    return AT(cube_vtx::quads,std::to_underlying(dir));
}
const auto& get_cactus_quad_data(Direction dir) {
    return AT(cactus_vtx::quads,std::to_underlying(dir));
}
const auto& get_bot_half_slab_quad_data(Direction dir) {
    return AT(lower_half_slab_vtx::quads,std::to_underlying(dir));
}
const auto& get_top_half_slab_quad_data(Direction dir) {
    return AT(top_half_slab_vtx::quads,std::to_underlying(dir));
}
template<std::size_t N>
const auto& snow_quad_data(Direction dir) {
    return AT(snow_vtx::quad_n[N-1],std::to_underlying(dir));
}

template<BlockShape block_shape>
void mesh_quad(BlockMeshContext& ctx, f32 quad_opacity, QuadVertexList quad_vertices, ChunkBlockPos chunk_local, QuadUVList uvs, i32 face_idx=0){
    using vec3 = glm::vec3;
    constexpr auto atlas_id = shape_atlas_id<block_shape>;
    for (const auto& [vtx_idx, vtx] : views::enumerate(quad_vertices)) {
        vtx.offset_by_chunk_pos(chunk_local);
        vtx.txCoords = uvs[vtx_idx];
        vtx.set_face_opacity(quad_opacity);
        if constexpr (block_shape != BlockShape::CROSS){
            vtx.set_face_dir(face_idx);
        }else {
            vtx.set_face_dir(5);
        }
        ctx.out_vertices.push_back(vtx);
    }
}

template<typename MeshDataType>
auto mesh_type_predicate(ChunkStore& chunk){
    if constexpr(std::same_as<MeshDataType,OpaqueMeshData>){
        return [&chunk](auto xyz){
            auto [x,y,z]=xyz;
            return chunk.at(x,y,z).is_opaque();
        };
    }else if constexpr(std::same_as<MeshDataType,BlendedMeshData>){
        return[&chunk](auto xyz){
            auto [x,y,z]=xyz;
            return chunk.at(x,y,z).is_blended();
        };
    }else if constexpr(std::same_as<MeshDataType,CutoutMeshData>){
        return[&chunk](auto xyz){
            auto [x,y,z]=xyz;
            return chunk.at(x,y,z).is_cutout();
        };
    }else {
        static_assert(false,"Unknown mesh type.");
    }
}

template<typename MaterialType>
void mesh_cactus(BlockMeshContext& ctx){
    const f32 block_opacity = ctx.block.get_opacity();
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    auto surrounding = get_surrounding_blocks(ctx);
    for (const auto& [face_idx, adjacentBlock] : views::enumerate(surrounding)) {
        const auto faceDir = static_cast<Direction>(face_idx);


        const auto& quad_vertices = get_cactus_quad_data(faceDir);
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cube(block.texture_id(), faceDir, quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        mesh_quad<BlockShape::CACTUS>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
    }
}
template<typename MaterialType>
void mesh_cube(BlockMeshContext& ctx){
    const f32 block_opacity = ctx.block.get_opacity();
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    auto surrounding = get_surrounding_blocks(ctx);
    for (const auto& [face_idx, adjacentBlock] : views::enumerate(surrounding)) {
        const auto faceDir = static_cast<Direction>(face_idx);
        if constexpr (same_as_nocvref<MaterialType,OpaqueMeshData>){
            // for opaque blocks, skip quads which face opaque neighbours
            if (adjacentBlock.is_opaque()){
                if(adjacentBlock.shape() == BlockShape::CUBE || 
                   (adjacentBlock.shape() == BlockShape::BOT_HALF_SLAB &&  faceDir == Direction::UP))
                {
                continue;
                }
            }
        }else if constexpr(same_as_nocvref<MaterialType,BlendedMeshData>){
            // for blended blocks, skip quads which face same-block neighbours
            if (adjacentBlock.type==block.type){
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,CutoutMeshData>){
            // skip ZERO quads for cutout. All sides always (i think?)
        }else {
            static_assert(false, "Unrecognized MeshDataType");
        }


        const auto& quad_vertices = get_cube_quad_data(faceDir);
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cube(block.texture_id(), faceDir, quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        mesh_quad<BlockShape::CUBE>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
    }
}
template<typename MaterialType, std::size_t N>
void mesh_snow(BlockMeshContext& ctx){
    const f32 block_opacity = ctx.block.get_opacity();
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    auto surrounding = get_surrounding_blocks(ctx);
    for (const auto& [face_idx, adjacentBlock] : views::enumerate(surrounding)) {
        const auto faceDir = static_cast<Direction>(face_idx);
        // Snow shape has the same rules as bot half slabs
        if constexpr (same_as_nocvref<MaterialType,OpaqueMeshData>){
            if (faceDir == Direction::DOWN && adjacentBlock.is_opaque()) {
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,BlendedMeshData>){
            if (faceDir == Direction::DOWN && adjacentBlock.type==block.type){
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,CutoutMeshData>){
            // skip ZERO quads for cutout. All sides always (i think?)
            // On second thought, i think it might be the same rules as opaque? 
            // Or would that make leaves and such look too sparse?
        }else {
            static_assert(false, "Unrecognized MeshDataType");
        }


        const auto& quad_vertices = snow_quad_data<N>(faceDir);
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cube(block.texture_id(), faceDir, quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count += quad_vertices.size();
        constexpr auto shape = shape_of_snow_level<N>;
        mesh_quad<shape>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
    }
}

template<typename MaterialType>
void mesh_top_half_slab(BlockMeshContext& ctx){
    const f32 block_opacity = ctx.block.get_opacity();
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    auto surrounding = get_surrounding_blocks(ctx);
    for (const auto& [face_idx, adjacentBlock] : views::enumerate(surrounding)) {
        const auto faceDir = static_cast<Direction>(face_idx);
        // Opaque top half slabs are only guaranteed to cover the block above them.
        if constexpr (same_as_nocvref<MaterialType,OpaqueMeshData>){
            if (faceDir == Direction::UP && adjacentBlock.is_opaque()) {
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,BlendedMeshData>){
            if (faceDir == Direction::UP && adjacentBlock.type==block.type){
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,CutoutMeshData>){
            // skip ZERO quads for cutout. All sides always (i think?)
            // On second thought, i think it might be the same rules as opaque? 
            // Or would that make leaves and such look too sparse?
        }else {
            static_assert(false, "Unrecognized MeshDataType");
        }


        const auto& quad_vertices = get_top_half_slab_quad_data(faceDir);
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cube(block.texture_id(), faceDir, quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count += quad_vertices.size();
        mesh_quad<BlockShape::TOP_HALF_SLAB>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
    }
}

template<typename MaterialType>
void mesh_lower_half_slab(BlockMeshContext& ctx){
    const f32 block_opacity = ctx.block.get_opacity();
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    auto surrounding = get_surrounding_blocks(ctx);
    for (const auto& [face_idx, adjacentBlock] : views::enumerate(surrounding)) {
        const auto faceDir = static_cast<Direction>(face_idx);
        if constexpr (same_as_nocvref<MaterialType,OpaqueMeshData>){
            // Opaque half slabs are only guaranteed to cover the block below them.
            if (faceDir == Direction::DOWN && adjacentBlock.is_opaque()) {
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,BlendedMeshData>){
            // for blended blocks, skip quads which face same-block neighbours
            if (faceDir == Direction::DOWN && adjacentBlock.type==block.type){
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,CutoutMeshData>){
            // skip ZERO quads for cutout. All sides always (i think?)
        }else {
            static_assert(false, "Unrecognized MeshDataType");
        }


        const auto& quad_vertices = get_bot_half_slab_quad_data(faceDir);
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cube(block.texture_id(), faceDir, quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        mesh_quad<BlockShape::BOT_HALF_SLAB>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
    }
}

template<typename MaterialType>
void mesh_cross(BlockMeshContext ctx){
    const f32 block_opacity = ctx.block.get_opacity();

    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;


    const auto directions = {Direction::FORWARD, Direction::BACKWARD};

    for (i8 quad_idx = 0; quad_idx < QUADS_PER_CROSS; quad_idx++){
        const auto& quad_vertices = get_cross_quad_data(quad_idx);
        const auto& quad_tex_coords = atlas->get_texture_uvs_cross(block.texture_id(), quad_vertices);

        for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        mesh_quad<BlockShape::CROSS>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords);
    }
}

template<typename MeshDataType>
void mesh_shape(BlockShape shape, BlockMeshContext& ctx){
    switch (ctx.block.get_shape()){
        case BlockShape::CUBE:
            mesh_cube<MeshDataType>(ctx);
            break;
        case BlockShape::CROSS:
            mesh_cross<MeshDataType>(ctx);
            break;
        case BlockShape::CACTUS:
            mesh_cactus<MeshDataType>(ctx);
            break;
        case BlockShape::BOT_HALF_SLAB:
            mesh_lower_half_slab<MeshDataType>(ctx);
            break;
        case BlockShape::TOP_HALF_SLAB:
            mesh_top_half_slab<MeshDataType>(ctx);
            break;
    	case BlockShape::SNOW_1_15:  mesh_snow<MeshDataType, 1>(ctx); break;
    	case BlockShape::SNOW_2_15:  mesh_snow<MeshDataType, 2>(ctx); break;
    	case BlockShape::SNOW_3_15:  mesh_snow<MeshDataType, 3>(ctx); break;
    	case BlockShape::SNOW_4_15:  mesh_snow<MeshDataType, 4>(ctx); break;
    	case BlockShape::SNOW_5_15:  mesh_snow<MeshDataType, 5>(ctx); break;
    	case BlockShape::SNOW_6_15:  mesh_snow<MeshDataType, 6>(ctx); break;
    	case BlockShape::SNOW_7_15:  mesh_snow<MeshDataType, 7>(ctx); break;
    	case BlockShape::SNOW_8_15:  mesh_snow<MeshDataType, 8>(ctx); break;
    	case BlockShape::SNOW_9_15:  mesh_snow<MeshDataType, 9>(ctx); break;
    	case BlockShape::SNOW_10_15: mesh_snow<MeshDataType,10>(ctx); break;
    	case BlockShape::SNOW_11_15: mesh_snow<MeshDataType,11>(ctx); break;
    	case BlockShape::SNOW_12_15: mesh_snow<MeshDataType,12>(ctx); break;
    	case BlockShape::SNOW_13_15: mesh_snow<MeshDataType,13>(ctx); break;
    	case BlockShape::SNOW_14_15: mesh_snow<MeshDataType,14>(ctx); break;
    	case BlockShape::SNOW_15_15: mesh_snow<MeshDataType,15>(ctx); break;
        case BlockShape::COUNT:
            BREAKPOINT(); // tf are you doing if you get here
            break;
    }
}

template<typename MeshDataType>
MeshDataType mesh_chunk(const MeshJob& job){
    MeshDataType mesh_data{};
    auto& out_indices = mesh_data.indices;
    auto& out_vertices = mesh_data.vertices;

    // WARNING: These are pretty huge reserve()s. no idea if they will be worth it 
    out_vertices.reserve(MAX_VERTICES_PER_CHUNK);
    out_indices.reserve(MAX_INDICES_PER_CHUNK);

    const WorldBlockPos world_pos = toWorldOrigin(job.chunkCoord);
    auto blocks = job.blocks;
    const auto atlas_map = job.atlas_map;
    const auto& surrounding_chunks = job.surroundingChunks;

    u32 vtx_count = 0;
    for (const auto& [x, y, z] : EachBlockInChunk(mesh_type_predicate<MeshDataType>(blocks))) {
        const ChunkBlockPos chunk_local_block = { x, y, z };
        Block block = blocks[x, y, z];
        if (block.is_air()) {
            continue;
        }

        auto block_shape = block.get_shape();
        const auto& atlas = atlas_map[block_shape_to_texture_atlas[block_shape]];
        auto ctx = BlockMeshContext{
            vtx_count,out_vertices,out_indices,block,chunk_local_block,atlas,blocks,surrounding_chunks
#ifdef CHUNK_NOISE_DEBUG
            ,job.noise
#endif
        };
        mesh_shape<MeshDataType>(block_shape,ctx);
    }
    return {out_vertices,out_indices};
}


void ChunkMesher::mesh_chunks (std::stop_token stopToken, Queue<MeshJob>& in_queue, Queue<MeshResult>& out_queue){
    ThreadTracker::assign_my_thread_type(ThreadType::mesh);
    while (!stopToken.stop_requested()){
        
        auto job = in_queue.wait_dequeue();
        job.bench.job_idle.bench_end(job.chunkCoord,job.meshRevisionID);

        job.bench.work.bench_start(job.chunkCoord,job.meshRevisionID);
            MeshResult res{job.meshRevisionID, job.chunkCoord};
            res.blended = mesh_chunk<BlendedMeshData>(job); // mandatory copy elision on job i think
            res.opaque = mesh_chunk<OpaqueMeshData>(job); // mandatory copy elision on job i think
            res.cutout = mesh_chunk<CutoutMeshData>(job); // mandatory copy elision on job i think
        job.bench.work.bench_end(job.chunkCoord,job.meshRevisionID);

        job.bench.res_idle.bench_start(job.chunkCoord,job.meshRevisionID);
        out_queue.wait_emplace(res);
    }

}

std::array<Block, Direction_Count> get_surrounding_blocks(const BlockMeshContext& ctx) {
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    std::array<Block, Direction_Count> res{};
    constexpr glm::ivec3 lo = glm::ivec3(0);
    constexpr glm::ivec3 hi = ChunkInfo::Extents3D;
    for (const auto& dir : each_horizontal_direction){
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool neighbour_inside_my_chunk = LM::isVecInBounds(neighbour_block_pos, lo, hi);
        
        if (neighbour_inside_my_chunk) [[likely]]{
            AT(res,dir_idx) = AT(ctx.blocks,neighbour_block_pos);
        } else [[unlikely]] {
            // The adjacent chunk, if it exists, has our neighbour. else its air
            bool other_chunk_exists = ctx.surrounding_chunks[dir_idx].has_value();
            if (other_chunk_exists){
                const auto& other_chunk = ctx.surrounding_chunks[dir_idx].value();
                neighbour_block_pos = LM::euclid_mod(neighbour_block_pos, ChunkInfo::Extents3D);
                AT(res,dir_idx) = AT(other_chunk,neighbour_block_pos);
            }else{
                // treat all blocks of a missing chunk as air
                AT(res,dir_idx) = Block(BlockType::AIR);
            }
        }
    }
    for (const auto& dir : each_vertical_direction){
        // if neighbour is within 0 - 256, then return that block. It exists in our chunk. It cannot be in another 
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool neighbour_outside_world = neighbour_block_pos.y>=WORLD_YMAX || neighbour_block_pos.y < WORLD_YMIN;
        if (neighbour_outside_world) {
            AT(res,dir_idx) = Block(BlockType::AIR);
            continue;
        }else{
            AT(res,dir_idx) = AT(ctx.blocks,neighbour_block_pos);
        }
    }

    return res;
}
