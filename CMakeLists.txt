cmake_minimum_required(VERSION 3.28)

project(mc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# this might break shit because it assumes libc++ instead of libstdc++
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use" CACHE STRING "IWYU" FORCE)

# -- source files --
file( GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)


file( GLOB IMGUI_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/*.cpp"
)

add_executable(mc
    ${APP_SRC}
    ${IMGUI_SRC}
)

set_target_properties( mc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)



# -- warnings --

target_compile_options(mc PRIVATE
    -Wall
    -Werror
    -Wimplicit-fallthrough
    -Wno-unused
    -fdiagnostics-color=always
    -fmax-errors=5
    -ftemplate-backtrace-limit=5
    -fdiagnostics-show-template-tree
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(mc PRIVATE
        -fdiagnostics-color=always
        -fdiagnostics-color=always
        -fdiagnostics-show-template-tree
#        -fdiagnostics-show-caret
    )
endif()

# -- includes --


target_include_directories(mc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"

    "${CMAKE_CURRENT_SOURCE_DIR}/external/KHR"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/OpenGL-Mathematics"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/fastnoiselite"

    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding-aux/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding-aux/include"
)


# -- DEPS --

# -GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

target_include_directories(mc PRIVATE ${GLFW3_INCLUDE_DIRS})
target_compile_options(mc PRIVATE ${GLFW3_CFLAGS_OTHER})
target_link_libraries(mc PRIVATE ${GLFW3_LIBRARIES})
target_link_directories(mc PRIVATE ${GLFW3_LIBRARY_DIRS})



# -GLBINDING
target_link_directories(mc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build"
)
target_link_libraries(mc PRIVATE
    glbinding
    glbinding-aux
)
set_target_properties(mc PROPERTIES
    BUILD_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build"
)

# --- Precompiled headers

target_precompile_headers(mc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)


# --- Optimization and sanitizers

option(MC_ENABLE_ASAN  "Enable AddressSanitizer" OFF)
option(MC_ENABLE_LSAN  "Enable AddressSanitizer leak detection" OFF)
option(MC_ENABLE_TSAN  "Enable ThreadSanitizer" OFF)
option(MC_ENABLE_UBSAN "Enable UBSan" OFF)
option(MC_FAST_MATH    "Enable -ffast-math" OFF)
option(MC_O1 "Enable -O1" OFF)
option(MC_O2 "Enable -O2" OFF)
option(MC_O3 "Enable -O3" OFF)

if(MC_O1)
    target_compile_options(mc PRIVATE -O1)
endif()

if(MC_O2)
    target_compile_options(mc PRIVATE -O2)
endif()

if(MC_O3)
    target_compile_options(mc PRIVATE -O3)
endif()

if(MC_FAST_MATH)
    target_compile_options(mc PRIVATE -ffast-math)
endif()

if(MC_ENABLE_ASAN OR MC_ENABLE_LSAN)
    target_compile_options(mc PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
    target_link_options(mc PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
endif()

if(MC_ENABLE_TSAN)
    target_compile_options(mc PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
    target_link_options(mc PRIVATE -fsanitize=thread -fno-omit-frame-pointer -g)
endif()

if(MC_ENABLE_UBSAN)
    target_compile_options(mc PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -g)
    target_link_options(mc PRIVATE -fsanitize=undefined -fno-omit-frame-pointer -g)
endif()

# helper targets

add_custom_target(run
    COMMAND mc
    DEPENDS mc
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(debug
    COMMAND lldb -o run -- "$<TARGET_FILE:mc>"
    DEPENDS mc
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

