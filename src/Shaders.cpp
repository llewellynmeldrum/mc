#include "Shaders.hpp"
#include "UnixHelpers.hpp"
#include "glbinding/gl/gl.h"
#include "glbinding/glbinding.h"
using namespace gl;

Shader::Shader(u32 shader_type, const char* src_path) : src_path(src_path) {
    this->ShaderType = shader_type;
    this->id = glCreateShader(static_cast<GLenum>(shader_type));
    if (!readSource(src_path)){
        LOG_ERROR("Error reading shader '{}'.",src_path);
        LOG_EXIT(EXIT_FAILURE);
    }
    if (!compile()){
        LOG_ERROR("Error compiling shader '{}'.",src_path);
    }
}
std::string Shader::tostr(u32 shader_type){
    if (shader_type == GL_VERTEX_SHADER)
        return "Vertex";
    else if (shader_type== GL_FRAGMENT_SHADER)  return "Fragment";
    else                                        return "Unknown shader type.";
}
bool Shader::readSource(const char* filename){
        std::string src_buf;
        std::ifstream file(filename);
        if (!file.is_open()){
            LOG_ERROR("Could not open file '{}'.", filename);
            return false;
        }
        u64 sz = unix::get_file_size(filename);
        src_buf = std::string(sz, '\0'); 
        file.read(&src_buf[0], sz);
        file.close();
        const char* data = src_buf.c_str();
        glShaderSource(id, 1, &data, nullptr);
        return true;
}
bool Shader::compile(){
    glCompileShader(id);
    if (has_error(static_cast<u32>(GL_COMPILE_STATUS))){
        LOG_ERROR("{} shader failed to compile:\nin {}:\n{}",tostr(ShaderType), src_path, get_info_log());
        return false;
    }
    return true;
}
bool Shader::has_error(u32 param_name) {
    i32 success = 0;
    glGetShaderiv(id, static_cast<GLenum>(param_name), &success);
    return !success;
}
std::string Shader::get_info_log() {
    constexpr u64 buf_sz = 512;
    std::string   info_log(buf_sz, '\0');
    glGetShaderInfoLog(id, buf_sz, nullptr, info_log.data());
    return info_log;
}
Shader::~Shader() {
    glDeleteShader(id);
}
VertexShader::VertexShader(const char* src) : Shader(static_cast<u32>(GL_VERTEX_SHADER), src) {
}

FragmentShader::FragmentShader(const char* src) : Shader(static_cast<u32>(GL_FRAGMENT_SHADER), src) {
}

ShaderProgram::ShaderProgram(const char* vtx_src, const char* frag_src) {
    this->id = glCreateProgram();
    VertexShader   vtx(vtx_src);
    FragmentShader frag(frag_src);
    glAttachShader(id, vtx.id);
    glAttachShader(id, frag.id);
    glLinkProgram(id);
    if (has_error(GL_LINK_STATUS)) {
        LOG_ERROR("ShaderProgram failed to link. Log:{}", get_info_log());
        LOG_EXIT(EXIT_FAILURE);
    }
}
void ShaderProgram::use() {
    glUseProgram(id);
}
void ShaderProgram::stop() {
    glUseProgram(0);
}
bool ShaderProgram::has_error(GLenum param_name) {
    i32 success = 0;
    glGetProgramiv(id, param_name, &success);
    return !success;
}
std::string ShaderProgram::get_info_log() {
    constexpr u64 buf_sz = 512;
    std::string   info_log(buf_sz, '\0');
    glGetProgramInfoLog(id, buf_sz, nullptr, info_log.data());
    return info_log;
}

void ShaderProgram::check_uniform(std::string name) {
    std::string count_str = unix::exec(std::format("rg -w '{}' ./shaders -c | wc -l", name));
    auto        count = std::stoi(count_str);
    if (count <= 0) {
        std::println("->{}Error! uniform {}'{}'{}. Was not found. Did you mean any of these?",
                     fmt::red, fmt::bold, name, fmt::clear);
        std::println("{}",
                     unix::exec(std::format("rg '' ./shaders | agrep -2 '{}' | tail -3", name)));

        LOG_EXIT(EXIT_FAILURE);
    }
}
template<typename T> void ShaderProgram::setUniform(std::string name, T val){
    i32 location = 0;
    if (uniformLocationsCache.contains(name)){
        location = uniformLocationsCache.at(name); 
    } else{
        #ifdef _DEBUG
        check_uniform(name);
        #endif 
        auto pair = uniformLocationsCache.insert({name,glGetUniformLocation(id,name.c_str())});
//            LOG_DEBUG("Cached unform type of '{} {}'.",pretty_type_name<T>(), name);
        location = pair.first->second;
    }
    if constexpr(std::same_as<T,mat4>){
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(val));
    }else if constexpr(std::same_as<T,vec2>){
        glUniform2fv(location,1,glm::value_ptr(val));
    }else if constexpr(std::same_as<T,f32>){
        LOG_DEBUG("Deduced unform type as f32 (1f){} = {}", name,val);
        glUniform1f(location,val);
    }else if constexpr(std::same_as<T,f64>){
        glUniform1d(location,val);
    }else if constexpr(std::same_as<T,i32>){
        glUniform1i(location,val);
    }else {
        LOG_FATAL("Failed to deduce unform type of '{} {}'.",pretty_type_name<T>(), name);
        LOG_EXIT(EXIT_FAILURE);
    }
}
