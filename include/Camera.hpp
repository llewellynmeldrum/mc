#pragma once

#include "Colors01.hpp"
#include "CoordTypes.hpp"
#include "DEBUG.hpp"
#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"
#include "Geometry.hpp"
#include <memory>
// src/Camera.cpp
struct Frustum;
struct Camera {

  public:
    bool isPlayer=false;
    glm::vec4 lineColor=Color01::BLUE;
    Camera() = default;
    ~Camera() = default;
    Camera(WorldFloatPos pos, f32 pitch, f32 yaw);
    static constexpr f32 BASE_MOVESPEED = 12.0f;
    static constexpr f32 SPRINT_MOVESPEED = 300.0f;
    static constexpr f32 SPRINT_KEYBOARD_SENSITVITY= 50.5f;
    static constexpr f32 BASE_KEYBOARD_SENSITIVITY = 1.5f;

    static constexpr i32 DebugChunkRenderDistance = 8;

    void set_pos_ori(WorldFloatPos pos, f32 pitch, f32 yaw);
    void move(Direction dir, f32 dt);
    void rotate(Direction dir, f32 dt);
    void rotateByMouse(glm::vec2 offset, f32 dt);

    bool requestsMeshRegen = true;
    // at the moment, any movement causes mesh regen for all chunks.
    // Should probably only be those that are visible.
    inline void requestMeshRegeneration() { requestsMeshRegen = true; }

    WorldFloatPos pos{};       // world
    // height * aspectRatio = width
    f32  aspectRatio{};

    //static constexpr f32 ORTHO_ZOOM_MIN{0.1f};
    //static constexpr f32 ORTHO_ZOOM_MAX{10.0f};

    //f32 ortho_zoom{1.0f};
    Bounded<f32> ortho_zoom{
        .base = 1.0f,
        .min = 0.1f,
        .max = 10.f,
    };

    f32 zoom_sens{2.0f};
    glm::mat4 projection_matrix = glm::mat4(1.0f);

    CachedValue<glm::mat4>    cached_viewMatrix{};
    CachedValue<Frustum> cached_frustum{};
    bool enable_mouse_panning = true;
    inline void enableMousePanning(){
        enable_mouse_panning = true;
    }
    inline void disableMousePanning(){
        enable_mouse_panning = false;
    }

    inline const Frustum& getFrustum() const {
        return cached_frustum.get(
            [this](){
                std::println(stderr,"Updated frustum.");
                return Frustum(this);  // NOLINT
            }
        );
    }
    inline glm::vec3 getFacing() const{
        using namespace glm;
        return vec3{
            cos(radians(yaw)) * cos(radians(pitch)),
            sin(radians(pitch)),
            sin(radians(yaw)) * cos(radians(pitch)),
        };
    };

    inline glm::vec3 getFront() const {
        return normalize(getFacing());
    }
    inline glm::vec3 getUp() const{
        return normalize(cross(getRight(), getFront()));
    }
    inline glm::vec3 getRight() const{
        return normalize(cross(getFront(), WorldUp()));
    }
    inline glm::vec3 getLeft() const{
        return normalize(cross(WorldUp(),getFront()));
    }
    inline const glm::mat4& getViewMatrix() { 
        return cached_viewMatrix.get([this](){
            return glm::lookAt(pos.raw(), pos.raw() + getFront(), WorldUp());
        });
    }

    f32 yaw = -90;
    f32 pitch = 0;
    f32 mouse_sensitivity = 6000;
    f32 keyboard_sensitivity = BASE_KEYBOARD_SENSITIVITY;

    f32 vertical_fov = 70.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 1000.0f;
    f32 moveSpeed = Camera::BASE_MOVESPEED;

    inline glm::mat4 getProjectionMatrix() const{
        if (isPlayer){
            return glm::perspective(glm::radians(vertical_fov), aspectRatio, near_clip_z, far_clip_z);
        }else{

            f32 scale = 9.0f * ortho_zoom;
            f32 xscale = scale*aspectRatio;
            return glm::ortho(-xscale, xscale, -scale, scale, near_clip_z, far_clip_z);
        }
    }

  private:
    static inline constexpr glm::vec3 WorldUp(){
        return WorldVector(Direction::UP);
    }
    WorldBlockPos block_pos;  // read only var?
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
