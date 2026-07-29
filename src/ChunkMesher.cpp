
#include <tuple>
#include <utility>

#include "CommonConcepts.hpp"
#include "DebugOptions.hpp"
#include "cpp23_ranges.hpp"

#include "World.hpp"

#include "Chunk.hpp"
#include "CoordIteration.hpp"
#include "Breakpoints.hpp"
#include "Direction.hpp"
#include "FormatSpecs.hpp"
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
#include "ThreadTracker.hpp"

template<BlockShape shape>
const auto& get_quad_data(i8 dir);

template<> const auto& get_quad_data<BlockShape::CROSS>(i8 idx) {
    return AT(cross_vtx::quads,idx);
}
template<> const auto& get_quad_data<BlockShape::CUBE>(i8 dir) {
    return AT(cube_vtx::quads,dir);
}
template<> const auto& get_quad_data<BlockShape::CACTUS>(i8 dir) {
    return AT(cactus_vtx::quads,dir);
}
template<> const auto& get_quad_data<BlockShape::BOT_HALF_SLAB>(i8 dir) {
    return AT(lower_half_slab_vtx::quads,dir);
}
template<> const auto& get_quad_data<BlockShape::TOP_HALF_SLAB>(i8 dir) {
    return AT(top_half_slab_vtx::quads,dir);
}
#define X(var)                                                              \
template<> const auto& get_quad_data<BlockShape::var>(i8 dir) {             \
    return AT(snow_vtx::quad_n[std::to_underlying(BlockShape::var)-1],dir); \
}
    SNOW_SHAPE_LIST
#undef X

template<typename T>
    requires has_default_ctor<T>
std::array<T, Direction_Count> get_block_neighbours(
    const GenericChunkStore<T>& center_chunk, 
    ChunkBlockPos cpos, 
    std::vector<GenericChunkSlice<T>> neighbour_chunk_slices
) {
    const auto& chunk_local_block = cpos;

    std::array<T, Direction_Count> res{};
    constexpr glm::ivec3 lo = glm::ivec3(0);
    constexpr glm::ivec3 hi = ChunkInfo::Extents3D;
    for (const auto& dir : each_horizontal_direction){
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool target_in_center_chunk = LM::isVecInBounds(neighbour_block_pos, lo, hi);
        
        if (target_in_center_chunk){
            AT(res,dir_idx) = AT(center_chunk, neighbour_block_pos);
        } else {
            if (neighbour_chunk_slices[dir_idx].is_empty){ 
                continue; // if other chunk is not stored, value is defaulted anyway
            }
            // The adjacent chunk has our neighbour. 
            const auto& other_chunk = neighbour_chunk_slices[dir_idx];
            neighbour_block_pos = LM::euclid_mod(neighbour_block_pos, ChunkInfo::Extents3D);
            AT(res,dir_idx) = AT(other_chunk,neighbour_block_pos);
        }
    }
    for (const auto& dir : each_vertical_direction){
        // if neighbour is within 0 - 256, then return that block. It exists in our chunk. It cannot be in another 
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        const bool neighbour_outside_world = neighbour_block_pos.y>=WORLD_YMAX || neighbour_block_pos.y < WORLD_YMIN;
        if (neighbour_outside_world) {
            AT(res,dir_idx) = T{};
            continue;
        }else{
            AT(res,dir_idx) = AT(center_chunk,neighbour_block_pos);
        }
    }

    return res;
}


struct BlockMeshContext{
    u32& vtx_count;
    std::vector<Vertex>& out_vertices;
    std::vector<u32>& out_indices;
    const Block& block;
    const ChunkBlockPos& chunk_local_block;
    const TextureAtlas* atlas;
    const ChunkBlockStore& blocks;
    const ChunkLightStore& lights;
    const_span<Block, Direction_Count> surrounding_blocks;
    const_span<PackedLightValue, Direction_Count> surrounding_lights;
};




// The necessary info to mesh a quad
template<BlockShape block_shape>
void mesh_quad(BlockMeshContext& ctx, size_t facing_idx){
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local = ctx.chunk_local_block;
    const auto& incoming_light = ctx.surrounding_lights[facing_idx];

    constexpr auto atlas_id = shape_atlas_id<block_shape>;
    auto quad_vertices = get_quad_data<block_shape>(facing_idx);
    const auto& tx_coords = ctx.atlas->quad_texture_uvs<block_shape>(block.texture_id(), facing_idx, quad_vertices);

    for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
        i32 mapped_index = ctx.vtx_count + quad_indices[i];
        ctx.out_indices.push_back(mapped_index);
    }

    ctx.vtx_count += quad_vertices.size();

    for (const auto& [vtx_idx, vtx] : views::enumerate(quad_vertices)) {
        vtx.copy_light_data(incoming_light);
        vtx.offset_by_chunk_pos(chunk_local);
        vtx.tx_coords = tx_coords[vtx_idx];
        vtx.set_face_opacity(block.get_opacity());
        if constexpr (block_shape == BlockShape::CROSS){
            vtx.set_face_dir(5);
        }else {
            vtx.set_face_dir(facing_idx);
        }
        ctx.out_vertices.push_back(vtx);
    }

}

