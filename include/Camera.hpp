#pragma once

#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"

// src/Camera.cpp
using namespace glm;
struct Camera {
  public:
    Camera() = default;
    ~Camera() = default;
    static constexpr f32 BASE_MOVESPEED = 0.2f;
    static constexpr f32 SPRINT_MOVESPEED = 5.0f;

    void setupCamera();
    void move(Direction dir, f32 dt);
    void rotate(Direction dir, f32 dt);
    void rotateByMouse(vec2 offset, f32 dt);

    bool requestsMeshRegen = true;
    // at the moment, any movement causes mesh regen for all chunks.
    // Should probably only be those that are visible.
    inline void
    requestMeshRegeneration() {
        requestsMeshRegen = true;
    }

    vec3  pos;  // world
    ivec3 block_pos;
    vec3  front{};   // world
    vec3  target{};  // world position of the target
    vec3  facing{};  // the (inverse) direction the camera is facing
    vec3  dir_upwards{};
    vec3  dir_rightwards{};
    f32   aspectRatio{};
    mat4  projection_matrix = mat4(1.0f);

    CachedValue<mat4, vec3&, vec3, f32, f32> cached_viewMatrix{};

    f32 yaw = -90;
    f32 pitch = 0;
    f32 mouse_sensitivity = 100;
    f32 keyboard_sensitivity = 1.5;
    f32 moveSpeed = Camera::BASE_MOVESPEED;

    f32 vertical_fov = 90.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 1000.0f;

    inline mat4
    getViewMatrix() {
        return cached_viewMatrix.get(front, pos, yaw, pitch);
    }
    inline mat4
    getProjectionMatrix() {
        return glm::perspective(radians(vertical_fov), aspectRatio, near_clip_z, far_clip_z);
    }

  private:
    void moveUpward(f32 dt);
    void moveDownward(f32 dt);
    void moveForward(f32 dt);
    void yawLeft(f32 dt);
    void yawRight(f32 dt);
    void pitchDown(f32 dt);
    void pitchUp(f32 dt);
    void moveBackward(f32 dt);
    void moveLeft(f32 dt);
    void moveRight(f32 dt);
};
