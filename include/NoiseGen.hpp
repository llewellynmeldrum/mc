#include "FastNoiseLite.h"
#include "Types.h"

using NoiseType = FastNoiseLite::NoiseType;
using RotationType3D = FastNoiseLite::RotationType3D;
using FractalType = FastNoiseLite::FractalType;
using CellularDistanceFunction = FastNoiseLite::CellularDistanceFunction;
using CellularReturnType = FastNoiseLite::CellularReturnType;
using DomainWarpType = FastNoiseLite::DomainWarpType;

struct NoiseConfig{
public:
    NoiseType type;
    i32 seed_offset;
    f32 freq{}; // horizontal 'scale' of features
    FractalType frac_type = FractalType::None;
    i32 frac_octaves = {3};  
    f32 frac_lacunarity = {2.0f}; // frequency multiplier across octaves
    f32 frac_persistence = {0.5f}; // amplitude multiplier across octaves
};

struct NoiseGen{
public:
    FastNoiseLite obj;
    NoiseGen()= default;
    i32 seed_offset{};
    NoiseGen(i32 seed, NoiseConfig cfg):
        obj(make_fnl(seed, cfg))
    {}
    f32 sample(i32 cx, i32 cz)const noexcept{
        return obj.GetNoise(static_cast<f32>(cx),static_cast<f32>(cz));
    }
    f32 sample(i32 cx,i32 cy, i32 cz)const noexcept{
        return obj.GetNoise(static_cast<f32>(cx),static_cast<f32>(cy),static_cast<f32>(cz));
    }

private:
    static inline FastNoiseLite make_fnl(i32 seed, NoiseConfig cfg){
        FastNoiseLite _obj;
        apply_cfg(seed, &_obj,cfg);
        return _obj;
    }
    static inline void apply_cfg(i32 seed, FastNoiseLite* fnl, NoiseConfig cfg) {
        fnl->SetNoiseType(cfg.type);//type;
        fnl->SetSeed(seed + cfg.seed_offset);
        fnl->SetFrequency(cfg.freq);
        fnl->SetFractalType(cfg.frac_type);
        fnl->SetFractalOctaves(cfg.frac_octaves);
        fnl->SetFractalLacunarity(cfg.frac_lacunarity); 
        fnl->SetFractalGain(cfg.frac_persistence); // gain == persistence (multiplier of the octaves amplitude)
    }
};

    inline const std::vector<char const*> NoiseType_names
    {
		"OpenSimplex2",
		"OpenSimplex2S",
		"Cellular",
		"Perlin",
		"ValueCubic",
		"Value",
	};

    inline const std::vector<char const*> RotationType3D_names
    {
		"None",
		"ImproveXYPlanes",
		"ImproveXZPlanes",
	};

    inline const std::vector<char const*> FractalType_names
    {
		"None",
		"FBm",
		"Ridged",
		"PingPong",
		"DomainWarpProgressive",
		"DomainWarpIndependent",
	};

    inline const std::vector<char const*> CellularDistanceFunction_names
    {
		"Euclidean",
		"EuclideanSq",
		"Manhattan",
		"Hybrid",
	};

    inline const std::vector<char const*> CellularReturnType_names
    {
		"CellValue",
		"Distance",
		"Distance2",
		"Distance2Add",
		"Distance2Sub",
		"Distance2Mul",
		"Distance2Div",
	};

    inline const std::vector<char const*> DomainWarpType_names
    {
		"OpenSimplex2",
		"OpenSimplex2Reduced",
		"BasicGrid",
	};
