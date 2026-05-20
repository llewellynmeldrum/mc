cmake_minimum_required(VERSION 3.28)

add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp

    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_stdlib.cpp

    # WARNING: demo window TU is excluded by default
    # imgui_demo.cpp
)

target_include_directories(imgui PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(imgui PUBLIC cxx_std_23)

# disable imgui warnings
target_compile_options(imgui PRIVATE
    -w
)
