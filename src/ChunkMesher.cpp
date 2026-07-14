
#include <tuple>
#include <utility>
#include "DebugOptions.hpp"
#include "cpp23_ranges.hpp"

#include "tracy/Tracy.hpp"

#include "Chunk.hpp"
#include "CoordIteration.hpp"
#include "Breakpoints.hpp"
#include "Direction.hpp"
#include "FormatSpecs.hpp"
#include "ChunkMesher.hpp"
#include "Block.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkInvariants.hpp"

// include/Chunk
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include "glmWrapper.hpp"
#include "Engine.hpp"
#include "Profiler.hpp"
#include "LM.hpp"
#include "Concurrency.hpp"

#include "Logger.hpp"
#include "Assertion.hpp"

#include "Vertex.hpp"

#include "ChunkMesher_RawData.hpp"


#include "ChunkNoiseDebug.hpp"

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

std::array<Block, DirectionCount> get_surrounding_blocks(const BlockMeshContext& ctx);


const auto& get_cross_quad_data(i8 idx) {
    return AT(cross_vtx::quads,idx);
}
const auto& get_cube_quad_data(Direction dir) {
    return AT(cube_vtx::quads,std::to_underlying(dir));
}


template<BlockShape block_shape>
void make_quad_vertices(BlockMeshContext& ctx, f32 quad_opacity, QuadVertexData quad_vertices, ChunkBlockPos chunk_local, QuadUVList uvs, i32 face_idx=0){
    using vec3 = glm::vec3;
    for (const auto& [idx, vtx] : views::enumerate(quad_vertices)) {
        // Vertex
        vec3 chunk_offsetted_vtx_pos =
            static_cast<vec3>(vtx.pos)
            +
            static_cast<vec3>(chunk_local.raw());
        if constexpr(block_shape == BlockShape::CROSS){
            ctx.out_vertices.emplace_back(
                chunk_offsetted_vtx_pos,
                uvs[idx], 
                glm::vec4{0.0f}, 
                static_cast<i32>(5), //HACK: until we make the separate shader, this gives us zero shadow. See vs.glsl
                quad_opacity,
                std::to_underlying(block_shape)
            );
        }else if constexpr(block_shape == BlockShape::CUBE){
            #ifdef CHUNK_NOISE_DEBUG
                glm::vec4 dbg_noise_overlay{0.0f};
                if (DebugOption::show_noise_debug){
                    constexpr static glm::vec4 CYAN = {40, 55, 225,255};
                    constexpr static glm::vec4 RED = {255, 55, 55,255};
                    auto red = (ctx.noise[chunk_local.x,chunk_local.z].heat) * RED;
                    auto cyan = (ctx.noise[chunk_local.x,chunk_local.z].rain) * CYAN;
                    dbg_noise_overlay = (red + cyan) / 2.0f;
                }
            #endif
            // 0.5-1.0 = 0.0 red to 1.0 red
            // 0.0 = 0.0 red
            // 0.5 = 0.0 red
            // 1.0 = 1.0 red
            ctx.out_vertices.emplace_back(
                chunk_offsetted_vtx_pos,
                uvs[idx], 
                #ifdef CHUNK_NOISE_DEBUG
                    dbg_noise_overlay,
                #else 
                {0.0f,0.0f,0.0f,0.0f},
                #endif
                static_cast<i32>(face_idx),
                quad_opacity,
                std::to_underlying(block_shape)
            );

        }else{
            ctx.out_vertices.emplace_back(
                chunk_offsetted_vtx_pos,
                uvs[idx], 
                glm::vec4{0.0f}, 
                static_cast<i32>(idx),
                quad_opacity,
                std::to_underlying(block_shape)
            );
        }
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
            if (adjacentBlock.is_opaque()) {
                continue;
            }
        }else if constexpr(same_as_nocvref<MaterialType,BlendedMeshData>){
            // for transparent blocks, skip quads which face same-block neighbours
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

        for (std::size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        make_quad_vertices<BlockShape::CUBE>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords,face_idx);
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
        const auto& quad_tex_coords = atlas->apply_texture_uvs_cross(block.texture_id(), quad_vertices);

        for (std::size_t i = 0; i < INDICES_PER_QUAD; i++) {
            i32 mapped_index = ctx.vtx_count + quad_indices[i];
            ctx.out_indices.push_back(mapped_index);
        }

        ctx.vtx_count+= quad_vertices.size();
        make_quad_vertices<BlockShape::CROSS>(ctx, block_opacity, quad_vertices,chunk_local_block, quad_tex_coords);
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
        const auto& atlas = atlas_map[std::to_underlying(block_shape)];
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
    tracy::SetThreadName("chunk mesher");
    while (!stopToken.stop_requested()){
        
        auto job = in_queue.wait_dequeue();


        MeshResult res{job.meshRevisionID, job.chunkCoord};
        { 
            ZoneScopedN("transparent_mesh_chunks")
            res.blended = mesh_chunk<BlendedMeshData>(job); // mandatory copy elision on job i think
        }
        { 
            ZoneScopedN("opaque_mesh_chunks")
            res.opaque = mesh_chunk<OpaqueMeshData>(job); // mandatory copy elision on job i think
        }
        { 
            ZoneScopedN("opaque_mesh_chunks")
            res.cutout = mesh_chunk<CutoutMeshData>(job); // mandatory copy elision on job i think
        }

        { 
            ZoneScopedN("mesh_await_output")
            out_queue.wait_emplace(res);
        }
    }

}

std::array<Block, DirectionCount> get_surrounding_blocks(const BlockMeshContext& ctx) {
    const auto& block = ctx.block;
    const auto& atlas = ctx.atlas;
    const auto& chunk_local_block = ctx.chunk_local_block;

    std::array<Block, DirectionCount> res{};
    constexpr glm::ivec3 lo = glm::ivec3(0);
    constexpr glm::ivec3 hi = Chunk::Extents;
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
                neighbour_block_pos = LM::euclid_mod(neighbour_block_pos, Chunk::Extents);
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
