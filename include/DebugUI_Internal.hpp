#pragma once
#include "CommonConcepts.hpp"
#include "DebugUI.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "glmWrapper.hpp"
#include "glmVecTypes.hpp"
#include "ImGuiWrapper.hpp"
#include "CoordTypes.hpp"
#include "imgui.h"

namespace IG =ImGui;

using ScreenPos = ImVec2;
// UV positions range between [0.0,1.0] on x and y, with a top left origin 
using UVPos = glm::vec2;
using UVSize= glm::vec2;
using Color = ImColor;
struct WinAlign{
    inline static auto TopLeft()    {return ImVec2{0.0f,0.0f};}
    inline static auto TopMid()     {return ImVec2{0.5f,0.0f};}
    inline static auto TopRight()   {return ImVec2{1.0f,0.0f};}

    inline static auto BotLeft()    {return ImVec2{0.0f,1.0f};}
    inline static auto BotMid()     {return ImVec2{0.5f,1.0f};}
    inline static auto BotRight()   {return ImVec2{1.0f,1.0f};}

    inline static auto MidLeft()    {return ImVec2{0.0f,0.5f};}
    inline static auto MidMid()     {return ImVec2{0.5f,0.5f};}
    inline static auto MidRight()   {return ImVec2{1.0f,0.5f};}

    inline static auto Center() {return MidMid();}
};
inline auto get_facing_str (glm::vec3 facing){
    auto        approx_eq = [](auto x, auto target) {
        const f32 tolerance = 0.1;
        return x >= (target - tolerance) && x <= (target + tolerance);
    };
    std::string facing_str{""};
    if (approx_eq(facing.x, +1.0))
        facing_str += "+X ";
    else if (approx_eq(facing.x, -1.0))
        facing_str += "-X ";
    else
        facing_str += "   ";

    if (approx_eq(facing.y, +1.0))
        facing_str += ",+Y ";
    else if (approx_eq(facing.y, -1.0))
        facing_str += ",-Y ";
    else
        facing_str += ",   ";

    if (approx_eq(facing.z, +1.0))
        facing_str += ",+Z ";
    else if (approx_eq(facing.z, -1.0))
        facing_str += ",-Z ";
    else
        facing_str += ",   ";
    return facing_str;
};
template<typename Item, std::size_t ItemCount>
struct DropDown{
    std::string name;
    std::array<Item,ItemCount> items;
    std::size_t idx{0};
    ImGuiTextFilter filter;
    inline constexpr std::string to_str(Item e){
        using namespace std;
        if constexpr (same_as_nocvref<std::string, Item>){
            return items[idx];
        } else if constexpr (has_to_string_overload<Item>){
            return std::to_string(items[idx]);
        } else if constexpr(has_name_member<Item>){
            return items[idx].name;
        }else if constexpr (is_string_pair<Item>){
            return items[idx].second;
        }else {
            return "INVALID - T HAS NO STRING CONVERSION";
        }
    }
    inline void show(){
        std::string preview = to_str(items.at(idx));
        if (IG::BeginCombo(name.c_str(), preview.c_str(), 0)) {
            if (IG::IsWindowAppearing()) {
                IG::SetKeyboardFocusHere();
                filter.Clear();
            }
            IG::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
            filter.Draw("##Filter", -FLT_MIN);

            for (std::size_t n = 0; n < items.size(); n++) {
                const char* item_cstr = items.at(n).c_str();
                const bool is_selected = (idx == n);
                if (filter.PassFilter(item_cstr)){
                    if (IG::Selectable(item_cstr, is_selected)){
                        idx = n;
                    }
                }
            }
            IG::EndCombo();
        }
    }
};
auto plotRingBuf (const auto& rb,std::size_t max, const std::string key="????", const std::string fmt="p??", bool printValue=false){
    
    if (printValue){
        std::string _fmt = std::string("%-12s: %")+fmt;
        // todo find longest string and make it the field length
        IG::Text(_fmt.c_str(), key.c_str(), rb.avg());
        UI::SameLine();
    }
    std::string id = "##"+key;
    if (!max) max=FLT_MAX;
    IG::PlotLines(id.c_str(), rb.data(), rb.size(), 0, "", FLT_MAX, max);
};
struct WindowConfig{
    std::string title;
    i32 flags;
    std::function<void(WindowConfig&, Simulation*)> draw;

    DebugUI* ui{nullptr};
    ImVec2 work_pos{};
    ImVec2 work_size{};
    UVPos PAD = {0.0f, 0.04f};
    ImVec2 alignment=WinAlign::TopLeft();

    auto toScreen(UVPos v){
        return ScreenPos{v.x,v.y} * work_size;
    }

    inline void setAlign(ImVec2 _alignment){
        alignment = _alignment;
    }
    inline void setAlpha(f32 alpha){
        IG::SetNextWindowBgAlpha(alpha);  // Transparent background
    }
    inline void setFlags(i32 flags){
        this->flags=flags;
    }
    template<typename Fn>
    inline void start_at(bool allowMovement, UVPos pos, Fn&& work){
        setup();
        IG::SetNextWindowViewport(IG::GetMainViewport()->ID);
        UVPos pos_padded{};
        pos_padded.x = alignment.x==0 ? pos.x+PAD.x : pos.x-PAD.x;
        pos_padded.y = alignment.y==0 ? pos.y+PAD.y : pos.y-PAD.y;
        if (allowMovement){
            IG::SetNextWindowPos(toScreen(pos_padded), ImGuiCond_FirstUseEver, alignment);
        }else{
            IG::SetNextWindowPos(toScreen(pos_padded), ImGuiCond_Always, alignment);
        }
        IG::Begin(title.c_str());
        std::invoke(std::forward<Fn>(work));
        IG::End();
    }

    template<typename Fn>
    inline void start_at(UVPos pos, Fn&& work){
        start_at(false,pos,std::forward<Fn>(work));
    }

    inline void setup(){
        const ImGuiViewport* viewport = IG::GetMainViewport();
        work_pos = viewport->WorkPos;  
        work_size = viewport->WorkSize;
    }
    inline void setSize(UVSize size){
        IG::SetNextWindowSize(toScreen(size), ImGuiCond_Always);
    }

    template <typename Fn>
    inline void section(std::string_view name, Fn&& section){
        IG::Separator();
        UI::Text("{}",name);
        std::invoke(std::forward<Fn>(section));
    };
    DropDown<std::string,7> dropdown ={"combo 2", { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG"}};
};
