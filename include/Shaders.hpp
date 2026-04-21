#pragma once 
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "Types.h"
// src/Shaders.cpp


struct Shader{
    Shader(u32 shader_type, const char* src_path);
    u32 id;
    u32 ShaderType;
    std::string src_path;
    static std::string tostr(u32 shader_type);
    bool readSource(const char* filename);
    bool compile();

    bool        has_error(u32 param_name);
    std::string get_info_log();
    ~Shader();
};

struct VertexShader: Shader{
    VertexShader(const char* src);
};
struct FragmentShader: Shader{
    FragmentShader(const char* src);
};

struct ShaderProgram{
    u32 id;
    // compiles and links a vertex and fragment shader from the path of their source files.
    ShaderProgram(const char* vtx_src, const char* frag_src);

    void use();
    void                                 stop();
    std::unordered_map<std::string, i32> uniformLocationsCache;
    void                                 check_uniform(std::string name);
    // `#ifdef _DEBUG`, this function will search with the ./shaders dir with `rg` to see if the `name` requested exists in a shader. 
    // If it doesnt, we crash immediately, providing an error message containing the top 1 `N=3` closest fuzzy results from `agrep`.
    template<typename T> void setUniform(std::string name, T val);

private:
  bool has_error(u32 param_name);
  std::string get_info_log();
};
