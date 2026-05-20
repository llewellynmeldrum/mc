#include "Geometry.hpp"
#include "Camera.hpp"
using namespace glm;
void Frustum::update(this auto& self, const Camera* cam) {
    const f32& aspect = cam->aspectRatio;
    const f32& vFov = cam->vertical_fov;
    const f32& zNear = cam->near_clip_z;
    const f32& zFar = cam->far_clip_z;
    const auto pos = cam->pos;
    const auto front = cam->getFront();
    const auto right = cam->getRight();
    const auto up = cam->getUp();

    // Camera origin, and the far planes minY and maxY, (when viewed isometrically on the X
    // plane) form an isoscoles triangle. Split that into two identical RAT's -
    // -> let theta = camera_fov/2.0 (we split it in half)
    // -> let adj = the distance between camera and far plane (zfar)
    // -> let opp = 0.5 * farPlaneHeight.
    // -> solving for opp:
    //    tan(a)                = opp/adj
    //    tan(a) * adj          = opp
    //    tan(fov/2.0) * zfar   = 0.5 * farPlaneHeight
    //    (aka halfFarPlaneHeight)
    const f32 halfFarPlaneHeight = zFar * std::tan(vFov / 2.0f);

    //    aspect = w/h
    //    aspect * h = w
    const f32 halfFarPlaneWidth = halfFarPlaneHeight * aspect;

    const vec3 frontMulFar = front * zFar;
    // most of the math beyond the near and far can be demonstrated here:
    // https://www.desmos.com/3d/ywltep5xan
    self.near = Plane(pos + front * zNear, front);
    self.far = Plane(pos + frontMulFar, -front);

    self.right = Plane(pos, cross(frontMulFar - right * halfFarPlaneWidth, up));
    self.left = Plane(pos, cross(up, frontMulFar - right * halfFarPlaneWidth));

    self.top = Plane(pos, cross(right, frontMulFar - up * halfFarPlaneHeight));
    self.bot = Plane(pos, cross(frontMulFar + up * halfFarPlaneHeight, right));
}
Frustum::Frustum(const Camera* cam) {
    update(cam);
}
