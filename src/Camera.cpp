#include "Camera.hpp"
#include "glmWrapper.hpp"

void Camera::setupCamera(){
    pos = {0, 0, 6};
    dir_upwards =    {0.0f, 1.0f, 0.0f};
    front = {0.0f, 0.0f, -1.0f};

    std::function<mat4(vec3&, vec3, f32, f32)> callback = 
    [](vec3& front, vec3 pos, f32 yaw, f32 pitch) -> mat4{
        vec3 facing{};
        facing.x = cos(radians(yaw)) * cos(radians(pitch));
        facing.y = sin(radians(pitch));
        facing.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(facing);
        return lookAt(vec3(pos.x, pos.y,pos.z), 
                   pos+front, 
                   vec3(0.0f, 1.0f, 0.0f));
    };
    cached_viewMatrix = CachedValue<mat4, vec3&, vec3, f32, f32>(callback);
}

void Camera::move(Direction dir, f32 dt){
    switch (dir){
        case Direction::RIGHT: moveRight(dt); break;

        case Direction::LEFT: moveLeft(dt); break;

        case Direction::UP: moveUpward(dt); break;

        case Direction::DOWN: moveDownward(dt); break;

        case Direction::BACKWARD: moveBackward(dt); break;

        case Direction::FORWARD: moveForward(dt); break;
    }
    cached_viewMatrix.invalidate();
}
void Camera::rotate(Direction dir, f32 dt){

    switch (dir){
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
    pitch = glm::max(pitch,-89.0f);
    pitch = glm::min(pitch,89.0f);
    cached_viewMatrix.invalidate();
}
inline void Camera::moveUpward(f32 dt) { pos += dt * (moveSpeed * dir_upwards); }
inline void Camera::moveDownward(f32 dt) { pos -= dt * (moveSpeed * dir_upwards); }
inline void Camera::moveForward(f32 dt) { pos += dt * (moveSpeed * front); }
inline void Camera::yawLeft(f32 dt) { yaw -= dt * sensitivity; }
inline void Camera::yawRight(f32 dt) { yaw += dt * sensitivity; }
inline void Camera::pitchDown(f32 dt) { pitch -= dt * sensitivity; }
inline void Camera::pitchUp(f32 dt) { pitch += dt * sensitivity; }
inline void Camera::moveBackward(f32 dt) { pos -= dt * (moveSpeed * front); }
inline void Camera::moveLeft(f32 dt) { pos -= dt * (normalize(cross(front, dir_upwards)) * moveSpeed); }
inline void Camera::moveRight(f32 dt) { pos += dt * (normalize(cross(front, dir_upwards)) * moveSpeed); }
