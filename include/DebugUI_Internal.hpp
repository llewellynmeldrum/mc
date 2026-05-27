#pragma once
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"
#include "glmVecTypes.hpp"
#include "ImGuiWrapper.hpp"
#include "CoordTypes.hpp"

namespace IG = ImGui;  // namespace alias for convinience
using Color = ImColor;
using ScreenPos = ImVec2;
template<typename T>
constexpr inline ScreenPos toScreenPos(T in){
    return ScreenPos{in.x,in.y};
}

constexpr inline ScreenPos PAD = {5.0f, 25.0f};
constexpr inline ScreenPos ALIGN_TOP_LEFT = {0.0f,0.0f};
constexpr inline ScreenPos ALIGN_TOP_RIGHT = {1.0f,0.0f};
constexpr inline ScreenPos ALIGN_MID_MID = {0.5f,0.5f};
constexpr inline f32 UI_SCALE = 1.25;


struct DebugDraw{
    ImDrawList* d = nullptr;
    ScreenPos transform={};
    ImColor fillColor{};
    ImColor strokeColor{};
    bool fillEnabled = false;

public:
    void selectWindow(){
        d = IG::GetWindowDrawList();
    }
    void fill(ImColor col){
        fillEnabled = true;
        fillColor = col;
    }
    void noFill(ImColor col){
        fillEnabled = false;
    }

    bool strokeEnabled = false;
    void stroke(ImColor col){
        strokeEnabled = true;
        strokeColor = col;
    }
    void noStroke(ImColor col){
        strokeEnabled = false;
    }

    void validate_context(){
        assert(d!=nullptr);
    }

    void pushWindowTransform(){
        transform = ImGui::GetCursorScreenPos();
    }

    void popWindowTransform(){
        transform={};
    }

    // NOTE: ImGui expects CLOCKWISE winding order 
    void rect(ScreenPos c, ScreenPos extents, f32 thick=1.0f){
        validate_context();
        ScreenPos h = {
            extents.x*0.5f,
            extents.y*0.5f
        };
        ScreenPos tl = {
            c.x-h.x,
            c.y-h.y,
        };
        ScreenPos br = {
            c.x+h.x,
            c.y+h.y,
        };
        tl+=transform; br+=transform;
        if (fillEnabled){
            d->AddRectFilled(tl,br,fillColor);
        }
        if (strokeEnabled){
            d->AddRect(tl,br,strokeColor,0.0f, 0, thick);
        }
    }
    void circle(ScreenPos c, f32 r, f32 thick=1.0f){
        validate_context();
        ScreenPos tl = {
            c.x-r,
            c.y-r,
        };
        ScreenPos br = {
            c.x+r,
            c.y+r,
        };
        c.x+=transform.x; 
        c.y+=transform.y; 
        if (fillEnabled){
            d->AddCircleFilled({c.x,c.y},r,fillColor);
        }
        if (strokeEnabled){
            d->AddCircle({c.x,c.y},r,fillColor,0,thick);
        }
    }

    void text(const std::string& str, ScreenPos ic, f32 fontSize=16.0f){
        validate_context();
        ScreenPos c = {ic.x,ic.y};
        c+=transform;
        ScreenPos he = IG::CalcTextSize(str.c_str())*0.5f;
        ScreenPos tm = {c.x,c.y};
        if (fillEnabled){
            d->AddText(tm,fillColor,str.c_str());
        }
    }
    template<typename... _Args>
    void textf(ScreenPos ic, f32 fontSize, std::format_string<_Args...> fmt, _Args&&... vargs){
        validate_context();
        auto str = std::vformat(fmt.get(), std::make_format_args(vargs...)); 
        ScreenPos c = {ic.x,ic.y};
        c+=transform;
        ScreenPos he = IG::CalcTextSize(str.c_str())*0.5f;
        ScreenPos tm = {c.x,c.y};
        if (fillEnabled){
            d->AddText(tm,fillColor,str.c_str());
        }
    }
};
struct ChunkMinimapSettings{
    float zoomScale = 1.0f;
};

struct ChunkMinimapSnapshot{
    // contains the state snapshots for each chunk

};

FORWARD_DECL_STRUCT(Simulation)
struct ChunkMinimap{
    ChunkMinimapSettings settings;
    ChunkMinimapSnapshot snapshot;

