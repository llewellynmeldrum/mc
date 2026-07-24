# new features
for each column 
check biomes feature list 
first (deterministic) should_place() success = hand over WriteCtx, make the feature


```cpp





struct BiomeFeatureSet{
    TreeFeature tree;
    GrassFeature grass;
};

constexpr BiomeFeatureSet PlainsFeatureSet{
.tree {
  .log_height = {5,8},
  .rng_offset = 12074,
  .log= BlockType::OAK_LOG,
  .leaf = BlockType::OAK_LEAF,
},

};

ForEachInRangeEx(lo,hi,[&](i32 cx, i32 cz){
    auto surface_height = heightmap[cx,cz];
    auto [wx,wz] = to_world(cx,cz);
    auto biome = biome_map[cx,cz];
    if (surface_block){

    }
});
```
