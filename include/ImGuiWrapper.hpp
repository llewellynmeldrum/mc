#pragma once 
#include "Renderer.hpp"
#include "glm/ext/vector_float4.hpp"
#include <format>
#include <functional>
#include <string_view>
#include <utility>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "Types.h"


// ImGui wrapper to make use of some c++ stuff
namespace UI{
    using vec2 = ImVec2;
    using Color = ImVec4;
    namespace WinFlags{
        enum : i32{
            None                   = 0,
            NoTitleBar             = 1 << 0,   // Disable title-bar
            NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
            NoMove                 = 1 << 2,   // Disable user moving the window
            NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
            NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel
            NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Win
            AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
            NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as
            NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
            NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
            MenuBar                = 1 << 10,  // Has a menu-bar
            HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindow
            NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
            NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or progr
            AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
            AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
            NoNavInputs            = 1 << 16,  // No keyboard/gamepad navigation within the window
            NoNavFocus             = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by
            UnsavedDocument        = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is sele
            NoDocking              = 1 << 19,  // Disable docking of this window
            NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
            NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
            NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
        };
    }
    namespace WinFlagGroup{
        enum WinFlagGroup : i32{
            Overlay = UI::WinFlags::NoDecoration | UI::WinFlags::NoDocking |
                                            UI::WinFlags::AlwaysAutoResize | UI::WinFlags::NoSavedSettings |
                                            UI::WinFlags::NoFocusOnAppearing | UI::WinFlags::NoNav | UI::WinFlags::NoMove,
            MovableOverlay = UI::WinFlags::NoDecoration | UI::WinFlags::NoDocking |
                                             UI::WinFlags::NoSavedSettings |
                                            UI::WinFlags::NoFocusOnAppearing ,
            Normal = UI::WinFlags::None,
        };
    }
    inline void SetTextColor(glm::vec4 c){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(c.r,c.g,c.b,c.a));
    }
    inline void SetTextColor01(glm::vec4 c){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(c.r*255,c.g*255,c.b*255,c.a*255));
    }
    inline void DrawTexture(TextureTarget& target){
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 size =  ImGui::GetContentRegionAvail();

        ImGui::GetWindowDrawList()->AddImage(
            reinterpret_cast<void*>(target.texture.id), 
            ImVec2(pos.x, pos.y), 
            ImVec2(pos.x + size.x, pos.y + size.y), 
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );
    }
    inline void SetTextColor(Color c){
        ImGui::PushStyleColor(ImGuiCol_Text, c);
    }
    inline void setTextColor(i32 r, i32 g, i32 b, i32 a=255){
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(r,g,b,a));
    }
    inline void ResetTextColor(){
        ImGui::PopStyleColor();
    }
    template<typename... Args>
    inline void Text(std::format_string<Args...> fmt, Args&&... args){
        std::string out = std::vformat(fmt.get(), std::make_format_args(args...)); 
        ImGui::TextUnformatted(out.c_str());
    }

    template<typename... Args>
    inline void ColoredText01(glm::vec4 color, std::format_string<Args...> fmt, Args&&... args){
        SetTextColor01(color);
        Text(fmt,args...);
        ResetTextColor();
    }
    inline void ColoredText01(glm::vec4 color, std::string str){
        SetTextColor01(color);
        ImGui::TextUnformatted(str.c_str());
        ResetTextColor();
    }

    template<typename... Args>
    inline void ColoredText(glm::vec4 color, std::format_string<Args...> fmt, Args&&... args){
        SetTextColor(color);
        Text(fmt,args...);
        ResetTextColor();
    }

    inline void Text(const std::string& s){
        ImGui::TextUnformatted(s.c_str());
    }
    inline void SameLine(){
        ImGui::SameLine();
    }

    // screen pos 
    inline vec2 getCursorPos(){
        return ImGui::GetCursorScreenPos();
    }
    inline void Separator(){
        return ImGui::Separator();
    }


    inline bool StartWindow(const char* name, i32 flags, std::function<bool()> pred=[]{ return true;}){
        bool open = pred();
        return ImGui::Begin(name, &open, static_cast<int>(flags));
    }
    inline void EndWindow(){
    }
};
