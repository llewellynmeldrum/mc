#include "WorldGen_BiomeFeatures.hpp"
void TreeFeature::place(WorldBlockPos origin, f32 sampled_density, BlockWriter& blocks)const noexcept{
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
        if (idx >= num_canopies) break;
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
}
