
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
#include <stb_image.hpp>

#include "Camera.hpp"
#include "Context.hpp"

#include "glmWrapper.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
using namespace gl;
#define _DEBUG

#define _LIBCPP_DEBUG
#define _LIBCPP_DEBUG_USE_EXCEPTIONS


u64 program_epoch_ns;

static void glfw_ErrorCallback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
}
static void glfw_ResizeCallback(GLFWwindow* win_ptr, int width, int height){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    glViewport(0,0,width,height);
    ctx->win.w=width;
    ctx->win.h=height;
    ctx->cam.aspectRatio = ctx->win.aspect();
}
static void glfw_MoveCallback(GLFWwindow* win_ptr, int xpos, int ypos){
    auto* ctx = (Context*)glfwGetWindowUserPointer(win_ptr);
    glViewport(xpos,ypos,ctx->win.w,ctx->win.h);
    ctx->win.x=xpos;
    ctx->win.y=ypos;
}
static void handleInputs(Context& ctx){
    if (glfwGetKey(ctx.win.ptr,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(ctx.win.ptr, true);
    }
    f32 dt = ctx.time.dt; // TODO: implement 
    dt*=60;
    // dt = 1/60
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_W) == GLFW_PRESS){
        ctx.cam.move(Direction::FORWARD,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_S) == GLFW_PRESS){
        ctx.cam.move(Direction::BACKWARD,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_A) == GLFW_PRESS){
        ctx.cam.move(Direction::LEFT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_D) == GLFW_PRESS){
        ctx.cam.move(Direction::RIGHT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_E) == GLFW_PRESS){
        ctx.cam.move(Direction::UP,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_Q) == GLFW_PRESS){
        ctx.cam.move(Direction::DOWN,dt);
    }


    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_LEFT) == GLFW_PRESS){
        ctx.cam.rotate(Direction::LEFT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_RIGHT) == GLFW_PRESS){
        ctx.cam.rotate(Direction::RIGHT,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_UP) == GLFW_PRESS){
        ctx.cam.rotate(Direction::UP,dt);
    }
    if (glfwGetKey(ctx.win.ptr, GLFW_KEY_DOWN) == GLFW_PRESS){
        ctx.cam.rotate(Direction::DOWN,dt);
    }
}


