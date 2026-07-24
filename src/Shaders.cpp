#include <filesystem>
#include <iostream>

#include "Shaders.hpp"
#include "GlobalDebugLog.hpp"
#include "UnixHelpers.hpp"
#include "glbindingWrapper.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"

#include <string_view>
std::string read_file_contents(std::string const& filename){
    std::ifstream file_stream(filename);
    if (!file_stream.is_open()) {
        LOG_ERROR("Could not open file: '{}'.", filename);
        return "FAILED TO OPEN";
    }
    i64 sz = unix::get_file_size(filename);
    auto file_contents = std::string(sz, '\0');
    file_stream.read(&file_contents[0], sz);
    if (file_contents.empty()){
        LOG_ERROR("Failed to load file contents of '{}'.", filename);
        file_stream.close();
        return "FAILED TO LOAD";
    }
    file_stream.close();
    return file_contents;
}
std::string parse_include_directives(std::string& shader_file_contents){
    auto iss = std::istringstream(shader_file_contents);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(iss, line)){
        lines.push_back(line);
    }

    std::string res; 
    for (const auto& line: lines){
        if (line.contains("#include")){
            auto open_quote = line.find_first_of("\"");
            auto close_quote = line.find_last_of("\"");
            auto included_filename = std::string(
                line.begin() + open_quote + 1,
                line.begin() + close_quote
            );
            LOG_DEBUG("PARSING #INCLUDE FOR ->{}<-",included_filename);
            res.append_range(read_file_contents(included_filename));
        }else{
            res.append_range(line);
        }
        res.append("\n");
    }
    return res;
}

#define _DEBUG
using namespace gl;

using namespace glm;
Shader::Shader(i32 shader_type, const std::string& src_path) 
: ShaderType(shader_type) 
, id(glCreateShader(static_cast<GLenum>(shader_type)))
, src_path(src_path)
{
    load_shader_file(src_path,true);
    if (!compile()) {
        LOG_ERROR("Error compiling shader '{}'.", src_path);
    }
}

void Shader::load_shader(const std::string& file_contents) {
    const char* ptr = file_contents.c_str();
    glShaderSource(id, 1, &(ptr), nullptr);
}
void Shader::load_shader_file(const std::string& filename, bool enable_includes) {
    file_contents = read_file_contents(filename);
    if (enable_includes){
        file_contents = parse_include_directives(file_contents);
    }
    load_shader(file_contents);
}
// given a file containing 0 or many #include "xyz.h" statements, find and paste the file contents
// of the file xyz.

bool Shader::compile() {
    glCompileShader(id);
    if (has_error(static_cast<i32>(GL_COMPILE_STATUS))) {
        LOG_ERROR("{} shader failed to compile:\nin {}:\n{}", shader_type_to_str(ShaderType), src_path,
                  get_info_log());
        LOG_DEBUG("Shader file contents:\n{}",file_contents);
        return false;
    }
    return true;
}
bool Shader::has_error(i32 param_name) {
    i32 success = 0;
    glGetShaderiv(id, static_cast<GLenum>(param_name), &success);
    return !success;
}
std::string Shader::get_info_log() {
    constexpr i64 buf_sz = 512;
    std::string   info_log(buf_sz, '\0');
    glGetShaderInfoLog(id, buf_sz, nullptr, info_log.data());
    return info_log;
}
Shader::~Shader() {
    glDeleteShader(id);
}

void ShaderProgram::load_vtx_and_frag(const std::string& vtx_src, const std::string& frag_src) {
    this->id = glCreateProgram();
    VertexShader   vtx(vtx_src);
    FragmentShader frag(frag_src);
    glAttachShader(id, vtx.id);
    glAttachShader(id, frag.id);
    glLinkProgram(id);
    if (has_error(to_i32(GL_LINK_STATUS))) {
        LOG_ERROR("ShaderProgram failed to link. Log:{}", get_info_log());
        LOG_EXIT(EXIT_FAILURE);
    } else {
        LOG_DEBUG("ShaderProgram succesfully linked!");
    }
}
void ShaderProgram::use() {
    glUseProgram(id);
}
void ShaderProgram::stop() {
    glUseProgram(0);
}
bool ShaderProgram::has_error(i32 param_name) {
    i32 success = 0;
    glGetProgramiv(id, to_glenum(param_name), &success);
    return !success;
}
std::string ShaderProgram::get_info_log() {
    constexpr i64 buf_sz = 512;
    std::string   info_log(buf_sz, '\0');
    glGetProgramInfoLog(id, buf_sz, nullptr, info_log.data());
    return info_log;
}

// Performs a ripgrep search for the uniform in the shaders directory. 
// Obviously not fool-proof, however its nice in catching obvious errors 
// (e.g asking for the location of a shader whose name never appears in the file)
void ShaderProgram::check_uniform(std::string name) {
    std::string count_str = unix::exec(std::format("rg -w '{}' ./shaders -c | wc -l", name));
    auto        count = std::stoi(count_str);
    if (count <= 0) {
        std::println("->{}Error! uniform {}'{}'{}. Was not found. Did you mean any of these?",
                     fmt::fg_red(), fmt::bold(), name, fmt::reset());
        std::println("{}",
                     unix::exec(std::format("rg '' ./shaders | agrep -2 '{}' | tail -3", name)));

        LOG_EXIT(EXIT_FAILURE);
    }
}
void ShaderProgram::setUniform(i32 loc_id, const mat4& val) {
    glUniformMatrix4fv(loc_id, 1, false, glm::value_ptr(val));
}
void ShaderProgram::setUniform(i32 loc_id, const vec2& val) {
    glUniform2fv(loc_id, 1, glm::value_ptr(val));
}
void ShaderProgram::setUniform(i32 loc_id, const f32& val) {
    //        LOG_DEBUG("Deduced unform type as f32 (1f){} = {}", name,val);
    glUniform1f(loc_id, val);
}
void ShaderProgram::setUniform(i32 loc_id, const f64& val) {
    glUniform1d(loc_id, val);
}

void ShaderProgram::setUniform(i32 loc_id, const i32& val) {
    glUniform1i(loc_id, val);
}
void ShaderProgram::setUniform(i32 loc_id, bool val) {
    glUniform1i(loc_id, val);
}
void ShaderProgram::setUniform(i32 loc_id, std::vector<i32> val) {
    glUniform1iv(loc_id, val.size(), val.data());
}

i32 ShaderProgram::getUniformLoc(const std::string& name) {
    i32 location = 0;
    if (uniformLocationsCache.contains(name)) {
        location = uniformLocationsCache.at(name);
    } else {
#ifdef _DEBUG
        check_uniform(name);
#endif
        auto [iter, inserted ]= uniformLocationsCache.insert({ name, glGetUniformLocation(id, name.c_str()) });
        LOG_DEBUG("Cached unform type of '{}'", name);
        location = iter->second;
    }
    if (location == -1) {
        LOG_FATAL("Unable to get location for uniform '{}'.", name);
        LOG_FATAL("{}", get_info_log());
    }
    return location;
}
std::string Shader::shader_type_to_str(i32 shader_type) {
    if (shader_type == GL_VERTEX_SHADER){
        return "Vertex";
    } else if (shader_type == GL_FRAGMENT_SHADER){
        return "Fragment";
    } else{
        return "Unknown shader type.";
    }
}
VertexShader::VertexShader(const std::string& filename) 
: Shader(static_cast<i32>(GL_VERTEX_SHADER), filename) {
}

FragmentShader::FragmentShader(const std::string& filename)
    : Shader(static_cast<i32>(GL_FRAGMENT_SHADER), filename) {
}