    DebugDraw& d;
    bool lock_chunk_y_to_cam=true;
    glm::vec3 camPos;
    i32 chunkY = 0;
    static constexpr i32 halfExtent = 32;
    static constexpr i32 ROWS = halfExtent*2;
    static constexpr i32 COLS = halfExtent*2;
    static constexpr i32 HEIGHT = halfExtent*2;
    static constexpr f32 cellScreenWidth = 20;
    static constexpr f32 cellScreenGap = 2;
    std::array<ChunkEntry*, ROWS*COLS*HEIGHT> vals;

    decltype(auto) at(this auto&& self, i32 x, i32 y, i32 z){
        return self.span()[x+halfExtent,y+halfExtent,z+halfExtent];
    }
    decltype(auto) at(this auto&& self, i32 x, i32 z){
        return self.at(x,self.chunkY,z);
    }
    decltype(auto) at(this auto&& self, WorldChunkCoord v){
        return self.at(v.x,v.y,v.z);
    }
    decltype(auto) span(this auto&& self){
        return std::mdspan(self.vals.data(),ROWS,HEIGHT, COLS);

    }
    void update(Simulation* ctx){
        const auto& coordList = ctx->world.generatedChunkCoordsInRadius({0,chunkY,0}, 8, 100);
        for (const auto& coord: coordList){
            if (coord.y<halfExtent && coord.y>=-halfExtent &&
                coord.x<halfExtent && coord.x>=-halfExtent &&
                coord.z<halfExtent && coord.z>=-halfExtent){
                at(coord) = ctx->world.chunkMap.get_entry(coord);
            }
        }
        camPos = toWorldChunkCoord(ctx->cam.pos);
        if (lock_chunk_y_to_cam){
            chunkY=camPos.y;
        }
    }
    ImColor defaultColor = ImColor(92, 89, 85);
    ImColor generatingColor = ImColor(184, 165, 140);
    ImColor generatedColor = ImColor(133, 81, 12);
    ImColor meshingColor = ImColor(255, 162, 0);
    ImColor meshedColor = ImColor(50, 255, 25);
    void showLegend(){
        d.selectWindow();
        d.pushWindowTransform();
        ScreenPos pos = {12,35};
        #define showColorPreview(name)\
        draw.fill(name);\
        draw.rect(pos,{cellScreenWidth,cellScreenWidth});\
        draw.fill(ImColor{255,255,255,255});\
        draw.text(#name, {pos.x+cellScreenWidth, pos.y-12}, 12.0f);\
        pos.y+=cellScreenWidth+cellScreenGap;\

        showColorPreview(defaultColor)
        showColorPreview(generatingColor)
        showColorPreview(generatedColor)
        showColorPreview(meshingColor)
        showColorPreview(meshedColor)

    }
    void showMap(){
        d.fill(ImColor(255,120,255));
        d.stroke(ImColor(255,255,255));
        for (const auto [ix,iz] :EachInRange(-halfExtent,halfExtent, -halfExtent,halfExtent)){
            auto px = ix * cellScreenWidth + ix*cellScreenGap;
            auto py = iz * cellScreenWidth + iz*cellScreenGap;
            const auto& status = at(ix,iz)->status;
            d.fill(defaultColor);
            if (at(ix,iz)){
                if (status.generating){
                    d.fill(generatingColor);
                }
                if (status.generated){
                    d.fill(generatedColor);
                }
                if (status.meshing){
                    d.fill(meshingColor);
                }
                if (status.meshed){
                    d.fill(meshedColor);
                }
            }
            d.rect({px,py},{cellScreenWidth,cellScreenWidth});
        }
    }
    void showChunkMinimap(){
        d.selectWindow();
        d.pushWindowTransform();
        d.transform += IG::GetWindowSize()*.5;
        
        showMap();

        d.fill(ImColor(255,0,0));
        for (const auto ix :EachInRange(0,COLS)){
            ScreenPos pos{
                ix * (cellScreenWidth + cellScreenGap),
                50,
            };
            d.text(std::format("{}",ix-halfExtent),{pos.x,pos.y});
        }
        for (const auto iz :EachInRange(0,COLS)){
            f32 px = 50;
            auto py = iz * cellScreenWidth + iz*cellScreenGap;
            d.text(std::format("{}",iz-halfExtent),{px,-py});
        }
        d.fill(ImColor(255,0,0));
        auto toscreen =[](auto v){
            auto px = v.x * cellScreenWidth + v.x*cellScreenGap;
            auto py = v.z * cellScreenWidth + v.z*cellScreenGap;
            return ScreenPos{px,py};
        };
        d.circle(toscreen(camPos),50);
    }
};
