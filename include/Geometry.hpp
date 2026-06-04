#pragma once
#include "CommonUtils.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"
#include "LM.hpp"
#include "Line3D.hpp"

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
    Plane(glm::vec3 origin, glm::vec3 _normal, std::string_view name = ""):
        normal(glm::normalize(_normal)), distance(glm::dot(normal, origin)) {
    };
    glm::vec3 normal{ 0, 1, 0 };
    f32  distance{};

};

struct AABB {
    AABB() = default;
    ~AABB() = default;


    glm::vec3 center = glm::vec3(0);
    glm::vec3 halfExtents = glm::vec3(0);
    // create an axis aligned bounding box from a min and a max world position
    template<AnyVec3 A, AnyVec3 B>
    AABB(A _min, B _max) {
        glm::vec3 min = _min.raw();
        glm::vec3 max = _max.raw();
        center = (min + max) * 0.5f;
        halfExtents = max - center;
    }
    inline std::vector<Line3D> getLines(glm::vec4 color){
        using namespace glm;
        auto extents = halfExtents*2.0f;
        auto b0 = center-halfExtents; // bl
        auto b1 = b0 + vec3{extents.x,0,0}; // br
        auto b2 = b1 + vec3{0,0,extents.z}; // tr
        auto b3 = b0 + vec3{0,0,extents.z}; // br
        //
        auto t0 = b0 + vec3{0,extents.y,0}; // bl
        auto t1 = b1 + vec3{0,extents.y,0}; // bl
        auto t2 = b2 + vec3{0,extents.y,0}; // bl
        auto t3 = b3 + vec3{0,extents.y,0}; // bl
        return {
            Line3D{b0,b1,0.25,color},
            Line3D{b1,b2,0.25,color},
            Line3D{b2,b3,0.25,color},
            Line3D{b3,b0,0.25,color},
            Line3D{b0,t0,0.25,color},
            Line3D{b1,t1,0.25,color},
            Line3D{b2,t2,0.25,color},
            Line3D{b3,t3,0.25,color},
            Line3D{t0,t1,0.25,color},
            Line3D{t1,t2,0.25,color},
            Line3D{t2,t3,0.25,color},
            Line3D{t3,t0,0.25,color},
        };
    }

    bool testForwardIntersection(const Plane& p) const;
};

struct Frustum {
    Frustum ()=default;
    ~Frustum ()=default;
    Frustum (const Camera * cam);
    Path3D path{};
    std::vector<Line3D> extra_lines;
    void update(this auto& self, const Camera* cam);

    inline bool isAABBInside(const AABB& v)const {
        return  v.testForwardIntersection(far)   && 
                v.testForwardIntersection(near)  &&
                v.testForwardIntersection(left)  &&
                v.testForwardIntersection(right) &&
                v.testForwardIntersection(top)   &&
                v.testForwardIntersection(bot);
    }
    inline bool isAABBInside(const AABB* v)const {
        return  v->testForwardIntersection(far)   && 
                v->testForwardIntersection(near)  &&
                v->testForwardIntersection(left)  &&
                v->testForwardIntersection(right) &&
                v->testForwardIntersection(top)   &&
                v->testForwardIntersection(bot);
    }

    Plane far;
    Plane near;
    Plane left;
    Plane right;
    Plane top;
    Plane bot;
};
