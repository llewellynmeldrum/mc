#pragma once 
#include "FastNoiseLite.h"
#include "Types.h"


template<FastNoiseLite::NoiseType noiseType>
struct Noise2D{
    using NoiseType = FastNoiseLite::NoiseType;
    using FractalType = FastNoiseLite::FractalType;



    FastNoiseLite obj;
    Noise2D(i64 seed){
        obj.SetNoiseType(NoiseType::OpenSimplex2);
        obj.SetSeed(seed);
    }
    void setFreq(f32 f){ 
        obj.SetFrequency(f); 
    }
    void setFractalType(FractalType ft){ obj.SetFractalType(ft); }
    void setFractalOctaves(u32 octaves){ obj.SetFractalOctaves(octaves); }
    f32 sample(f32 x, f32 y){
        return obj.GetNoise(x,y);
    }
};
