#define sq(x) (x*x)
#define rgb(r,g,b) vec3(r/255.0, g/255.0, b/255.0)
#ifndef f32
    #define f32 float
#endif

#define RGB(r,g,b) vec3(r,g,b)
#define ROW_MAJOR_IDX2D(p, width, height) (((p.y) * (width)) + (p.x))
