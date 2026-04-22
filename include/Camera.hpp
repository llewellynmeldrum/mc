#pragma once 

#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"

// src/Camera.cpp
using namespace glm;
struct Camera{
public:
    Camera()=default;
    ~Camera()=default;
    void setupCamera();
    void move(Direction dir, f32 dt);
    void rotate(Direction dir, f32 dt);

    vec3 pos;               // world
    vec3 front{};           // world
    vec3 target{};          // world position of the target
    vec3 facing{};          // the (inverse) direction the camera is facing
    vec3 dir_upwards{};
    vec3 dir_rightwards{};
    f32 aspectRatio{};
    mat4 projection_matrix = mat4(1.0f);

    CachedValue<mat4,vec3&,  vec3, f32, f32> cached_viewMatrix{};

    f32 yaw = -90;
    f32 pitch = 0;
    f32 sensitivity = 1.5;
    f32 moveSpeed = 0.1f;

    f32 vertical_fov = 90.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 100.0f;



    inline mat4 getViewMatrix(){
        return cached_viewMatrix.get(front, pos, yaw, pitch);
    }
    inline mat4 getProjectionMatrix(){
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
