#include "Geometry.hpp"
#include "Camera.hpp"
#include "Line3D.hpp"
using namespace glm;
void Frustum::update(this auto& self, const Camera* cam) {
    self.path.clear(); 
    auto& path = self.path;
    const auto origin = cam->pos.raw();
    const f32& aspect = cam->aspectRatio;
    const f32& vFov = cam->vertical_fov;
    const f32& zNear = cam->near_clip_z;
    const f32& zFar = cam->far_clip_z;
    const auto pos = cam->pos.raw();
    const auto front = cam->getFront();
    const auto right = cam->getRight();
    const auto up = cam->getUp();

    path.thickness=0.10f;
    const f32 halfNearPlaneHeight = zNear * std::tan(glm::radians(vFov) / 2.0f);
    const f32 halfNearPlaneWidth = halfNearPlaneHeight * aspect;
    vec3 zNearMid = pos+(front*zNear);
    vec3 zNearTR = zNearMid+(right*halfNearPlaneWidth)+(up*halfNearPlaneHeight);
    vec3 zNearBR = zNearMid+(right*halfNearPlaneWidth)-(up*halfNearPlaneHeight);
    vec3 zNearBL = zNearMid-(right*halfNearPlaneWidth)-(up*halfNearPlaneHeight);
    vec3 zNearTL = zNearMid-(right*halfNearPlaneWidth)+(up*halfNearPlaneHeight);
//    path.add(zNearTR).add(zNearBR).add(zNearBL).add(zNearTL).add(zNearTR);

    const f32 halfFarPlaneHeight = zFar * std::tan(glm::radians(vFov) / 2.0f);

    //    aspect = w/h
    //    aspect * h = w
    const f32 halfFarPlaneWidth = halfFarPlaneHeight * aspect;

    vec3 zFarMid = pos+(front*zFar);
    vec3 zFarTR = zFarMid+(right*halfFarPlaneWidth)+(up*halfFarPlaneHeight);
    vec3 zFarBR = zFarMid+(right*halfFarPlaneWidth)-(up*halfFarPlaneHeight);
    vec3 zFarBL = zFarMid-(right*halfFarPlaneWidth)-(up*halfFarPlaneHeight);
    vec3 zFarTL = zFarMid-(right*halfFarPlaneWidth)+(up*halfFarPlaneHeight);
    path.add(zNearTR).add(zFarTR).add(zFarBR).add(zNearBR).add(zFarBR)
                    .add(zFarBL).add(zNearBL).add(zFarBL)
                    .add(zFarTL).add(zNearTL).add(zFarTL).add(zFarTR);
    const glm::vec3 frontMulFar = +front * zFar;

    // most of the math beyond the near and far can be demonstrated here:
    // https://www.desmos.com/3d/ywltep5xan
    self.near = Plane(pos + front * zNear, front,"near");
    self.far = Plane(pos + frontMulFar, -front,"far");

    self.right = Plane(pos, cross(frontMulFar - right * halfFarPlaneWidth, up),"right");
    self.left = Plane(pos, cross(up, frontMulFar + right * halfFarPlaneWidth),"left");

    self.top = Plane(pos, cross(right, frontMulFar - up * halfFarPlaneHeight),"top");

    self.bot = Plane(pos, cross(frontMulFar + up * halfFarPlaneHeight, right),"bot");
}
bool AABB::testForwardIntersection(const Plane& p) const {
        // returns true if the AABB is infront of or intersecting plane P
        // algorithm from https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
        // 1. Compute the 'projection interval radius'
        // clang-format off
        const glm::vec3 p_abs_normal = glm::abs(p.normal);
        const f32 pir = halfExtents.x * p_abs_normal.x
                    + halfExtents.y * p_abs_normal.y
                    + halfExtents.z * p_abs_normal.z;

        // 2. Compute dist(AABB_center, plane)
        f32 signed_dist = dot(p.normal, center) - p.distance;

        // 3. if (-pir <= signed_dist): `this` is infront of plane.
        return -pir <= signed_dist;
    }
Frustum::Frustum(const Camera* cam) {
    update(cam);
}
