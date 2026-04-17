
#include <iostream>
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding-aux/debug.h>
#include <glbinding-aux/Meta.h>
#include <glbinding/FunctionCall.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/CallbackMask.h>


#include "Logger.hpp"
#include "Types.h"
#include "UnixHelpers.hpp"
#include "glHelpers.hpp"

#include "UnixHelpers.hpp"

#include <cfloat>
#include <cassert>
#include <format>
#include <limits>
#include <concepts>
#include <print>
#include <string>
#include <unordered_map>
#include <fstream>

#include "stb_image.hpp"

#include "glmWrapper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
using namespace gl;
#define _DEBUG


u64 program_epoch_ns;
struct Context {
    GLFWwindow* win{};
        struct Timer{
        u64 framecount = 0;
        f64 dt_ms= 0.0f;
        f64 elapsed_ms = 0.0;
        void init() { 
            glfwSetTime(0.0);
        }
        void update() {
            f64 prev_elapsed_ms = elapsed_ms;
            elapsed_ms = glfwGetTime()/1000.0;
            dt_ms = elapsed_ms-prev_elapsed_ms;
        }
    }time;
    static constexpr i32 win_x = 0;
    static constexpr i32 win_y = 0;
    static constexpr i32 win_w = 900; // half my screen width
    static constexpr i32 win_h = 1169;
    bool wireframe = false;
};
Context ctx;

