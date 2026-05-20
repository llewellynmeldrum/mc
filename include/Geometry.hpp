#pragma once
#include "CommonUtils.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"

FORWARD_DECL_STRUCT(Camera);

constexpr glm::vec3 WorldVector(Direction dir){
    switch (dir){
        case  Direction::FORWARD:   return glm::ivec3{  0,  0, -1 }; break;
        case  Direction::BACKWARD:  return glm::ivec3{  0,  0, +1 }; break;
        case  Direction::LEFT:      return glm::ivec3{ -1,  0,  0 }; break;
        case  Direction::RIGHT:     return glm::ivec3{ +1,  0,  0 }; break;
        case  Direction::DOWN:      return glm::ivec3{  0, -1,  0 }; break;
        case  Direction::UP:        return glm::ivec3{  0, +1,  0 }; break;
        default: return glm::ivec3{};
    }
}

struct Plane {
    Plane() = default;
    ~Plane() = default;
    Plane(glm::vec3 origin, glm::vec3 norm) : normal(norm), distance(glm::dot(norm, origin)) {};
    glm::vec3 normal{ 0, 1, 0 };
    f32  distance{};
};

struct AABB {
    AABB() = default;
    ~AABB() = default;


    glm::vec3 center = glm::vec3(0);
    glm::vec3 extents = glm::vec3(0);
    AABB(const glm::vec3& min, const glm::vec3& max) {
        center = min + max * 0.5f;
        extents = max - center;
    }

    inline bool testForwardIntersection(const Plane& p) const {
        // returns true if the AABB is infront of or intersecting plane P
        // algorithm from https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
        // 1. Compute the 'projection interval radius'
        // clang-format off
        const glm::vec3 p_abs_normal = glm::abs(p.normal);
        const f32 pir = extents.x * p_abs_normal.x
                    + extents.y * p_abs_normal.y
                    + extents.z * p_abs_normal.z;

        // 2. Compute dist(AABB_center, plane)
        f32 signed_dist = dot(p.normal, center) - p.distance;

        // 3. if (-pir <= signed_dist): `this` is infront of plane.
        return -pir <= signed_dist;
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
