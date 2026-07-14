# worldgen

1. TerrainZone      -> Ocean, Coastal, Inland  (decided by continentalness)
From there:
2. Biome ->ocean, warm_rainforest
3. TerrainElevation -> valley, flat, hill, mountain

eg: 
cold_desert + flat + coast = glacier
warm_rainforest + hill + inland = 





## Base uoise maps:
heatmap         [0,1]       cold=[0.0,0.45], warm=[0.45,0.55]     hot=[0.55,1.0]
rainmap         [0,1]       dry=[0.0,0.5],      wet=[0.5,1.0]
gradmap         [0,1]       flat=[0.0,0.5],     steep=[0.5,1.0]
contmap         [0,1]       ocean=[0.0,0.5],    coast=[0.4,0.6], inland=[0.5,1.0]


## Variants 
BiomeList{
    Ocean,
    Tundra,
    Desert,
    Plains,
    Beach,
}

(_,         _,          _,             ocean)      -> Ocean
(cold,      _,          _,             inland)     -> Tundra
(hot ,      _,          _,             inland)     -> Desert
(warm,      wet,        _,             inland)     -> Plains
(_,         _,          _,             coast)      -> Beach

```cpp



struct BiomeMatchTable{
    MinMax heat;
    MinMax rain;
    MinMax grad;
    MinMax cont;
    BiomeType biome;
};

// Representing the entire range, i.e "this biome doesnt care what the distance to x param is"
static constexpr inline auto ANY = MinMax(0.00f,1.00f);
static constexpr inline MinMax INLAND = {1.00f,1.00f};
static const inline std::vector<BiomeMatchTable> biome_match_tables{
  //{   .heat = {0.00f,0.00f},  .rain = {0.00f,0.00f} , .grad = {0.00f,0.00f},  .cont = {1.00f,1.00f},  .match = BiomeType::Example},

    {   .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = {0.00f},        .biome = BiomeType::Ocean },
    {   .heat = {0.00f},        .rain = ANY,            .grad = ANY,            .cont = {1.00f},        .biome = BiomeType::Tundra},
    {   .heat = {1.00f},        .rain = ANY,            .grad = ANY,            .cont = {1.00f},        .biome = BiomeType::Desert},
    {   .heat = {0.50f},        .rain = {0.50f},        .grad = ANY,            .cont = {1.00f},        .biome = BiomeType::Plains},
    {   .heat = ANY,            .rain = ANY,            .grad = ANY,            .cont = {0.50f},        .biome = BiomeType::Beach},
};


// GenConfig member
struct NoiseGenerator{
    const NoiseGen heat{NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = cfg.world_seed + 4823,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }};
    const NoiseGen rain{NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = cfg.world_seed + 93221,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }};
    const NoiseGen cont{NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = cfg.world_seed + 59392,
        .freq = 0.002f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 3,
        .frac_lacunarity = 1.5f,
        .frac_persistence = 0.9f,
    }};
    const NoiseGen grad{NoiseConfig{
        .type = NoiseType::Perlin,
        .seed = cfg.world_seed + 1234,
        .freq = 0.001f,
        .frac_type = FractalType::FBm,
        .frac_octaves = 5,
    }};
};

struct NoiseParams{
    f32 heat;
    f32 rain;
    f32 grad;
    f32 cont;
};

auto generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin){
    ArrayList2D<NoiseParams> res;
    const glm::ivec2 chunk_local_min = {0,0};
    const glm::ivec2 chunk_local_max = glm::ivec2{Chunk::Extents.x, Chunk::Extents.z};
    ForEachInRangeEx(chunk_local_min,chunk_local_max,[&](i32 cx, i32 cz){
        auto [wx,wz] = to_world(cx,cz);
        res[cx,cz].heat = cfg.noise.heat.sample(wx,wz); 
        res[cx,cz].rain = cfg.noise.rain.sample(wx,wz); 
        res[cx,cz].cont = cfg.noise.cont.sample(wx,wz); 
        res[cx,cz].grad = cfg.noise.grad.sample(wx,wz); 
    });
    return res;
}

auto classify_biome(const NoiseParams& noise){
    auto min_dist = numeric_max<f32>();
    BiomeType res = BiomeType::Ocean; // ?
    for (const auto& target: biome_match_tables){
        f32 dh = target.heat.dist_to(noise.heat);
        f32 dr = target.rain.dist_to(noise.rain);
        f32 dg = target.grad.dist_to(noise.cont);
        f32 dc = target.cont.dist_to(noise.cont);
        f32 dist = (dh+dr+dg+dc)/4.0f;
        if (dist < min_dist){
            min_dist = dist;
            res = target.biome;
        }
    }
    return res;
}

auto classify_each_column_biome(const ArrayList2D<NoiseParams>& noise_list){
    ArrayList2D<BiomeType> biome_list;
    for (const auto& noise: noise_list){
        res.push_back(classify_biome(noise));
    }
    return biome_list;
}







```
