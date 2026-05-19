#pragma once 
#include "FastNoiseLite.h"
#include "Types.h"
#include <utility>

#define FNL_ENUM_WRAPPER(ENUM_T)\
constexpr inline FastNoiseLite::ENUM_T to_##ENUM_T(ENUM_T val){\
    return static_cast<FastNoiseLite::ENUM_T>(val);\
}

enum struct NoiseType
{
    OpenSimplex2,
    OpenSimplex2S,
    Cellular,
    Perlin,
    ValueCubic,
    Value
};
FNL_ENUM_WRAPPER(NoiseType)

enum struct RotationType3D
{
    None,
    ImproveXYPlanes,
    ImproveXZPlanes
};
FNL_ENUM_WRAPPER(RotationType3D)

enum struct FractalType
{
    None,
    FBm,
    Ridged,
    PingPong,
    DomainWarpProgressive,
    DomainWarpIndependent
};
FNL_ENUM_WRAPPER(FractalType)



enum struct CellularDistanceFunction
{
    Euclidean,
    EuclideanSq,
    Manhattan,
    Hybrid
};
FNL_ENUM_WRAPPER(CellularDistanceFunction)

enum struct CellularReturnType
{
    CellValue,
    Distance,
    Distance2,
    Distance2Add,
    Distance2Sub,
    Distance2Mul,
    Distance2Div
};
FNL_ENUM_WRAPPER(CellularReturnType)

enum struct DomainWarpType
{
    OpenSimplex2,
    OpenSimplex2Reduced,
    BasicGrid
};
FNL_ENUM_WRAPPER(DomainWarpType)

struct Noise2D{
 public:
    i64 seed{};
    NoiseType noiseType{};

    Noise2D(NoiseType _noiseType): 
        Noise2D(_noiseType, 1337) {}

    Noise2D(NoiseType _noiseType, i64 _seed) :
        seed(_seed),
        noiseType(_noiseType),
        obj(_seed){}

    Noise2D(const Noise2D& other) :
        seed(other.seed),
        noiseType(other.noiseType),
        obj(other.seed){}

    Noise2D(Noise2D&& other) :
        seed(other.seed),
        noiseType(other.noiseType),
        obj(other.seed){}


    Noise2D operator=(const Noise2D& other)=delete;
    Noise2D operator=(Noise2D&& other)=delete;

    ~Noise2D() = default;


    inline void setSeed(i64 seed){
        this->seed = seed;
        obj.SetSeed(seed);
    }
    inline void setNoiseType(NoiseType nt){ 
        obj.SetNoiseType(to_NoiseType(nt)); 
    }
    inline void setFreq(f32 f){ 
        obj.SetFrequency(f); 
    }
    inline void setFractalType(FractalType ft){
        obj.SetFractalType(to_FractalType(ft)); 
    }
    inline void setFractalOctaves(u32 n_octaves){
        obj.SetFractalOctaves(n_octaves); 

    }
    inline f32 sample(f32 x, f32 y){
        return obj.GetNoise(x,y);
    }

 private:
    FastNoiseLite obj;
};
