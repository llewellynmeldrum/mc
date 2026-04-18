#pragma once 

#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "CommonUtils.hpp"
#include "glmWrapper.hpp"

using namespace glm;
struct Camera{
    vec3 pos; // world
    f32 yaw = -90;
    f32 pitch = 0;
    vec3 front{}; // world

    vec3 target{}; // world position of the target
    vec3 facing{}; // the (inverse) direction the camera is facing
    vec3 dir_upwards{};
    vec3 dir_rightwards{};
    CachedValue<mat4,vec3&,  vec3, f32, f32> cached_viewMatrix{};

    std::function<mat4(vec3&, vec3, f32, f32)> callback = [](vec3& front, vec3 pos, f32 yaw, f32 pitch) -> mat4{
        vec3 facing{};
        facing.x = cos(radians(yaw)) * cos(radians(pitch));
        facing.y = sin(radians(pitch));
        facing.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(facing);
        return lookAt(vec3(pos.x, pos.y,pos.z), 
                   pos+front, 
                   vec3(0.0f, 1.0f, 0.0f));
    };
    Camera(vec3 _pos): pos(_pos){
        dir_upwards =    {0.0f, 1.0f, 0.0f};
        front = {0.0f, 0.0f, -1.0f};
        cached_viewMatrix = CachedValue<mat4, vec3&, vec3, f32, f32>(callback);
    }

    f32 sensitivity = 1.5;
    f32 moveSpeed = 0.1f;

    f32 vertical_fov = 90.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 100.0f;



    mat4 getViewMatrix(){
        return cached_viewMatrix.get(front, pos, yaw, pitch);
    }
    f32 aspectRatio{};
    mat4 projection_matrix = mat4(1.0f);
    mat4 getProjectionMatrix(){
        return glm::perspective(radians(vertical_fov), aspectRatio, near_clip_z, far_clip_z);
    }


    void move(Direction dir, f32 dt){
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
    void rotate(Direction dir, f32 dt){

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
private:
    void moveUpward(f32 dt){
        pos += dt*(moveSpeed * dir_upwards);
    }
    void moveDownward(f32 dt){
        pos -= dt*(moveSpeed * dir_upwards);
    }
    void moveForward(f32 dt){
        pos += dt*( moveSpeed * front);
    }
    void yawLeft(f32 dt){
        yaw-= dt*sensitivity;
    }
    void yawRight(f32 dt){
        yaw+= dt*sensitivity;
    }
    void pitchDown(f32 dt){
        pitch-= dt*sensitivity;
    }
    void pitchUp(f32 dt){
        pitch+= dt*sensitivity;
    }
    void moveBackward(f32 dt){
        pos -= dt*( moveSpeed * front);
    }
    void moveLeft(f32 dt){
        pos -= dt*( normalize(cross(front,dir_upwards))*moveSpeed);
    }
    void moveRight(f32 dt){
        pos += dt*( normalize(cross(front,dir_upwards))*moveSpeed);
    }

};
