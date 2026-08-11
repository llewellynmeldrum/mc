#include <algorithm>
#include <filesystem>
#include <format>
#include <ios>
#include <iostream>
#include <print>
#include <regex>
#include <string>
#include <string_view>

#include "Shaders.hpp"
#include "FmtStyle.hpp"
#include "GlobalDebugLog.hpp"
#include "UnixHelpers.hpp"
#include "glbindingWrapper.hpp"
#include "Breakpoints.hpp"
#include "Logger.hpp"
#include "cpp23_ranges.hpp"
#include "libassert/assert.hpp"

using namespace gl;

std::string parse_bitfield_definition(std::string const& line){
    ASSERT(line.starts_with("BITFIELD_MEMBER("), "", line);
    std::string res{};
    auto start = line.find_first_of("(");
    auto stop = line.find_last_of(")");
    auto included_filename = std::string(
        line.begin() + start + 1,
        line.begin() + stop
    );
    std::smatch matches;
    // BITFIELD_MEMBER(sunlight_intensity, 0,  4L) => v/15.0
    std::regex pattern(R"(^BITFIELD_MEMBER\s*\(\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*,\s*(-?\d+)\s*,\s*(-?\d+)[uUlL]*\s*,\s*(.*)\)$)");
    if (std::regex_match(line, matches, pattern)) {
        // matches[0] is the entire string
        std::string name = matches[1].str();
        std::ranges::transform(name, name.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        int offset = std::stoi(matches[2].str());
        int len = std::stoi(matches[3].str());
        std::string convert = matches[4].str();
        res.append(std::format(
            "#define {}_OFFSET    ({}u)\n", name, offset
        ));
        res.append(std::format(
            "#define {}_LEN       ({}u)\n", name, len
        ));
        res.append(std::format(
            "#define {}_MASK      (MASK({}_OFFSET,{}_LEN))\n", name, name, name
        ));
        res.append(std::format(
            "#define GET_{}(packed)     ( ((packed) & ({}_MASK))>> ({}_OFFSET))\n",name,name,name
        ));
        res.append(std::format(
            "#define CONVERT_{}(v)     ({})\n",name,convert
        ));
        res.append(std::format(
            "#define UNPACK_{}(packed)     ( CONVERT_{}( (GET_{}(packed)) ) )\n",name,name,name
        ));
    } else {
        std::println(stderr,"'{}'",line);
        assert(false);
    }
    return res;
}
std::string parse_bitfield_definition_file(std::string const& file_contents){
    std::string res{};

    std::string line{};
    auto iss = std::istringstream(file_contents);
    while (std::getline(iss, line)){
        if ( line.starts_with("//") 
            || line.starts_with("\n") 
            || line.starts_with("\r\n")
            || line.empty()
        ) continue; // comment, line is ignored

        res.append_range(parse_bitfield_definition(line));
    }

    return res;
}
std::string read_file_contents(std::string const& filename){
    std::ifstream file_stream(filename);
    assert(file_stream.is_open());
    i64 sz = unix::get_file_size(filename);
    auto file_contents = std::string(sz, '\0');
    file_stream.read(&file_contents[0], sz);
    assert(!file_contents.empty());
    file_stream.close();
    return file_contents;
}
std::string read_shader_include(std::string const& filename){
    std::string file_contents = read_file_contents(filename);
    if (filename.ends_with(".def")){
        LOG_DEBUG("PARSING BITFIELD DEFINITION #INCLUDE FOR ->{}<-",filename);
        return parse_bitfield_definition_file(file_contents);
    }else{
        LOG_DEBUG("PARSING REGULAR #INCLUDE FOR ->{}<-",filename);
        return file_contents;
    }
}
std::string trim_all_ws(std::string const & str) {
    std::string copy = str;
    copy.erase(
        std::remove_if(copy.begin(), copy.end(), [](unsigned char ch) { //NOLINT
            return std::isspace(ch);
        }),
        copy.end()
    );
    return copy;
}


std::string ShaderBase::parse_include_directives(std::string& shader_file_contents){
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
            res.append_range(read_shader_include(included_filename));
        }else{
            res.append_range(line);
        }
        res.append("\n");
    }
    return res;
}


using namespace glm;
void ShaderBase::init(i32 _shader_type, const std::string& _src_path) {
    ShaderType = _shader_type;
    src_path = _src_path;
    id = glCreateShader(static_cast<GLenum>(_shader_type));
    auto& contents = load_shader_file(src_path,true);
    load_shader(contents);
    if (!compile()) {
        LOG_ERROR("Error compiling shader '{}'.", src_path);
    }
}

void ShaderBase::load_shader(const std::string& file_contents) {
    const char* ptr = file_contents.c_str();
    glShaderSource(id, 1, &(ptr), nullptr);
}
std::string& ShaderBase::load_shader_file(const std::string& filename, bool enable_includes) {
    file_contents = read_file_contents(filename);
    if (enable_includes){
        file_contents = parse_include_directives(file_contents);
    }
    return file_contents;
}
// given a file containing 0 or many #include "xyz.h" statements, find and paste the file contents
// of the file xyz.

