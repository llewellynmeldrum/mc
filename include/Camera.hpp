#pragma once

#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
#include "Geometry.hpp"
// src/Camera.cpp
struct Frustum;
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
    inline void requestMeshRegeneration() { requestsMeshRegen = true; }

    vec3 pos{};       // world
    f32  aspectRatio{};
    mat4 projection_matrix = mat4(1.0f);

    CachedValue<mat4>    cached_viewMatrix{};
    CachedValue<Frustum> cached_frustum{};

    inline const Frustum& getFrustum(){
        return cached_frustum.get([this](){
            return Frustum(this);  // NOLINT
        });
    }
    inline vec3 getFacing() const{
        return vec3{
            cos(radians(yaw)) * cos(radians(pitch)),
            sin(radians(pitch)),
            sin(radians(yaw)) * cos(radians(pitch)),
        };
    };

    inline vec3 getFront() const {
        return normalize(getFacing());
    }
    inline vec3 getUp() const{
        return normalize(cross(getRight(), getFront()));
    }
    inline vec3 getRight() const{
        return normalize(cross(getFront(), WorldUp()));
    }
    inline const mat4& getViewMatrix() { 
        return cached_viewMatrix.get([this](){
            return lookAt(pos, pos + getFront(), WorldUp());
        });
    }

    inline vec3 getRight() {
        const auto& v = getViewMatrix();
        return { v[0][0], v[1][0], v[2][0] };
    }
    f32 yaw = -90;
    f32 pitch = 0;
    f32 mouse_sensitivity = 100;
    f32 keyboard_sensitivity = 1.5;
    f32 moveSpeed = Camera::BASE_MOVESPEED;

    f32 vertical_fov = 90.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 1000.0f;

    inline mat4 getProjectionMatrix() {
        return glm::perspective(radians(vertical_fov), aspectRatio, near_clip_z, far_clip_z);
    }

  private:
    static inline constexpr vec3 WorldUp(){
        return WorldVector(Direction::UP);
    }
    ivec3 block_pos;  // read only var?
    void  moveUpward(f32 dt);
    void  moveDownward(f32 dt);
    void  moveForward(f32 dt);
    void  yawLeft(f32 dt);
    void  yawRight(f32 dt);
    void  pitchDown(f32 dt);
    void  pitchUp(f32 dt);
    void  moveBackward(f32 dt);
    void  moveLeft(f32 dt);
    void  moveRight(f32 dt);
};
