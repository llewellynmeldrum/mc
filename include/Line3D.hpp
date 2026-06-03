#pragma once 
#include "CommonConcepts.hpp"
#include "Types.h"

extern f32 Line3D_thickness;
extern glm::vec4 Line3D_color;
struct Line3D{
    Line3D(glm::vec3 _p0, glm::vec3 _p1):
        p0(_p0), p1(_p1),thickness(Line3D_thickness),color(Line3D_color){}
    Line3D(glm::vec3 _p0, glm::vec3 _p1, f32 _thickness,glm::vec4 _color):
        p0(_p0), p1(_p1),thickness(_thickness),color(_color){}
    glm::vec3 p0;
    glm::vec3 p1;
    f32 thickness{1.0f};
    glm::vec4 color={1,1,1,1};

};
struct Path3D{
    std::vector<glm::vec3> segments{};
    float thickness{1.0f};
    glm::vec4 color{1,1,1,1};
    Path3D& add(glm::vec3 w_pos)&noexcept{
        segments.emplace_back(w_pos);
        return *this;
    }
    void publish(std::vector<Line3D>& dst)const noexcept{
        for (auto i = 0uz; i<segments.size()-1; i++){
            dst.emplace_back(segments[i],segments[i+1],thickness,color);
        }
    }
    void clear(){ segments.clear(); }
    
};


