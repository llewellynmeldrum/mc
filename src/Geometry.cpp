#include "Geometry.hpp"
#include "Camera.hpp"
#include "Line3D.hpp"
using namespace glm;
Line3D Plane::getNormalLine(glm::vec3 origin, glm::vec4 color){
    return Line3D{origin,(origin+distance)*-normal,1.0f,color};
}
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

    path.thickness=0.01f;
    const f32 halfNearPlaneHeight = zNear * std::tan(glm::radians(vFov) / 2.0f);
    const f32 halfNearPlaneWidth = halfNearPlaneHeight * aspect;
    vec3 zNearMid = pos+(front*zNear);
    vec3 zNearTR = zNearMid+(right*halfNearPlaneWidth)+(up*halfNearPlaneHeight);
    vec3 zNearBR = zNearMid+(right*halfNearPlaneWidth)-(up*halfNearPlaneHeight);
    vec3 zNearBL = zNearMid-(right*halfNearPlaneWidth)-(up*halfNearPlaneHeight);
    vec3 zNearTL = zNearMid-(right*halfNearPlaneWidth)+(up*halfNearPlaneHeight);
    path.add(zNearTR).add(zNearBR).add(zNearBL).add(zNearTL).add(zNearTR);

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
    const f32 halfFarPlaneHeight = zFar * std::tan(glm::radians(vFov) / 2.0f);

    //    aspect = w/h
    //    aspect * h = w
    const f32 halfFarPlaneWidth = halfFarPlaneHeight * aspect;

    vec3 zFarMid = pos+(front*zFar);
    vec3 zFarTR = zFarMid+(right*halfFarPlaneWidth)+(up*halfFarPlaneHeight);
    vec3 zFarBR = zFarMid+(right*halfFarPlaneWidth)-(up*halfFarPlaneHeight);
    vec3 zFarBL = zFarMid-(right*halfFarPlaneWidth)-(up*halfFarPlaneHeight);
    vec3 zFarTL = zFarMid-(right*halfFarPlaneWidth)+(up*halfFarPlaneHeight);
    path.add(zFarTR).add(zFarBR).add(zNearBR).add(zFarBR)
                    .add(zFarBL).add(zNearBL).add(zFarBL)
                    .add(zFarTL).add(zNearTL).add(zFarTL).add(zFarTR);
    const glm::vec3 frontMulFar = +front * zFar;

    // UP
    self.extra_lines.emplace_back(origin,origin+(up*halfFarPlaneHeight),0.1f,     Color01::RED  );
    self.extra_lines.emplace_back(origin,origin+(right*halfFarPlaneWidth),0.1f,   Color01::GREEN);
    self.extra_lines.emplace_back(origin,origin+(front*(zFar-zNear)),0.1f,        Color01::BLUE );
    self.extra_lines.emplace_back(origin,origin-(up*halfFarPlaneHeight),0.1f,     Color01::RED_a(0.5));
    self.extra_lines.emplace_back(origin,origin-(right*halfFarPlaneWidth),0.1f,   Color01::GREEN_a(0.5));
    self.extra_lines.emplace_back(origin,origin-(front*(zFar-zNear)),0.1f,        Color01::BLUE_a(0.5));
    // most of the math beyond the near and far can be demonstrated here:
    // https://www.desmos.com/3d/ywltep5xan
    self.near = Plane(pos + front * zNear, front);
    self.far = Plane(pos + frontMulFar, -front);

    self.right = Plane(pos, cross(frontMulFar - right * halfFarPlaneWidth, up));
    self.left = Plane(pos, cross(up, frontMulFar + right * halfFarPlaneWidth));

    self.top = Plane(pos, cross(right, frontMulFar - up * halfFarPlaneHeight));

    // BUG: WRONG, 
    self.bot = Plane(pos, cross(frontMulFar + up * halfFarPlaneHeight, right));
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
