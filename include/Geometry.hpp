#pragma once
#include "CommonUtils.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"
#include "LM.hpp"
#include "Line3D.hpp"
#include "Direction.hpp"

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
    AABB(glm::vec3 min, glm::vec3 max) {
        center = (min + max) * 0.5f;
        halfExtents = max - center;
    }
    std::vector<Line3D> getLines(glm::vec4 color, bool includeCrissCross=false);
    

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
