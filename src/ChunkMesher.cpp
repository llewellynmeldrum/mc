
#include <tuple>
#include <utility>

#include "ChunkConcurrency.hpp"
#include "ChunkNeighbourhood.hpp"
#include "CommonConcepts.hpp"
#include "CoordTypes.hpp"
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
#include "BlockAmbientOcclusion.hpp"
#include "Assertion.hpp"

#include "Vertex.hpp"

#include "ChunkMesher_RawData.hpp"
#include "ThreadTracker.hpp"
#include "ChunkMesher.hpp"


// Thats 2 bits ()
template<typename T>
    requires has_default_ctor<T>
std::array<T, Direction_Count> get_block_neighbours(
    ChunkNeighbourhoodSnapshot<T> const& neighbourhood,
    ChunkBlockPos cpos
) {
    // TODO: finish
    const auto& chunk_local_block = cpos;

    std::array<T, Direction_Count> res{};
    for (const auto& dir : each_direction){
        const i32   dir_idx = static_cast<i32>(dir);
        const auto neigh_offset = Direction_offset[dir_idx];
        ChunkBlockPos neighbour_block_pos = chunk_local_block + BlockOffset{neigh_offset};
        T neighbour_val = {};
        if (neighbourhood.in_neighbourhood(neighbour_block_pos)){
            neighbour_val = neighbourhood.get(neighbour_block_pos);
        }
        res[dir_idx] = neighbour_val; 
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
    BlockNeighbourhoodSnapshot const& blocks;
    LightNeighbourhoodSnapshot const& lights;
    const_span<Block, Direction_Count> adjacent_blocks;
    const_span<PackedLightValue, Direction_Count> adjacent_lights;
};




u8 get_ao_state(Block const& recipient, AONeighbours const& blockers){
    static constexpr u8 NO_AO {0b00}, ONLY_CORNER_AO{0b01}, ONE_SIDE_AO {0b10}, BOTH_SIDES_AO {0b11};

    if (!recipient.is_opaque()){
        // only opaque blocks should receive AO
        return NO_AO;
    }
    // TODO: In future, move AO into its own packed float, and make it so that other block shapes
    // can occlude, but perhaps smaller amounts. Slabs would occlude, but only the blocks which they are directly touching.
    // Cutouts could occlude, but perhaps somehow scaled based on their density or something.
    
    auto can_occlude = [](Block const& bt){
        return bt.is_opaque() && bt.shape() == BlockShape::CUBE;
    };
    if (can_occlude(blockers.a) && can_occlude(blockers.b)) return BOTH_SIDES_AO;
    if (can_occlude(blockers.a) || can_occlude(blockers.b)) return ONE_SIDE_AO;
    if (can_occlude(blockers.corner)) return ONLY_CORNER_AO;
    return NO_AO;
    // 00 (0) - no AO
    // 01 (1) - only corner
    // 10 (2) - one side
    // 11 (3) - both sides, corner doesnt matter
}
// The necessary info to mesh a quad
template<BlockShape block_shape>
void mesh_quad(BlockMeshContext& ctx, size_t facing_idx){
    const auto& block = ctx.block;
    const auto& chunk_local = ctx.chunk_local_block;
    const auto& incoming_light = ctx.adjacent_lights[facing_idx];
    auto quad_vertices = get_quad_data<block_shape>(facing_idx);
    const auto& tx_coords = ctx.atlas->quad_texture_uvs<block_shape>(block.texture_id(), facing_idx, quad_vertices);



    AONeighbours ao0 = get_vtx_ao_neighbours(ctx.blocks,chunk_local,static_cast<Direction>(facing_idx), 0);
    AONeighbours ao1 = get_vtx_ao_neighbours(ctx.blocks,chunk_local,static_cast<Direction>(facing_idx), 1);
    AONeighbours ao2 = get_vtx_ao_neighbours(ctx.blocks,chunk_local,static_cast<Direction>(facing_idx), 2);
    AONeighbours ao3 = get_vtx_ao_neighbours(ctx.blocks,chunk_local,static_cast<Direction>(facing_idx), 3);
    std::array<u8,4> ao_state= {
        get_ao_state(block, ao0),
        get_ao_state(block, ao1),
        get_ao_state(block, ao2),
        get_ao_state(block, ao3),
    };
    std::array<f32,4> ao_lut= {
        1.00f, // 00 (0) - no AO
        0.75f, // 01 (1) - only corner
        0.50f, // 10 (2) - one side
        0.25f // 11 (3) - both sides, corner doesnt matter
    };
    auto a0 = ao_lut[ao_state[0]];
    auto a1 = ao_lut[ao_state[1]];
    auto a2 = ao_lut[ao_state[2]];
    auto a3 = ao_lut[ao_state[3]];
    auto const* selected_indices = &quad_indices; // seam runs 1->3
    //if (a0 + a3  < a1 + a2){
    if (a0 + a2  < a1 + a3){
        selected_indices = &quad_indices;
        // use alternative triangle order to avoid anisotropy 
//        selected_indices = &flipped_quad_indices;   // seam runs 0->2
    }else{
        selected_indices = &flipped_quad_indices;
    }


    for (size_t i = 0; i < INDICES_PER_QUAD; i++) {
        i32 mapped_index = ctx.vtx_count + (*selected_indices).at(i);
        ctx.out_indices.push_back(mapped_index);
    }
    ctx.vtx_count += quad_vertices.size();
    for (const auto& [vtx_idx, vtx] : views::enumerate(quad_vertices)) {
        vtx.set_ao_state(ao_state[vtx_idx]);
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

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.adjacent_blocks)) {
        mesh_quad<BlockShape::CACTUS>(ctx,face_idx);
    }
}

