#ifndef SHADERS_NOISE_GLSL
#define SHADERS_NOISE_GLSL

#ifndef f32
#define f32 float
#endif

vec3 permute(vec3 x) { return mod(((x*34.0f)+1.0f)*x, 289.0f); }

float simplex2d(vec2 v){
  const vec4 C = vec4(0.211324865405187f, 0.366025403784439f,
           -0.577350269189626f, 0.024390243902439f);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0f, 0.0f) : vec2(0.0f, 1.0f);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0f);
  vec3 p = permute( permute( i.y + vec3(0.0f, i1.y, 1.0f ))
  + i.x + vec3(0.0f, i1.x, 1.0f ));
  vec3 m = max(0.5f - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0f);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0f * fract(p * C.www) - 1.0f;
  vec3 h = abs(x) - 0.5f;
  vec3 ox = floor(x + 0.5f);
  vec3 a0 = x - ox;
  m *= 1.79284291400159f - 0.85373472095314f * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0f * dot(m, g);
}
float simplex2d_octaves(vec2 uv, uint octave_count, f32 lacunarity, f32 persistence){
    f32 freq = 2.0f;
    f32 amp = 0.5f;
    f32 res = 0.0f;
    for (uint i = 0u; i <octave_count; i++){
        res+= simplex2d(uv*freq) * amp;
        freq *=lacunarity;
        amp *=persistence;
    }
    return res;
}


#endif //SHADERS_NOISE_GLSL
