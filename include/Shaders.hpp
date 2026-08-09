#pragma once 
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "LM.hpp"
#include "Logger.hpp"
#include "Logger.hpp"
#include "Types.h"
// src/Shaders.cpp


struct ShaderBase{
    ShaderBase()=default;
    ShaderBase(const ShaderBase&) = delete;
    ShaderBase& operator=(const ShaderBase&) = delete;

    ShaderBase(ShaderBase&& rhs)
        :ShaderType(rhs.ShaderType)
        ,id(rhs.id)
        ,src_path(std::move(rhs.src_path))
        ,file_contents(std::move(rhs.file_contents))
    {
        

    }
    ShaderBase(i32 shader_type, const std::string& src_path) { init(shader_type, src_path); }
    void init(i32 shader_type, const std::string& src_path);
    ~ShaderBase(); 
    std::string parse_include_directives(std::string& shader_file_contents);

    i32 ShaderType{};
    u32 id{};
    std::string src_path{};
    std::string file_contents{};
    std::vector<std::string> file_lines{};
    static std::string shader_type_to_str(i32 shader_type);
    bool compile();
    void load_shader(const std::string& file_contents);
    std::string& load_shader_file(const std::string& filename, bool enable_includes);

    bool has_error(i32 param_name);
    std::string get_info_log();
};

struct VertexShader: ShaderBase{
    using ShaderBase::ShaderBase;
    VertexShader(const std::string& filename){init(filename);}
    void init(const std::string& filename);
};
struct FragmentShader: ShaderBase{
    using ShaderBase::ShaderBase;
    FragmentShader(const std::string& filename){init(filename);}
    void init(const std::string& filename);
};

// A wrapper around a FragmentShader and a VertexShader, 
// which provides some helpers for loading/binding them both at the same time.
// Also has helpers for uniforms
struct ShaderProgram{
    ShaderProgram()= default;
    ~ShaderProgram()=default;
    VertexShader   vtx;
    FragmentShader frag;
    u32 id;

    // compiles and links a vertex and fragment shader from the path of their source files.
    void load_vtx_and_frag(const std::string&  vtx_src, const std::string&  frag_src);

    void use();
    void stop();
    bool check_shader_contains_uniform_name(std::string name);
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
        LOG_WARN("Setting uniforms by name is costly, prefer storing the uniform location.");
        LOG_WARN("(Setting uniform '{}' by name.)",name);
        setUniform(getUniformLoc(name),val);
    }

    void setUniform(i32 id, const i32& val);
    void setUniform(i32 id, const glm::mat4& val);
    void setUniform(i32 id, const glm::vec2& val);
    void setUniform(i32 loc_id, const glm::vec3& val);
    void setUniform(i32 id, const f32& val);
    void setUniform(i32 id, const f64& val);
    void setUniform(i32 id, std::vector<i32> val);
    void setUniform(i32 loc_id, bool const& val) ;

private:
  bool has_error(i32 param_name);
  std::string get_info_log();
};
