#pragma once 

#include "Types.h"
#include <cmath>
#include <memory>

// convinience functions
template<class V>
constexpr inline f32 magsq(const V& v){
    v.magsq();
}
template<class V>
constexpr inline f32 mag(const V& v){
    v.mag();
}
template<class V>
constexpr inline f32 dot(const V& v){
    v.dot();
}

// I dont think a vec.normalize(void) makes much sense. Normalization is more of a free function type of thing                    
template<class V>
constexpr inline V normalize(const V v){
    auto len = mag(v);
    if constexpr (V::size() == 2){
        return V(v.x/len,v.y/len);
    }else if constexpr (V::size() == 3){
        return V(v.x/len,v.y/len, v.z/len);
    }else if constexpr (V::size() == 4){
        return V(v.x/len,v.y/len, v.z/len, v.w/len);
    }
}

struct vec2{
    f32 x,y;
    static consteval u64 size(){ 
        return 2;
    }
    f32* data() noexcept{
        return &x;
    }
    bool operator==(this const vec2& self, const vec2& other) {
        return  self.x==other.x &&
                self.y==other.y;
    }

    constexpr inline f32 magsq(const vec2& v){
        return sqrt(v.x*v.x + v.y*v.y);
    }
    constexpr inline f32 mag(const  vec2& v){
        return sqrt(magsq(v));
    }
    constexpr inline f32 dot(const vec2& lhs, const vec2& rhs){
        return lhs.x*rhs.x + lhs.y*rhs.y;
    }


 #define VEC2_OPEQ_SCALAR(op)\
[[nodiscard]]vec2& operator CONCAT(op,=)(this vec2& self, const f32 s) {\
    self.x CONCAT(op,=) s;\
    self.y CONCAT(op,=) s;\
    return self;\
}

#define VEC2_OP_SCALAR(OP)\
vec2 operator OP(this vec2 self, const f32 s) {\
    self CONCAT(OP,=) s;\
    return self; /* copy */\
}

#define VEC2_OP_VEC_EQ(op)\
[[nodiscard]]vec2& operator CONCAT(op,=)(this vec2& lhs, vec2& rhs) {\
    lhs.x CONCAT(op,=) rhs.x;\
    lhs.y CONCAT(op,=) rhs.y;\
    return lhs;\
}

#define VEC2_OP_VEC(OP)\
vec2 operator OP(this vec2 lhs, vec2& rhs) {\
    lhs CONCAT(OP,=) rhs;\
    return lhs;\
}

    VEC2_OPEQ_SCALAR(+)
    VEC2_OPEQ_SCALAR(-)
    VEC2_OPEQ_SCALAR(*)
    VEC2_OPEQ_SCALAR(/)
    VEC2_OP_SCALAR(+)
    VEC2_OP_SCALAR(-)
    VEC2_OP_SCALAR(*)
    VEC2_OP_SCALAR(/)

    VEC2_OP_VEC_EQ(+)
    VEC2_OP_VEC_EQ(-)
    VEC2_OP_VEC(+)

};

struct vec3{
    f32 x,y,z;
    static consteval u64 size(){ 
        return 3;
    }
    f32* data() noexcept{
        return &x;
    }
    bool operator==(this const vec3& self, const vec3& other) {
        return  self.x==other.x &&
                self.y==other.y && 
                self.z==other.z;
    }

    constexpr inline f32 magsq(const vec3& v){
        return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }
    constexpr inline f32 mag(const  vec3& v){
        return sqrt(magsq(v));
    }
    constexpr inline f32 dot(const vec3& lhs, const vec3& rhs){
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
    }

 #define VEC3_OPEQ_SCALAR(op)\
[[nodiscard]]vec3& operator CONCAT(op,=)(this vec3& self, const f32 s) {\
    self.x CONCAT(op,=) s;\
    self.y CONCAT(op,=) s;\
    self.z CONCAT(op,=) s;\
    return self;\
}

#define VEC3_OP_SCALAR(OP)\
vec3 operator OP(this vec3 self, const f32 s) {\
    self CONCAT(OP,=) s;\
    return self; /* copy */\
}

#define VEC3_OP_VEC_EQ(op)\
[[nodiscard]]vec3& operator CONCAT(op,=)(this vec3& lhs, vec3& rhs) {\
    lhs.x CONCAT(op,=) rhs.x;\
    lhs.y CONCAT(op,=) rhs.y;\
    lhs.z CONCAT(op,=) rhs.z;\
    return lhs;\
}

#define VEC3_OP_VEC(OP)\
vec3 operator OP(this vec3 lhs, vec3& rhs) {\
    lhs CONCAT(OP,=) rhs;\
    return lhs;\
}

    VEC3_OPEQ_SCALAR(+)
    VEC3_OPEQ_SCALAR(-)
    VEC3_OPEQ_SCALAR(*)
    VEC3_OPEQ_SCALAR(/)
    VEC3_OP_SCALAR(+)
    VEC3_OP_SCALAR(-)
    VEC3_OP_SCALAR(*)
    VEC3_OP_SCALAR(/)

    VEC3_OP_VEC_EQ(+)
    VEC3_OP_VEC_EQ(-)
    VEC3_OP_VEC(+)
};



struct vec4{
    f32 x,y,z,w;
    static consteval u64 size(){ 
        return 4;
    }
    const f32* data() const noexcept{
        return &x;
    }
    bool operator==(this const vec4& self, const vec4& other) {
        return  self.x==other.x &&
                self.y==other.y && 
                self.z==other.z && 
                self.w==other.w;
    }

    constexpr inline f32 magsq(const vec4& v){
        return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
    }
    constexpr inline f32 mag(const  vec4& v){
        return sqrt(magsq(v));
    }
    constexpr inline f32 dot(const vec4& lhs, const vec4& rhs){
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z + lhs.w*rhs.w;
    }

#define VEC4_OPEQ_SCALAR(op)\
[[nodiscard]]vec4& operator CONCAT(op,=)(this vec4& self, const f32 s) {\
    self.x CONCAT(op,=) s;\
    self.y CONCAT(op,=) s;\
    self.z CONCAT(op,=) s;\
    self.w CONCAT(op,=) s;\
    return self;\
}

#define VEC4_OP_SCALAR(OP)\
vec4 operator OP(this vec4 self, const f32 s) {\
    self CONCAT(OP,=) s;\
    return self; /* copy */\
}

#define VEC4_OP_VEC_EQ(op)\
[[nodiscard]]vec4& operator CONCAT(op,=)(this vec4& lhs, vec4& rhs) {\
    lhs.x CONCAT(op,=) rhs.x;\
    lhs.y CONCAT(op,=) rhs.y;\
    lhs.z CONCAT(op,=) rhs.z;\
    lhs.w CONCAT(op,=) rhs.w;\
    return lhs;\
}

#define VEC4_OP_VEC(OP)\
vec4 operator OP(this vec4 lhs, vec4& rhs) {\
    lhs CONCAT(OP,=) rhs;\
    return lhs;\
}

    VEC4_OPEQ_SCALAR(+)
    VEC4_OPEQ_SCALAR(-)
    VEC4_OPEQ_SCALAR(*)
    VEC4_OPEQ_SCALAR(/)
    VEC4_OP_SCALAR(+)
    VEC4_OP_SCALAR(-)
    VEC4_OP_SCALAR(*)
    VEC4_OP_SCALAR(/)

    VEC4_OP_VEC_EQ(+)
    VEC4_OP_VEC_EQ(-)
    VEC4_OP_VEC(+)
    VEC4_OP_VEC(-)
};
