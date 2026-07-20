#include "WorldGen_BiomeFeatures.hpp"
#include "FormatSpecs.hpp"
#include "WorldGen_BiomeClassification.hpp"
#include "WorldGen_FeatureClassifcation.hpp"
#include "WorldGen_Utils.hpp"
// NOTE: SHARED

template<typename T>
bool should_place_shared(const FeatureShared& shared, WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks){
    if (!shared.is_enabled){
        return false;
    }
    auto cpos = toChunkBlockPos(origin) - BlockOffset{0,1,0};
    auto surface_block = blocks.at(cpos);
    if (!ranges::contains(shared.target_surfaces, surface_block)){
        return false;
    }
    if (blocks.at(cpos + ChunkBlockPos{0,1,0}) == BlockType::WATER_BLOCK){
        if (!feature_traits<T>::allow_underwater){
            return false;
        }
    }else{
        if (!feature_traits<T>::allow_outside_water){
            return false;
        }
    }

    if (sampled_density < shared.min_density()){
        return false;
    }
    return true;
}

// NOTE: SHARED
bool TreeFeature::should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const {
    if (!should_place_shared<value_type>(shared,origin,sampled_density, blocks)){
        return false;
    }
    return jitter.should_accept(origin, sampled_density, shared.density_spawn_remap);
}

bool SingleBlockFeature::should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const {
    if (!should_place_shared<value_type>(shared,origin,sampled_density, blocks)){
        return false;
    }
    return jitter.should_accept(origin, sampled_density, shared.density_spawn_remap);
}
bool MultiSegmentFeature::should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const {
    if (!should_place_shared<value_type>(shared,origin,sampled_density, blocks)){
        return false;
    }
    return jitter.should_accept(origin, sampled_density, shared.density_spawn_remap);
}
bool GrassFeature::should_place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const {
    if (!should_place_shared<value_type>(shared,origin,sampled_density, blocks)){
        return false;
    }
    return jitter.should_accept(origin, sampled_density, shared.density_spawn_remap);
}

void SingleBlockFeature::place(WorldBlockPos origin, f32 density, BlockWriter& blocks) const noexcept{
    blocks.try_place(
        OverwritePolicy::OnlyAir,
        origin,
        block
    );
}
void MultiSegmentFeature::place(WorldBlockPos origin, f32 density, BlockWriter& blocks) const noexcept{
    auto stem_height = stem_height_rng.roll(origin);
    blocks.try_place(
        OverwritePolicy::OnlyAir,
        origin,
        root
    );
    for (i32 dy = 0; dy<stem_height; dy++){
        blocks.try_place(
            OverwritePolicy::OnlyAir,
            origin + BlockOffset{0,dy,0},
            stem
        );

    }
    blocks.try_place(
        OverwritePolicy::OnlyAir,
        origin + BlockOffset{0,stem_height,0},
        head
    );
}

void GrassFeature::place(WorldBlockPos origin, f32 density, BlockWriter& blocks) const noexcept{
    auto block = growth_stages[0];
    f32 density_score = LM::unlerp(shared.min_density(),shared.max_density(),density);
    i32 stage = static_cast<i32>(density_score * growth_stages.size());
    if (wpos_seeded_rand01(origin, RandOffset::GrassPromotion) <= promotion_chance){
        stage = std::min(stage+1,(i32)growth_stages.size()-1);
        assert_geq(stage,0);
        assert_lt(stage,growth_stages.size());
    }
    if (wpos_seeded_rand01(origin, RandOffset::GrassDemotion) <= demotion_chance){
        if (stage<=0) return; // demoted to stage -1, i.e reject 
        stage--;
        assert_geq(stage,0);
        assert_lt(stage,growth_stages.size());
    }
    assert_geq(stage,0);
    assert_lt(stage,growth_stages.size());
    blocks.try_place(
        OverwritePolicy::OnlyAir,
        origin,
        growth_stages.at(stage)
    );
}

void TreeFeature::place(WorldBlockPos origin, [[maybe_unused]]f32 density, BlockWriter& blocks)const noexcept{
//    std::println("PLACING TREE {}", origin);
    // origin should be the location of the bottom log, i.e grass + {0,1,0}
    auto source_chunk = toWorldChunkCoord(origin);
    const i32 log_height = log_height_rng.roll(origin);
    const i32 bot_log_y = origin.y;
    const i32 top_log_y = origin.y + log_height-1;

    //NOTE: LOGS
    for (i32 dy = 0; dy<log_height; dy++){
        blocks.try_place(
            OverwritePolicy::OnlyAir,
            {
                origin.x, 
                origin.y + dy,
                origin.z
            },
            log_block
        );
    }
    //NOTE: CANOPY
    i32 canopy_y0 = top_log_y + top_canopy_y_offset;
    for (const auto& [idx, canopy]: views::enumerate(canopies)){
        if (idx >= canopies.size()) break;
        for (auto dy = 0 ; dy < canopy.height; dy++){
            // place canopy layer
            for (auto dx = -canopy.hoz_extent ; dx<=canopy.hoz_extent; dx++){
                for (auto dz = -canopy.hoz_extent ; dz<=canopy.hoz_extent; dz++){
                    auto pos = WorldBlockPos{
                        origin.x + dx, 
                        canopy_y0 - dy,
                        origin.z + dz,
                    };
                    if (canopy.is_corner(dx,dz)){
                        bool leaf_decayed = canopy.is_decayed(pos);
                        if (leaf_decayed) {
                            continue;
                        }
                    }
                    blocks.try_place(
                        OverwritePolicy::OnlyAir,
                        {
                            origin.x + dx, 
                            canopy_y0 - dy,
                            origin.z + dz,
                        },
                        leaf_block
                    );
                }
            }
        }
        canopy_y0 -= canopy.height; // next canopy starts at prev-height
    }
    if (spruce_crown){
        auto crown_y = top_log_y + 1;
        blocks.try_place(
            OverwritePolicy::OnlyAir,
            {
                origin.x,
                crown_y,
                origin.z,
            },
            leaf_block
        );

    }
}
