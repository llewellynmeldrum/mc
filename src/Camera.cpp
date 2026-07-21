#include "DebugFormat.hpp"
#include "FormatSpecs.hpp"
#include "Breakpoints.hpp"

#include "Camera.hpp"
#include "glmWrapper.hpp"

using namespace glm;
Camera::Camera(WorldFloatPos pos, f32 pitch, f32 yaw) {
    set_pos_ori(pos,pitch,yaw);
}
void Camera::set_pos(WorldFloatPos pos) {
    this-> pos = pos;
    cached_viewMatrix.invalidate();
    cached_frustum.invalidate();
}
void Camera::set_orientation(f32 pitch, f32 yaw) {
    this->pitch = pitch;
    this->yaw = yaw;
    cached_viewMatrix.invalidate();
    cached_frustum.invalidate();
}
void Camera::set_pos_ori(WorldFloatPos pos, f32 pitch, f32 yaw) {
    this-> pos = pos;
    this->pitch = pitch;
    this->yaw = yaw;
    cached_viewMatrix.invalidate();
    cached_frustum.invalidate();
}

void Camera::move(Direction dir, f32 dt) {
    switch (dir) {
    case Direction::RIGHT:
        moveRight(dt);
        break;

    case Direction::LEFT:
        moveLeft(dt);
        break;

    case Direction::UP:
        moveUpward(dt);
        break;

    case Direction::DOWN:
        moveDownward(dt);
        break;

    case Direction::BACKWARD:
        moveBackward(dt);
        break;

    case Direction::FORWARD:
        moveForward(dt);
        break;
    default:
        BREAKPOINT();
        break;
    }
    cached_viewMatrix.invalidate();
    cached_frustum.invalidate();
    WorldBlockPos new_block_pos =  toWorldBlockPos(pos);
    block_pos = new_block_pos;
}
void Camera::rotateByMouse(vec2 offset, f32 dt) {
    if (enable_mouse_panning){
        pitch += dt * mouse_sensitivity * offset.y;
        yaw -= dt * mouse_sensitivity * offset.x;  // up down is inverted
        pitch = glm::max(pitch, -89.0f);
        pitch = glm::min(pitch, 89.0f);
        cached_viewMatrix.invalidate();
        cached_frustum.invalidate();
    }
}
void Camera::rotate(Direction dir, f32 dt) {

    switch (dir) {
    case Direction::UP:
        pitchUp(dt);
        break;

    case Direction::DOWN:
        pitchDown(dt);
        break;

    case Direction::LEFT:
        yawLeft(dt);
        break;

    case Direction::RIGHT:
        yawRight(dt);
        break;

    default:
        LOG_ERROR("invalid rotation value '{}' passed to {}", (int)dir, __FUNCTION__);
        break;
    }
    pitch = glm::max(pitch, -89.0f);
    pitch = glm::min(pitch, 89.0f);
    cached_viewMatrix.invalidate();
    cached_frustum.invalidate();
}
inline void Camera::moveUpward(f32 dt) {
    pos += FloatOffset{dt * (moveSpeed * WorldUp())};
}
inline void Camera::moveDownward(f32 dt) {
    pos -= FloatOffset{dt * (moveSpeed * WorldUp())};
}
inline void Camera::moveForward(f32 dt) {
    pos += FloatOffset{dt * (moveSpeed * getFront())};
}
inline void Camera::yawLeft(f32 dt) {
    yaw -= dt * keyboard_sensitivity;
}
inline void Camera::yawRight(f32 dt) {
    yaw += dt * keyboard_sensitivity;
}
inline void Camera::pitchDown(f32 dt) {
    pitch -= dt * keyboard_sensitivity;
}
inline void Camera::pitchUp(f32 dt) {
    pitch += dt * keyboard_sensitivity;
}
inline void Camera::moveBackward(f32 dt) {
    pos -= FloatOffset{dt * (moveSpeed * getFront())};
}
inline void Camera::moveLeft(f32 dt) {
    pos -= FloatOffset{dt * (normalize(cross(getFront(), getUp())) * moveSpeed)};
}
inline void Camera::moveRight(f32 dt) {
    pos += FloatOffset{dt * (normalize(cross(getFront(), getUp())) * moveSpeed)};
}