auto split_lines(auto&& str){
    return str | ranges::views::split('\n') | ranges::to<std::vector<std::string>>();
}
bool ShaderBase::compile() {
    glCompileShader(id);
    if (has_error(static_cast<i32>(GL_COMPILE_STATUS))) {
        file_lines = split_lines(file_contents);
        LOG_DEBUG("ERROR IN SHADER, file contents:\n{}",file_contents);
        LOG_ERROR("{} shader failed to compile:\nin {}:", shader_type_to_str(ShaderType), src_path);
        auto log_lines = split_lines(get_info_log());
        for (const auto& line: log_lines){
            i32 first_colon = line.find_first_of(":");
            i32 second_colon = line.find_first_of(":",first_colon+1);
            i32 third_colon = line.find_first_of(":",second_colon+1);
            auto err_msg = line.substr(third_colon+1);
            auto err_line_no = std::stoi(std::string(line.begin()+second_colon+1, line.begin() + third_colon));
            constexpr static i32 ctx_radius = 2;
            auto min_i  =std::max(0,err_line_no-ctx_radius);
            auto max_i = std::min(file_lines.size(),(size_t)err_line_no+ctx_radius);
            std::println("=====================");
            for (i32 i = min_i; i<max_i; i++){
                auto line_str = std::format("{:>4} : {}",std::format("{}",i),file_lines.at(i-1));
                if (i == err_line_no){
                    std::println(
                        "{}", 
                        fmt::styled_fg(
                            fmt::fg_red(),
                            line_str
                        )
                    );
                }else{
                    std::println( "{}", line_str);
                }
            }
            std::println("=====================");
            std::println(
                "{}",
                fmt::styled_fg(
                    fmt::bold_red(),
                    std::format("On line {}: {}\n",err_line_no, err_msg) 
                )
            );
        }
        return false;
    }
    return true;
}
bool ShaderBase::has_error(i32 param_name) {
    i32 success = 0;
    glGetShaderiv(id, static_cast<GLenum>(param_name), &success);
    return !success;
}
std::string ShaderBase::get_info_log() {
    constexpr i64 buf_sz = 512;
    std::string   info_log(buf_sz, '\0');
    glGetShaderInfoLog(id, buf_sz, nullptr, info_log.data());
    return info_log;
}
ShaderBase::~ShaderBase() {
    glDeleteShader(id);
}

void ShaderProgram::load_vtx_and_frag(const std::string& vtx_src, const std::string& frag_src) {
    this->id = glCreateProgram();
    vtx.init(vtx_src);
    frag.init(frag_src);
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
bool ShaderProgram::check_shader_contains_uniform_name(std::string name) {
    std::string ripgrep_command = std::format("rg -w '{}' ./{} ./{} -c | wc -l", name, vtx.src_path,frag.src_path);
    std::string count_str = unix::exec(ripgrep_command);
    auto        count = std::stoi(count_str);
    if (count <= 0) {
        std::println("->{}Error! uniform {}'{}'{}. never occurs in the vertex or frag shaders {}/{}.. Did you mean any of these?",
                     fmt::fg_red(), fmt::bold(), name, fmt::reset(),vtx.src_path,frag.src_path);
        std::println("{}",
                     unix::exec(std::format("rg '' ./shaders | agrep -2 '{}' | tail -3", name)));

        return false;
        LOG_EXIT(EXIT_FAILURE);
    }
    return true;
}
void ShaderProgram::setUniform(i32 loc_id, const mat4& val) {
    glUniformMatrix4fv(loc_id, 1, false, glm::value_ptr(val));
}
void ShaderProgram::setUniform(i32 loc_id, const vec2& val) {
    glUniform2fv(loc_id, 1, glm::value_ptr(val));
}
void ShaderProgram::setUniform(i32 loc_id, const glm::vec3& val) {
    glUniform3fv(loc_id, 1, glm::value_ptr(val));
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
void ShaderProgram::setUniform(i32 loc_id, bool const& val) {
    glUniform1i(loc_id, static_cast<int>(val));
}
void ShaderProgram::setUniform(i32 loc_id, std::vector<i32> val) {
    glUniform1iv(loc_id, val.size(), val.data());
}

i32 ShaderProgram::getUniformLoc(const std::string& name) {
    i32 location = 0;
    if (uniformLocationsCache.contains(name)) {
        location = uniformLocationsCache.at(name);
    } else {
        check_shader_contains_uniform_name(name);
        auto [iter, inserted ] = uniformLocationsCache.try_emplace(name, glGetUniformLocation(id, name.c_str()));
//        LOG_DEBUG("Cached unform type of '{}'", name);
        location = iter->second;
    }
    if (location == -1) {
        LOG_FATAL("Unable to get location for uniform '{}'. One of the following is probably true:  \
                  \n-> The uniform is likely inactive (i.e not used by the shader).                 \
                  \n-> name does not correspond to an active uniform variable in program            \
                  \n-> name starts with the reserved prefix 'gl_'                                   \
                  \n-> name is associated with an atomic counter or a named uniform block\
                  \n====\
                  \nglGetProgramInfoLog():{}"
                  ,name, get_info_log());
    }
    return location;
}
std::string ShaderBase::shader_type_to_str(i32 shader_type) {
    if (shader_type == GL_VERTEX_SHADER){
        return "Vertex";
    } else if (shader_type == GL_FRAGMENT_SHADER){
        return "Fragment";
    } else{
        return "Unknown shader type.";
    }
}
void VertexShader::init(const std::string& filename) {
    ShaderBase::init(static_cast<i32>(GL_VERTEX_SHADER), filename);
}

void FragmentShader::init(const std::string& filename){
    ShaderBase::init(static_cast<i32>(GL_FRAGMENT_SHADER), filename);
}