template<typename MaterialType>
void mesh_cactus(BlockMeshContext& ctx){

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.surrounding_blocks)) {
        mesh_quad<BlockShape::CACTUS>(ctx,face_idx);
    }
}

template<typename MaterialType>
void mesh_cube(BlockMeshContext& ctx){
    const auto& block = ctx.block;
    const auto& chunk_local_block = ctx.chunk_local_block;

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.surrounding_blocks)) {
        const auto faceDir = static_cast<Direction>(face_idx);
        if constexpr (same_as_nocvref<MaterialType,OpaqueMeshData>){
            // for opaque blocks, skip quads which face opaque neighbours
            using BlockShape::CUBE;
            using BlockShape::BOT_HALF_SLAB;
            using Direction::UP;
            if (adjacentBlock.is_opaque()){
                bool adjacent_block_is_occluded = 
                    adjacentBlock.shape() == CUBE 
                    || (adjacentBlock.shape() == BOT_HALF_SLAB &&  faceDir == UP);

                if(adjacent_block_is_occluded) {
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

        mesh_quad<BlockShape::CUBE>(ctx,face_idx);
    }
}
template<typename MaterialType, std::size_t N>
void mesh_snow(BlockMeshContext& ctx){
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.surrounding_blocks)) {
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
        constexpr auto shape = shape_of_snow_level<N>;
        mesh_quad<shape>(ctx,face_idx);
    }
}

template<typename MaterialType>
void mesh_top_half_slab(BlockMeshContext& ctx){
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.surrounding_blocks)) {
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

        mesh_quad<BlockShape::TOP_HALF_SLAB>(ctx,face_idx);
    }
}

template<typename MaterialType>
void mesh_lower_half_slab(BlockMeshContext& ctx){
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.surrounding_blocks)) {
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


        mesh_quad<BlockShape::BOT_HALF_SLAB>(ctx,face_idx);
    }
}

template<typename MaterialType>
void mesh_cross(BlockMeshContext ctx){

    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;


    const auto directions = {Direction::FORWARD, Direction::BACKWARD};

    for (i8 quad_idx = 0; quad_idx < QUADS_PER_CROSS; quad_idx++){
        mesh_quad<BlockShape::CROSS>(ctx,quad_idx);
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
auto mesh_type_predicate(const ChunkBlockStore& chunk){
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

template<typename MeshDataType>
MeshDataType mesh_chunk(const MeshJob& job){
    MeshDataType mesh_data{};
    auto& out_indices = mesh_data.indices;
    auto& out_vertices = mesh_data.vertices;

    // WARNING: These are pretty huge reserve()s. no idea if they will be worth it 
    // out_vertices.reserve(MAX_VERTICES_PER_CHUNK);
    // out_indices.reserve(MAX_INDICES_PER_CHUNK);

    const WorldBlockPos world_pos = toWorldOrigin(job.chunkCoord);
    const auto& blocks = job.blocks;
    const auto& lights = job.light_data;
    const auto& atlas_map = job.atlas_map;
    const auto& neighbour_chunks_block_slices = job.surrounding_chunks_block_slices;
    const auto& neighbour_chunks_light_slices = job.surrounding_chunks_light_slices;

    u32 vtx_count = 0;
    for (const auto& [x, y, z] : EachBlockInChunk(mesh_type_predicate<MeshDataType>(blocks))) {
        const ChunkBlockPos chunk_local_block = { x, y, z };
        Block block = blocks[x, y, z];
        if (block.is_air()) {
            continue;
        }

        auto block_shape = block.get_shape();
        const auto& atlas = atlas_map[block_shape_to_texture_atlas[block_shape]];
        auto surrounding_blocks = get_block_neighbours(blocks, chunk_local_block, neighbour_chunks_block_slices);
        auto surrounding_lights = get_block_neighbours(lights, chunk_local_block, neighbour_chunks_light_slices);

        if (job.chunkCoord == WorldChunkCoord{0,0}){
            for (const auto& plv: lights){
            }
        }
        auto ctx = BlockMeshContext{ 
            vtx_count,
            out_vertices,
            out_indices,
            block,
            chunk_local_block,
            atlas,
            blocks,
            lights,
            surrounding_blocks,
            surrounding_lights 
        };
        mesh_shape<MeshDataType>(block_shape,ctx);
    }
    return {out_vertices,out_indices};
}


void mesh_chunks (std::stop_token stopToken, Queue<MeshJob>& in_queue, Queue<MeshResult>& out_queue){
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