template<typename MaterialType>
void mesh_cube(BlockMeshContext& ctx){
    const auto& block = ctx.block;

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.adjacent_blocks)) {
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

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.adjacent_blocks)) {
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

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.adjacent_blocks)) {
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

    for (const auto& [face_idx, adjacentBlock] : views::enumerate(ctx.adjacent_blocks)) {
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
auto mesh_type_matches(Block block){
    if constexpr(std::same_as<MeshDataType,OpaqueMeshData>)
        return block.is_opaque();
    else if constexpr(std::same_as<MeshDataType,BlendedMeshData>)
        return block.is_blended();
    else if constexpr(std::same_as<MeshDataType,CutoutMeshData>)
        return block.is_cutout();
    else 
        static_assert(false,"Unknown mesh type.");
}

template<typename MeshDataType>
MeshDataType mesh_chunk(const MeshJob& job){
    MeshDataType res{};
    auto& out_indices = res.indices;
    auto& out_vertices = res.vertices;

    // WARNING: These are pretty huge reserve()s. no idea if they will be worth it 
    // out_vertices.reserve(MAX_VERTICES_PER_CHUNK);
    // out_indices.reserve(MAX_INDICES_PER_CHUNK);

    const auto& blocks = job.blocks;
    const auto& lights = job.lights;
    const auto& atlas_map = job.atlas_map;

    u32 vtx_count = 0;
    for_each_xyz_in_chunk([&](i32 cx, i32 cy, i32 cz){
        auto chunk_local_block = ChunkBlockPos{cx,cy,cz};
        auto block = blocks.get(chunk_local_block);
        if (block.is_air() || !mesh_type_matches<MeshDataType>(block)){
            return;
        }

        auto block_shape = block.get_shape();
        const auto& atlas = atlas_map[block_shape_to_texture_atlas[block_shape]];
        auto adjacent_blocks = get_block_neighbours(blocks, chunk_local_block);
        auto adjacent_lights = get_block_neighbours(lights, chunk_local_block);

        auto ctx = BlockMeshContext{ 
            vtx_count,
            out_vertices,
            out_indices,
            block,
            chunk_local_block,
            atlas,
            blocks,
            lights,
            adjacent_blocks,
            adjacent_lights 
        };
        mesh_shape<MeshDataType>(block_shape,ctx);
    });
    return res;
}


MeshResult perform_mesh_work(MeshJob&& job){
    MeshResult res{
        .rev = job.rev,
        .coord = job.coord,
        .opaque = mesh_chunk<OpaqueMeshData>(job),
        .blended = mesh_chunk<BlendedMeshData>(job),
        .cutout = mesh_chunk<CutoutMeshData>(job),
    };
    return res;
}