f32 randf(f32 min, f32 max){
    return min+(random()/(f32)RAND_MAX)*(max-min);
}
vec3 randvec3(f32 min, f32 max){
    return vec3{
        randf(min,max),
        randf(min,max),
        randf(min,max),
    };
}
static const std::array<f32, 5 * 36> triangle_verts ={
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    // compiles and links a vertex and fragment shader from the path of their source files.
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
    // `#ifdef _DEBUG`, this function will search with the ./shaders dir with `rg` to see if the `name` requested exists in a shader. 
    // If it doesnt, we crash immediately, providing an error message containing the top 1 `N=3` closest fuzzy results from `agrep`.
    template<typename T>
    void setUniform(std::string name, T val){
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

    u32 sum{};
    template <class VT>
    void set_vtx_attributes(u32 location, i32 count, u64 offset){
        // assumed packed non normalized vertex data
        void* offset_ptr = (void*)(offset*sizeof(VT));
        i32 stride  = buffer_cols * sizeof(VT);
        GLenum type = gl_type<VT>();
        glVertexAttribPointer(location, count, type, false,stride, offset_ptr);
        glEnableVertexAttribArray(location);
        sum+=count;
        LOG_DEBUG("Adding {} -> {}",count,sum);
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
    u32 idx=0;
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
void init_glFunctionLoader(){
    // at the moment, using glbinding, but i think this is a reasonable thing to swap out
    glbinding::initialize(glfwGetProcAddress);
    glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After | glbinding::CallbackMask::ParametersAndReturnValue, {"glGetError"});
    glbinding::setAfterCallback([](const glbinding::FunctionCall & call) {
        const auto err = glGetError();
        if (err != GL_NO_ERROR){
            std:: cout << fmt::bold_red;
            std::print(">>>>>OPEN GL ERR [{}].  LAST CALL:",glbinding::aux::Meta::getString(err));
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
}

void init_glfw(Context& ctx){
    program_epoch_ns = get_current_ns();
    stbi_set_flip_vertically_on_load(true);  
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

    ctx.win.ptr = glfwCreateWindow(ctx.win.w, ctx.win.h, "Window Title", nullptr, nullptr);
    if (!ctx.win.ptr) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    // WARNING: Doing any OpenGL calls before makeContextCurrent will fk shit up
    glfwMakeContextCurrent(ctx.win.ptr);
    // WARNING:  ^^^^^^^




    glfwSetWindowUserPointer(ctx.win.ptr,&ctx);
//    glfwSetWindowPosCallback(win,glfw_MoveCallback);
    glfwSetWindowPos(ctx.win.ptr, ctx.win.x,ctx.win.y);

    float xscale, yscale;
    glfwGetWindowContentScale(ctx.win.ptr, &xscale, &yscale);
    if (xscale !=1.0 || yscale != 1.0){
        LOG_WARN("Retina mode detected, check scaling if any dimensions are weird");
    }
    // ensure we pass the true pixel size to openGL
    glfwGetFramebufferSize(ctx.win.ptr,&ctx.win.w,&ctx.win.h);
}
void init_opengl(Context& ctx){
    init_glFunctionLoader();
    
    // WARNING: Only set/call these once glbinding has been setup, as they make gl calls.
    assert(ctx.win.ptr);
    glfwSetFramebufferSizeCallback(ctx.win.ptr,glfw_ResizeCallback);
    glfw_ResizeCallback(ctx.win.ptr,ctx.win.w,ctx.win.h);
    glViewport(ctx.win.x, ctx.win.y, ctx.win.w, ctx.win.h);
    // perform depth testing, i.e refuse draw calls which would cause a vertex further away to overwrite a closer one
    glEnable(GL_DEPTH_TEST); 
}

int main(int argc, char** argv) {
    Camera cam((vec3){0,0,6});
    auto ctx = Context(cam);
    init_glfw(ctx);
    init_opengl(ctx);

    ShaderProgram prog("shaders/vs.glsl","shaders/fs.glsl");
    VertexBuffer vbo;
    VertexArray vao;

    Texture2D texture1("resources/textures/missing_content_valve.png");

    mat4 model_matrix = mat4(1.0f);



    // apply model transformations
//    model_matrix = rotate(model_matrix, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    //model_matrix = scale(model_matrix, vec3(2.0f, 2.0f, 2.0f));


    // apply perspective translation

    LOG_EXPR(model_matrix);
//    LOG_EXPR(radians(vertical_fov));
//    LOG_EXPR(projection_matrix);

    vao.bind();
    vbo.load(triangle_verts, GL_STATIC_DRAW);
    vao.buffer_cols = 5;  // x,y,z,s,t,r,g,b
    vao.set_vtx_attributes<f32>(0, 3, +0); // x,y,z
    vao.set_vtx_attributes<f32>(1, 2, +3); // s,t

    vao.unbind();


    prog.use();
    prog.setUniform("texture1", (int)0);
    prog.stop();

    const std::vector<vec3> cube_positions = {
        randvec3(-2,2),
        randvec3(-2,2),
        randvec3(-2,2),
        randvec3(-2,2),
        randvec3(-2,2),
    };

    u64 frameCount = 0;
    ctx.time.init();
    while (!glfwWindowShouldClose(ctx.win.ptr)){
        handleInputs(ctx);

        if (ctx.wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        }
        
        f32 x = frameCount/10.0f;
        glClearColor(1.0, 0.5, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prog.use();
            texture1.bind();
            vao.bind();
            mat4 view;

            prog.setUniform("view", ctx.cam.getViewMatrix());
            prog.setUniform("proj", ctx.cam.getProjectionMatrix());
            for (const auto& cube_pos : cube_positions){
                    prog.setUniform("model", model_matrix);
                    vao.drawArrays(36, GL_TRIANGLES);
            }
            vao.unbind();
        prog.stop();


        glfwSwapBuffers(ctx.win.ptr);
        glfwPollEvents();
        ctx.time.update();
        frameCount++;
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
