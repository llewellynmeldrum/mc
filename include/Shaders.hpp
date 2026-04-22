#pragma once 
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "Logger.hpp"
#include "Logger.hpp"
#include "Types.h"
// src/Shaders.cpp


struct Shader{
    Shader(u32 shader_type, const char* src_path);
    ~Shader(); // custom

    Shader()=delete;
    u32 id;
    u32 ShaderType;
    std::string src_path;
    static std::string tostr(u32 shader_type);
    bool readSource(const char* filename);
    bool compile();

    bool        has_error(u32 param_name);
    std::string get_info_log();
};

struct VertexShader: Shader{
    VertexShader(const char* src);
    ~VertexShader()=default;

    VertexShader()=delete;

};
struct FragmentShader: Shader{
    FragmentShader(const char* src);
    ~FragmentShader()=default;

    FragmentShader()=delete;

};

struct ShaderProgram{
    ShaderProgram()= default;
    ~ShaderProgram()=default;
    u32 id;

    // compiles and links a vertex and fragment shader from the path of their source files.
    void setupShaderProgram(const char* vtx_src, const char* frag_src);

    u32 getUniformLoc(std::string name);
    void use();
    void                                 stop();
    std::unordered_map<std::string, i32> uniformLocationsCache;
    void                                 check_uniform(std::string name);
    // `#ifdef _DEBUG`, this function will search with the ./shaders dir with `rg` to see if the `name` requested exists in a shader. 
    // If it doesnt, we crash immediately, providing an error message containing the top 1 `N=3` closest fuzzy results from `agrep`.
    template<typename T> 
    void setUniform(std::string& name, const T& val){
        LOG_FATAL("Failed to deduce unform type of '{} {}'.",pretty_type_name<T>(), name);
        LOG_EXIT(EXIT_FAILURE);
    }
    void setUniform(std::string name, const i32& val);
    void setUniform(std::string name, const mat4& val);
    void setUniform(std::string name, const vec2& val);
    void setUniform(std::string name, const f32& val);
    void setUniform(std::string name, const f64& val);

private:
  bool has_error(u32 param_name);
  std::string get_info_log();
};
