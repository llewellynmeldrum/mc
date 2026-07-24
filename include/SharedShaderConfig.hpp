#ifdef __cplusplus // SECTION: c++ specific versions of stuff
    #pragma once 
#endif 

#define TEXTURE_ID_COUNT 4
#define ENABLE_FAKE_SHADOWS 
#define DIR_COUNT 6

#ifdef __cplusplus 
// SECTION: c++ specific versions of stuff

#define START_DECL_ARRAY(T, N, name) std::array<T,N> name{                   
#define END_DECL_ARRAY() };
#else 
// SECTION: glsl specific versions of stuff
#define START_DECL_ARRAY(T, N, name)    T name[N] = T[N](
#define END_DECL_ARRAY() );
#endif


START_DECL_ARRAY(float, DIR_COUNT, faceShadowOpacity)
    0.15, // FORWARD,
    0.45, // BACKWARD
    0.35, // LEFT,   
    0.35, // RIGHT,//
    0.7, // DOWN, //
    0.0 // UP,     
END_DECL_ARRAY()