static void glfw_ErrorCallback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
}
static void handleInputs(GLFWwindow* win){
    if (glfwGetKey(win,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(win, true);
    }
}


static const std::array<f32, 3 * 8> triangle_verts ={
    //Vertex coords     //tex coords    //base colors 
     0.0,  0.7, 0.0,    0.5,  1.0,      1.0,  0.0, 0.0,                  // top middle
     0.7,  0.0, 0.0,    1.0,  0.0,      0.0,  1.0, 0.0,                  // bot right
    -0.7,  0.0, 0.0,    0.0,  0.0,      0.0,  0.0, 1.0,                  // bot left
};
static const std::array<u32, 3> triangle_offsets{ 
	 0 , 	// idx::0 slot occupied by vtx::0
	 1 , 	// idx::1 slot occupied by vtx::1
	 2 , 	// idx::2 slot occupied by vtx::2 
};




template <typename C>
concept ContiguousContainer = std::ranges::contiguous_range<C>;

struct Shader{
    u32 id;
    GLenum ShaderType;
    std::string src_path;
    Shader(GLenum shader_type, const char* src_path): src_path(src_path){
        this->ShaderType = shader_type;
        this->id = glCreateShader(shader_type);
        if (!readSource(src_path)){
            LOG_ERROR("Error reading shader '{}'.",src_path);
            LOG_EXIT(EXIT_FAILURE);
        }
        if (!compile()){
            LOG_ERROR("Error compiling shader '{}'.",src_path);
        }
    }
    bool readSource(const char* filename){
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


    bool compile(){
        glCompileShader(id);
        if (has_error(GL_COMPILE_STATUS)){
            LOG_ERROR("{} shader failed to compile:\nin {}:\n{}",tostr(ShaderType), src_path, get_info_log());
            return false;
        }
        return true;
    }
    static std::string tostr(GLenum shader_type){
        if (shader_type== GL_VERTEX_SHADER)         return "Vertex";
        else if (shader_type== GL_FRAGMENT_SHADER)  return "Fragment";
        else                                        return "Unknown shader type.";
    }

    bool has_error(GLenum param_name){
        i32 success = 0;
        glGetShaderiv(id, param_name, &success);
        return !success;
    }
    std::string get_info_log(){
        constexpr u64 buf_sz = 512;
        std::string info_log(buf_sz,'\0');
        glGetShaderInfoLog(id, buf_sz, nullptr, info_log.data());
        return info_log;
    }
    ~Shader(){
        glDeleteShader(id);
    }
};

struct VertexShader: Shader{
    VertexShader(const char* src): Shader(GL_VERTEX_SHADER, src){}
};
struct FragmentShader: Shader{
    FragmentShader(const char* src): Shader(GL_FRAGMENT_SHADER, src){}
};

struct ShaderProgram{
    u32 id;
    ShaderProgram(const char* vtx_src, const char* frag_src) {
        this->id = glCreateProgram();
        VertexShader vtx(vtx_src);
        FragmentShader frag(frag_src);
        glAttachShader(id, vtx.id);
        glAttachShader(id, frag.id);
        glLinkProgram(id);
        if (has_error(GL_LINK_STATUS)){
            LOG_ERROR("ShaderProgram failed to link. Log:{}", get_info_log());
            LOG_EXIT(EXIT_FAILURE);
        }
    }

    void use(){
        glUseProgram(id);
    }
    void stop(){
        glUseProgram(0);
    }
    std::unordered_map<std::string, i32> uniformLocationsCache;
    void check_uniform(std::string name){
        std::string count_str = unix::exec(std::format("rg -w '{}' ./shaders -c | wc -l",name));
        auto count = std::stoi(count_str);
        if (count<=0){
            std::println("->{}Error! uniform {}'{}'{}. Was not found. Did you mean any of these?",
                       fmt::red, fmt::bold, name, fmt::clear);
            std::println("{}",unix::exec(std::format("rg '' ./shaders | agrep -2 '{}' | tail -3",name)));

            LOG_EXIT(EXIT_FAILURE);
        }
    }
    template<typename T>
    void setUniform(std::string name, T val){
        i32 location = 0;
        if (uniformLocationsCache.contains(name)){
            location = uniformLocationsCache.at(name); 
        } else{
            #ifdef _DEBUG
            check_uniform(name);
            #endif 
            auto pair =uniformLocationsCache.insert({name,glGetUniformLocation(id,name.c_str())});
            location = pair.first->second;
        }
        if constexpr(std::same_as<T,mat4>){
            glUniformMatrix4fv(location,1,false, value_ptr(val));
        }else if constexpr(std::same_as<T,f32>){
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

private:
    bool has_error(GLenum param_name){
        i32 success = 0;
        glGetProgramiv(id, param_name, &success);
        return !success;
    }


    std::string get_info_log(){
        constexpr u64 buf_sz = 512;
        std::string info_log(buf_sz,'\0');
        glGetProgramInfoLog(id, buf_sz, nullptr, info_log.data());
        return info_log;
    }
};

static u32 currentlyBoundBuffer = 0;
struct ElementBuffer{
    u32 id;
    u32 size; // size in bytes
    // perhaps implement some sort of cast_buffer<T target>()
    static const GLenum buffer_type=GL_ELEMENT_ARRAY_BUFFER;
    u32 location={};

    ElementBuffer(){ glGenBuffers(1, &this->id); }

    void bind(){
		currentlyBoundBuffer=id;
        glBindBuffer(buffer_type, id);
    }

    template <ContiguousContainer C>
    void load(C c, GLenum usage_type){
        bind();
        using T = C::value_type;
        static_assert(std::same_as<T,u32>);

        glBufferData(buffer_type, sizeof(c), c.data(), usage_type);
    }
};
struct VertexBuffer{
    u32 id;
    u32 size; // size in bytes
    u32 location={};
   static constexpr GLenum buffer_type = GL_ARRAY_BUFFER;

    VertexBuffer(){
        glGenBuffers(1, &this->id);
    }

    void bind(){
		currentlyBoundBuffer=id;
        glBindBuffer(buffer_type, id);
    }

    template <ContiguousContainer C>
    void load(C c, GLenum usage_type, u32 offset=0){
        bind();
        glBufferData(buffer_type, sizeof(c), c.data()+offset, usage_type);
    }
};

struct AttributeType{
    GLenum v;
};

template <class VT>
struct VertexAttribute {
    u32 location;
    u32 count;
    AttributeType type;
    bool normalized;
    u32 stride;
    u32 offset=0;
};
struct VertexArray{
    u32 id;
    i32 buffer_cols;
    VertexArray(){
        glGenVertexArrays(1, &id);  
    }

    void bind(){
		currentlyBoundBuffer=id;
        glBindVertexArray(id);
    }

    void unbind(){
        glBindVertexArray(0);
    } 

    template <class VT>
    void set_vtx_attributes(u32 location, i32 count, u64 offset){
        // assumed packed non normalized vertex data
        void* offset_ptr = (void*)(offset*sizeof(VT));
        i32 stride  = buffer_cols * sizeof(VT);
        GLenum type = gl_type<VT>();
        glVertexAttribPointer(location, count, type, false,stride, offset_ptr);
        glEnableVertexAttribArray(location);
    }
    // draws *num* vertices, chosen via whichever EBO is bound to this VAO
    void drawElements(u32 num, GLenum elem_t){
        glDrawElements(elem_t, num, GL_UNSIGNED_INT, nullptr);
    }
    void drawArrays(u32 count, GLenum elem_t, u32 offset=0){
        glDrawArrays(elem_t, offset, count);
    }

};

static u64 texture_count = 0;
struct Texture2D{
    u32 id;
    u32 idx;
    GLint pxwidth, pxheight, nchannels;
    Texture2D(const char* tex_dir, GLenum image_fmt=GL_RGB,vec4 border_color = {1,0,1,1}){
        mat4::length();
        // stbi_load returns row major 2d pixels array. 
        u8* tex_pixels = stbi_load(tex_dir, &pxwidth, &pxheight, &nchannels, 0);
        if (!tex_pixels){
            LOG_ERROR("Failed to load texture file '{}'.",tex_dir);
            return;
        }
        Texture2D::init();
        Texture2D::bind(); 
            Texture2D::setMinifyMode(GL_LINEAR_MIPMAP_NEAREST);
            Texture2D::setMagnifyMode(GL_NEAREST);
            Texture2D::setWrapMode(GL_CLAMP_TO_BORDER);
            Texture2D::setBorderColor(border_color);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pxwidth, pxheight,
                         0, image_fmt, GL_UNSIGNED_BYTE, (const void*)tex_pixels);
            glGenerateMipmap(GL_TEXTURE_2D);
        Texture2D::unbind();

        stbi_image_free(tex_pixels);
        idx = texture_count++;
    }
    inline void bind(){ 
        glActiveTexture(GL_TEXTURE0+idx);
        glBindTexture(GL_TEXTURE_2D, id); 
    }
private:
    inline void init(){ glGenTextures(1, &id); }
    inline void unbind(){ glBindTexture(GL_TEXTURE_2D, 0); }

    static inline void setBorderColor(vec4 color){
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, data(color));

    }
    static inline void setMinifyMode(GLenum mode){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    }
    static inline void setMagnifyMode(GLenum mode){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    }
    static inline void setWrapMode(GLenum mode){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    }


};
#ifdef TESTING
#include "UnitTests.hpp"
#endif

vec3 mouse_pos_to_NDC(){
    double x,y;
    glfwGetCursorPos(ctx.win, &x,&y);

    x/=ctx.win_w; // map to [0,1]
    y/=ctx.win_h;
    // flip the y
    y= 1-y;
    x= (x * 2) -1; // map to [-1,1]
    y= (y*2) -1; // map to [-1,1]

    return {x,y,1.0};
}
int main(int argc, char** argv) {
#ifdef TESTING
    RUN_TESTS();
}
#else
    stbi_set_flip_vertically_on_load(true);  
    program_epoch_ns = get_current_ns();
    glfwSetErrorCallback(glfw_ErrorCallback);

    // init glfw
    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW.");
        LOG_EXIT(EXIT_FAILURE);
    } 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // required for opengl 3.2+
    #endif

    // init glfw window
    GLFWwindow* win = glfwCreateWindow(ctx.win_w, ctx.win_h, "Window Title", nullptr, nullptr);
    if (!win) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }
    ctx.win = win;
    glfwSetWindowPos(win, ctx.win_x,ctx.win_y);

    // Bind the openGL ctx of the window to the current thread
    glfwMakeContextCurrent(win);

    glbinding::initialize(glfwGetProcAddress);
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue, {"glGetError"});
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call) {
        const auto err = glGetError();
        if (err != GL_NO_ERROR){
            std:: cout << fmt::bold_red;
            std::print("OPEN GL ERR [{}].  LAST CALL:",glbinding::aux::Meta::getString(err));
            if (!call.function->isResolved()){
                std::cout << " (UNRESOLVED FUNCTION CALL!!):";
            }
            std:: cout << fmt::clear << std::endl << "\t";

            std:: cout << fmt::cyan;
            std::cout << call.function->name();
            std:: cout << fmt::clear << "(";
            for (const auto& param: call.parameters){
                std::cout << param.get();
                if (param != call.parameters.back()){
                  std::print(", ");
                }
            }
            std::cout << ")";

            if (call.returnValue){
              std::cout << " -> " << call.returnValue.get();
            }
            std::println();
        }
    });


    // ensure we pass the true pixel size to openGL
    i32 viewport_w, viewport_h;
    glfwGetFramebufferSize(win,&viewport_w,&viewport_h);
    // init viewport
    //glViewport(ctx.win_x, ctx.win_y, viewport_w, viewport_h);
    glViewport(ctx.win_x, ctx.win_y, viewport_w, viewport_h);


    // GL CODE
    ShaderProgram prog("shaders/vs.glsl","shaders/fs.glsl");
    VertexBuffer vbo;
    ElementBuffer ebo;
    VertexArray vao;

    Texture2D texture1("resources/textures/missing_content_valve.png");
    Texture2D texture2("resources/textures/illuminati.png",GL_RGBA);

    mat4 scale1 = scale(mat4(1.0f), vec3(0.5f,0.5f, 0.5f));
    mat4 _trans = translate(mat4(1.0f), vec3(-0.5f,0.0f, 0.0f));

    vao.bind();

    vbo.load(triangle_verts, GL_STATIC_DRAW);
    ebo.load(triangle_offsets, GL_STATIC_DRAW);

    vao.buffer_cols = 8;  // x,y,z,s,t,r,g,b
    vao.set_vtx_attributes<f32>(0, 3, +0); // x,y,z
    vao.set_vtx_attributes<f32>(1, 2, +3); // s,t
    vao.set_vtx_attributes<f32>(2, 3, +5); // r,g,b 

    vao.unbind();


    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.setUniform("texture2", (int)1);
    prog.setUniform("scale1", scale1);
    prog.stop();

    u64 frameCount = 0;
    ctx.time.init();
    while (!glfwWindowShouldClose(win)){
        handleInputs(win);

        if (ctx.wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        }
        
        f32 x = frameCount/10.0;
        
        f32 rad = radians(x);
        glClearColor(0.2, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        prog.use();
            texture1.bind();
            texture2.bind();
            prog.setUniform("blendFactor", 0.5f*sin(x)+0.5f);
            prog.setUniform("rotation", rotate(mat4(1.0f), rad, vec3(0.0f,0.0f,1.0f)));
            prog.setUniform("scale0", scale(_trans, vec3(0.5*sin(x)+0.5)));
            prog.setUniform("mouse_transform", translate(mat4(1.0f), mouse_pos_to_NDC()));
            vao.bind();
                prog.setUniform("triangle_idx", (int)0);
                vao.drawElements(3, GL_TRIANGLES);
                prog.setUniform("triangle_idx", (int)1);
                vao.drawElements(3, GL_TRIANGLES);
                prog.setUniform("triangle_idx", (int)2);
                prog.setUniform("blendFactor", 0.0f);
                vao.drawElements(3, GL_TRIANGLES);
            vao.unbind();
        prog.stop();


        glfwSwapBuffers(win);
        glfwPollEvents();
        ctx.time.update();
        frameCount++;
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

#endif
