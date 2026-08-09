#pragma once 
#include <libassert/assert.hpp>

#include "FmtStyle.hpp"
#include "LM.hpp"
#include "Types.h"
template<typename T>
struct ProgressBar{

    ProgressBar(std::string_view _name)
        :name(_name)
        ,terminal_rows(libassert::terminal_width(libassert::stdout_fileno))
    {}
    std::string_view name;
    i32 terminal_rows;
    T current_progress{0};
    T expected_total;
    void set_total(i32 new_total){
        expected_total = new_total;
    }
    void print_name()const noexcept{
        std::println("\n\n{}:\n",name);
    }
    constexpr static std::string bar_char = "▄"; 
    void update(i32 new_progress)noexcept{
        current_progress = new_progress;
        print_progress();
    }
    bool is_done()const noexcept{
        return current_progress >= expected_total;
    }
    f32 progress01() const noexcept{
        return std::clamp(static_cast<f32>(current_progress) / expected_total,0.0f,1.0f);
    };
    void print_progress() const noexcept{
        i32 bar_progress = std::round(progress01() * terminal_rows);
        std::print("{}{}",fmt::up_row(),fmt::clear_row());
        std::string bar_str{};
        for (i32 i=0; i<bar_progress; i++) bar_str += bar_char;
        std::println("{}",bar_str);
    };

};
