#pragma once
#include "CommonUtils.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"

FORWARD_DECL_STRUCT(Camera);

constexpr vec3 WorldVector(Direction dir){
    switch (dir){
        case  Direction::FORWARD:   return ivec3{  0,  0, -1 }; break;
        case  Direction::BACKWARD:  return ivec3{  0,  0, +1 }; break;
        case  Direction::LEFT:      return ivec3{ -1,  0,  0 }; break;
        case  Direction::RIGHT:     return ivec3{ +1,  0,  0 }; break;
        case  Direction::DOWN:      return ivec3{  0, -1,  0 }; break;
        case  Direction::UP:        return ivec3{  0, +1,  0 }; break;
        default: return ivec3{};
    }
}

struct Plane {
    Plane() = default;
    ~Plane() = default;
    Plane(vec3 origin, vec3 norm) : normal(norm), distance(glm::dot(norm, origin)) {};
    vec3 normal{ 0, 1, 0 };
    f32  distance{};
};

struct AABB {
    AABB() = default;
    ~AABB() = default;

    vec3 center = vec3(0);
    vec3 extents = vec3(0);
    AABB(const vec3& min, const vec3& max) {
        center = min + max * 0.5f;
        extents = max - center;
    }

    // returns true if the AABB is infront of or intersecting plane P
    // algorithm from https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
    inline bool testForwardIntersection(const Plane& p) const {
        //
        // 1. Compute the 'projection interval radius'
        // clang-format off
        const vec3 p_abs_normal = glm::abs(p.normal);
        const f32 r = extents.x * p_abs_normal.x
                    + extents.y * p_abs_normal.y
                    + extents.z * p_abs_normal.z;

        //2. Compute dist(AABB_center, plane)
        f32 signed_dist = dot(p.normal, center) - p.distance;
        return -r <= signed_dist;
    }
};

struct Frustum {
    Frustum ()=default;
    ~Frustum ()=default;
    Frustum (const Camera * cam);
    void update(this auto& self, const Camera* cam);

    inline bool isAABBInside(const AABB& v)const {
        return  v.testForwardIntersection(far)   && 
                v.testForwardIntersection(near)  &&
                v.testForwardIntersection(left)  &&
                v.testForwardIntersection(right) &&
                v.testForwardIntersection(top)   &&
                v.testForwardIntersection(bot);
    }

    Plane far;
    Plane near;
    Plane left;
    Plane right;
    Plane top;
    Plane bot;
};
