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
    Shader(i32 shader_type, const std::string& src_path);
    ~Shader(); // custom

    i32 ShaderType;
    u32 id;
    std::string src_path;
    std::string file_contents;
    static std::string shader_type_to_str(i32 shader_type);
    bool compile();
    void load_shader(const std::string& file_contents);
    void load_shader_file(const std::string& filename, bool enable_includes);

    bool has_error(i32 param_name);
    std::string get_info_log();
};

struct VertexShader: Shader{
    VertexShader(const std::string& filename);

};
struct FragmentShader: Shader{
    FragmentShader(const std::string& filename);
};

// A wrapper around a FragmentShader and a VertexShader, 
// which provides some helpers for loading/binding them both at the same time.
// Also has helpers for uniforms
struct ShaderProgram{
    ShaderProgram()= default;
    ~ShaderProgram()=default;
    u32 id;

    // compiles and links a vertex and fragment shader from the path of their source files.
    void load_vtx_and_frag(const std::string&  vtx_src, const std::string&  frag_src);

    void use();
    void stop();
    void check_uniform(std::string name);
    std::unordered_map<std::string, i32> uniformLocationsCache;
    i32 getUniformLoc(const std::string& name);
    // `#ifdef _DEBUG`, this function will search with the ./shaders dir with `rg` to see if the `name` requested exists in a shader. 
    // If it doesnt, we crash immediately, providing an error message containing the top 1 `N=3` closest fuzzy results from `agrep`.
    template<typename T> 
    void setUniform(std::string& name, const T& val){
        LOG_FATAL("Failed to deduce unform type of '{} {}'.",pretty_type_name<T>(), name);
        LOG_EXIT(EXIT_FAILURE);
    }
    template<typename T>
    void setUniform(const std::string& name, const T& val){
        setUniform(getUniformLoc(name),val);
    }

    void setUniform(i32 id, const i32& val);
    void setUniform(i32 id, const glm::mat4& val);
    void setUniform(i32 id, const glm::vec2& val);
    void setUniform(i32 id, const f32& val);
    void setUniform(i32 id, const f64& val);
    void setUniform(i32 id, std::vector<i32> val);
    void setUniform(i32 id, bool val);

private:
  bool has_error(i32 param_name);
  std::string get_info_log();
};
