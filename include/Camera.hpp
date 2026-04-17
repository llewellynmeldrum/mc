#pragma once 

#include "Logger.hpp"
#include "Types.h"
#include "glm/geometric.hpp"
#include "glmWrapper.hpp"

using namespace glm;

struct Camera{
    vec3 pos; // world
    vec3 front{}; // world


    vec3 target{}; // world position of the target
    vec3 facing{}; // the (inverse) direction the camera is facing
    vec3 up{};
    vec3 right{};
    f32 moveSpeed = 0.1f;
    Camera(vec3 _pos): pos(_pos){
        up =    {0.0f, 1.0f, 0.0f};
        front = {0.0f, 0.0f, -1.0f};
    }

    


    f32 yaw = -90;
    f32 pitch = 0;

    f32 sensitivity = 0.5;
    mat4 getViewMatrix(){
        facing.x = cos(radians(yaw)) * cos(radians(pitch));
        facing.y = sin(radians(pitch));
        facing.z = sin(radians(yaw)) * cos(radians(pitch));
        front = normalize(facing);
        return glm::lookAt(glm::vec3(pos.x, pos.y,pos.z), 
                   pos+front, 
                   glm::vec3(0.0f, 1.0f, 0.0f));
    }
    f32 aspectRatio{};
    mat4 projection_matrix = mat4(1.0f);
    mat4 getProjectionMatrix(){
        return glm::perspective(radians(vertical_fov), aspectRatio, near_clip_z, far_clip_z);
    }

    f32 vertical_fov = 90.0f;
    f32 near_clip_z = 0.1f;
    f32 far_clip_z = 100.0f;

    void moveUpward(f32 dt){
        // TODO: implement
    }
    void moveDownward(f32 dt){
        // TODO: implement
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
        pos -= dt*( normalize(cross(front,up))*moveSpeed);
    }
    void moveRight(f32 dt){
        pos += dt*( normalize(cross(front,up))*moveSpeed);
    }

};
