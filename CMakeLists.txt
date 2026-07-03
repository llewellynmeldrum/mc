cmake_minimum_required(VERSION 3.28)

project(mc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)


option(MC_ENABLE_IWYU "Run include-what-you-use" OFF)


if(MC_ENABLE_IWYU)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
endif()

# -- deps --
add_subdirectory(external/imgui)
add_subdirectory(external/glbinding EXCLUDE_FROM_ALL)
add_subdirectory(external/cpptrace)

# -- source files --
file( GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)


# -- executable --
add_executable(mc ${APP_SRC})

# -- warnings --
target_compile_options(mc PRIVATE
    -Wall
    -Werror
    -Wimplicit-fallthrough
    -Wno-unused
    -Wfatal-errors
    -fdiagnostics-color=always
    -fmax-errors=2
    -ftemplate-backtrace-limit=0
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}=. # gives stack traces a relative path to cwd
)

# --  gcc specific flags --
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(mc PRIVATE
        -fdiagnostics-color=always
        -fdiagnostics-color=always
        -fdiagnostics-show-template-tree
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
    "${CMAKE_CURRENT_SOURCE_DIR}/external/cpptrace"

#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding-aux/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding-aux/include"
)

set_target_properties( mc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# TEMPORARY FOR BENCHMARKING COMPILE TIME
#target_compile_options(mc PRIVATE -ftime-report)



# -- Tracy (benchmarking/profiling)
option(TRACY_ENABLE "" OFF)
option(TRACY_ONLY_LOCALHOST "" ON)
set(TRACY_NO_SYSTEM_TRACING ON CACHE BOOL "" FORCE)
option(TRACY_ON_DEMAND "" OFF)
add_subdirectory(external/tracy)
#target_include_directories( mc       PRIVATE    external/tracy)
target_link_libraries(mc PUBLIC Tracy::TracyClient)


# -- GLFW (window management/platform)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

target_include_directories( mc       PRIVATE    ${GLFW3_INCLUDE_DIRS})

target_include_directories( imgui    PRIVATE    ${GLFW3_INCLUDE_DIRS})
target_compile_options(     imgui    PRIVATE    ${GLFW3_CFLAGS_OTHER})
target_link_libraries(      imgui    PUBLIC     ${GLFW3_LIBRARIES})
target_link_directories(    imgui    PUBLIC     ${GLFW3_LIBRARY_DIRS})

# -- CPPTRACE (stack traces)
target_link_libraries(mc PRIVATE cpptrace::cpptrace)


# -GLBINDING (gl function loader)
target_link_directories(mc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build"
)
if(TARGET glbinding::glbinding)
    target_link_libraries(mc PRIVATE
        glbinding::glbinding
        glbinding::glbinding-aux
        imgui
    )
else()
    message(FATAL_ERROR "Could not find glbinding CMake targets after add_subdirectory(external/glbinding)")
    #    target_link_libraries(mc PRIVATE
    #        glbinding
    #        glbinding-aux
    #        imgui
    #    )
endif()

# -- CURL (only needed for debugging opengl errors, we literally download the help pages to understand errrors)
find_package(CURL REQUIRED)

# -- LibXML2 (also only for debugging opengl errors. Parses the webpages for us)
find_package(LibXml2 REQUIRED)

target_link_libraries(mc PRIVATE
    glbinding
    glbinding-aux
    imgui
    CURL::libcurl
    LibXml2::LibXml2
)

# --- Precompiled headers
target_precompile_headers(mc PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp"
)


# --- Optimization and sanitizers

OPTION(MC_ENABLE_ASAN  "ENABLE ADDRESSSANITIZER" OFF)
OPTION(MC_ENABLE_LSAN  "ENABLE ADDRESSSANITIZER LEAK DETECTION" OFF)
OPTION(MC_ENABLE_TSAN  "ENABLE THREADSANITIZER" OFF)
OPTION(MC_ENABLE_UBSAN "ENABLE UBSAN" OFF)
OPTION(MC_FAST_MATH    "ENABLE -FFAST-MATH" OFF)
OPTION(MC_O1 "ENABLE -O1" OFF)
OPTION(MC_O2 "ENABLE -O2" OFF)
OPTION(MC_O3 "ENABLE -O3" OFF)


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
if(APPLE)
    add_custom_command(TARGET mc POST_BUILD
    COMMAND dsymutil $<TARGET_FILE:mc>
    COMMENT "Generating .dSYM for stack traces")
endif()


#============================================
#               TESTING
#============================================

#add_executable(test_queue
#    tests/src/test_queue.cpp
#    src/Assertion.cpp
#    src/Concurrency.cpp
#)
#
#target_link_libraries(test_queue PRIVATE cpptrace::cpptrace)
#
#target_include_directories(test_queue PRIVATE
#    "${CMAKE_CURRENT_SOURCE_DIR}/include")
#target_include_directories(test_queue PRIVATE
#    "${CMAKE_CURRENT_SOURCE_DIR}/tests/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/KHR"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/OpenGL-Mathematics"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/stb_image"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/fastnoiselite"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/cpptrace/include"
#
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/source/glbinding-aux/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding/include"
#    "${CMAKE_CURRENT_SOURCE_DIR}/external/glbinding/build/source/glbinding-aux/include"
#)
#
#add_test(NAME Queue_tests COMMAND test_queue)
