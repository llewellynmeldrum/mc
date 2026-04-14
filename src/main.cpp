
#include <assert.h>
#include <concepts>
#include <print>
#include <string>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include "annotated_gl.h"

#include <GLFW/glfw3.h>

#include <format>
#include <limits>

#include "Logger.hpp"
#include "Types.h"

#include "UnixHelpers.hpp"

u64 program_epoch_ns;
struct Context {
        struct Timer{
        u64 framecount = 0;
        f64 dt_ms= 0.0f;
        f64 elapsed_ms = 0.0;
        void init() noexcept{ 
            glfwSetTime(0.0);
        }
        void update() noexcept{
            f64 prev_elapsed_ms = elapsed_ms;
            elapsed_ms = glfwGetTime()/1000.0;
            dt_ms = elapsed_ms-prev_elapsed_ms;
        }
    }time;
    static constexpr i32 win_x = 0;
    static constexpr i32 win_y = 0;
    static constexpr i32 win_w = 900; // half my screen width
    static constexpr i32 win_h = 1169;
}ctx;

static void error_callback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
}
static void gl_error_callback(GLenum source, GLenum type, GLuint id,
   GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
  std::println( stderr, "GL CALLBACK: {} type = 0x{:x}, severity = 0x{:x}, message = {}",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
//    LOG_ERROR("GLERROR: {} T:{} ID:{} -> {}",source,type,id, message);
}
static void handleInputs(GLFWwindow* win){
    if (glfwGetKey(win,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(win, true);
    }
}
struct Modulator{
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 run(f32 x) const noexcept{
        const f32 y = a*sin(b*x-c)+d;
    //    LOG_DEBUG("[{},{}]",x,y);
        return y;
    }
};
static void renderCommands(GLFWwindow* win){
    static constexpr Modulator mod = {.a=0.5,.b=1.2,.c=1.6,.d=0.5};
    const f64 x = ctx.time.elapsed_ms*1000.0;
    glClearColor(0.2f, mod.run(x), 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void resize_callback(GLFWwindow* win, i32 w, i32 h){
    (void)w; 
    (void)h;
//  no resizing >:(
//  glViewport(ctx.win_x,ctx.win_y,ctx.win_w,ctx.win_h)
}

static const std::array<float, 3 * 3> triangle_vertices ={
    -0.5, -0.5,0.0,
    0.5, -0.5,0.0,
    0.0, 0.5, 0.0,
};

static const char *vertex_shader_src =\
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const char* fragment_shader_src =\
"#version 330 core\n"
"out vec4 frag_color;\n"
"\n"
"void main(){\n"
"    frag_color = vec4(1.0f, 0.2f, 1.0f, 1.0f);\n"
"} \n";

template <typename C>
concept ContiguousContainer = std::ranges::contiguous_range<C>;

#include <fstream>
struct Shader{
    u32 id;
    GLenum ShaderType;
    Shader(GLenum shader_type){
        this->ShaderType = shader_type;
        this->id = glCreateShader(shader_type);
    }
    bool readSource(const char* filename){
        std::ifstream file(filename);
        if (!file.is_open()){
            LOG_ERROR("Failed to open {} shader source.", tostr(ShaderType));
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

    void readInlineSource(const char* src_data){
        this->src_buf = std::string(src_data);
        const char* data = src_buf.c_str();
        glShaderSource(id, 1, &data, nullptr);
    }

    bool compileSource(){
        glCompileShader(id);
        if (has_error(GL_COMPILE_STATUS)){
            LOG_ERROR("{} shader failed to compile.\n{}",tostr(ShaderType), get_info_log());
            return false;
        }
        return true;
    }
    std::string src_buf;
    static std::string tostr(GLenum shader_type){
        if (shader_type== GL_VERTEX_SHADER)         return "Vertex";
        else if (shader_type== GL_FRAGMENT_SHADER)  return "Vertex";
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
    VertexShader(): Shader(GL_VERTEX_SHADER){}
};
struct FragmentShader: Shader{
    FragmentShader(): Shader(GL_FRAGMENT_SHADER){}
};

struct ShaderProgram{
    u32 id;
    bool ready_for_use = false;
    ShaderProgram() {
        this->id = glCreateProgram();
    }

    void attach(Shader& shader){
        glAttachShader(id, shader.id);
    }

    bool link(){
        glLinkProgram(id);
        if (has_error(GL_LINK_STATUS)){
            LOG_ERROR("ShaderProgram failed to link. Log:{}", get_info_log());
            return false;
        }
        return (ready_for_use = true);
    }
    bool has_error(GLenum param_name){
        i32 success = 0;
        glGetProgramiv(id, param_name, &success);
        return !success;
    }
    void use(){
        assert(ready_for_use);
        glUseProgram(id);
    }
    std::string get_info_log(){
        constexpr u64 buf_sz = 512;
        std::string info_log(buf_sz,'\0');
        glGetProgramInfoLog(id, buf_sz, nullptr, info_log.data());
        return info_log;
    }
};

struct VertexObject{
    u32 id;
    VertexObject()=delete;
};
template<typename T>
constexpr GLenum get_type_enum(){
    GLenum res{};
    if constexpr (std::same_as<T, f32>){
        res = GL_FLOAT;
    } else if constexpr(std::same_as<T,f64>){
        res = GL_DOUBLE;
    } else if constexpr(std::same_as<T,i32>){
        res = GL_INT;
    }else if constexpr (std::same_as<T,u32>){
        res = GL_UNSIGNED_INT;
    }
    return res;
}
struct VBO{
    u32 id;
    u32 size; // size in bytes
    GLenum bound_buffer_type={};
    GLenum value_type_enum={};
    u64 value_type_size={};
    u32 location={};

    VBO(u32 buffer_type, u32 size_i = 1){
        bound_buffer_type = buffer_type;
        glGenBuffers(size_i, &this->id);
    }
    // binds the VBO to a *buffer_type*
    void bind(){
        if (glGetError()){
            LOG_ERROR("GLAD: {}",glGetError());
        }
        glBindBuffer(bound_buffer_type, id);
        if (glGetError()){
            LOG_ERROR("GLAD: {}",glGetError());
        }
    }

//    glDebugMessageCallback();
//    void funcname(GLenum source, GLenum type, GLuint id,
//   GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    // populates the VBO's buffer of the currently bound type, specifying the usage_type.
    //  GL_STREAM_DRAW:       for data set once and used a few times by GPU.
    //  GL_STATIC_DRAW:       for data set once and used ALOT by the GPU.   (faster reads)
    // GL_DYNAMIC_DRAW:       for data set ALOT and used ALOT by the GPU    (faster reads/writes) 
    template <ContiguousContainer C>
    void populate(C c, GLenum usage_type){
        using T = C::value_type;
        value_type_size = sizeof(T);
        value_type_enum = get_type_enum<T>();

        assert(usage_type == GL_STREAM_DRAW ||
               usage_type == GL_STATIC_DRAW ||
               usage_type == GL_DYNAMIC_DRAW);
        glBufferData(bound_buffer_type, sizeof(c), c.data(), usage_type);
    }
};

struct AttributeType{
    GLenum v;
};
struct VertexAttribute {
    u32 location;
    u32 count;
    AttributeType type;
    bool normalized;
    u32 stride;
    void* offset=nullptr;
};
struct VAO{
    u32 id;
    i32 count;
    VAO(i32 _count=1){
        this->count  =_count;
        glGenVertexArrays(count, &id);  
    }
    void bind(){
        glBindVertexArray(id);
    }
    //GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    void attach_attributes(this auto&& vao, VBO& vbo, VertexAttribute attr){
        GLenum type_enum;
        vao.bind();
        vbo.bind();
        glVertexAttribPointer(attr.location, attr.count, attr.type.v,attr.normalized,attr.stride, attr.offset);
        glEnableVertexAttribArray(attr.location);
    }

};

int main(int argc, char** argv) {
    const double x = 20.5;
    const double & xref = x;
    program_epoch_ns = get_current_ns();
    glfwSetErrorCallback(error_callback);

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
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required for opengl 3.2+
    #endif

    // init glfw window
    GLFWwindow* win = glfwCreateWindow(ctx.win_w, ctx.win_h, "Window Title", nullptr, nullptr);
    if (!win) {
        LOG_ERROR("Failed to initialize GLFW.");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }
    glfwSetWindowPos(win, ctx.win_x,ctx.win_y);

    // Bind the openGL ctx of the window to the current thread
    glfwMakeContextCurrent(win);

    // init glad, 
    if (!gladLoadGL(glfwGetProcAddress)){
        LOG_ERROR("Failed to initialize GLAD");
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }


    // init viewport
    glViewport(ctx.win_x, ctx.win_y, ctx.win_w, ctx.win_h);

    // register resize callback
    glfwSetFramebufferSizeCallback(win, resize_callback);



    VertexShader vtx;
    vtx.readInlineSource(vertex_shader_src);
    if (!vtx.compileSource()){
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }
    LOG_INFO("{} shader succesfully compiled. Log:{}",vtx.tostr(vtx.ShaderType), vtx.get_info_log());

    FragmentShader frag;
    frag.readInlineSource(fragment_shader_src);
    if (!frag.compileSource()){
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }
    LOG_INFO("{} shader succesfully compiled. Log:{}",frag.tostr(frag.ShaderType), frag.get_info_log());

    ShaderProgram prog;
    prog.attach(vtx);
    prog.attach(frag);
    prog.link();

    VBO vbo(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.populate(triangle_vertices, GL_STATIC_DRAW);

    VertexAttribute attr = {
        .location=0,
        .count =3,
        .type={GL_FLOAT},
        .normalized = false,
        .stride = 3* sizeof(float),
        .offset=nullptr,
    };
    VAO vao; 



    ctx.time.init();
    while (!glfwWindowShouldClose(win)){
        handleInputs(win);

        renderCommands(win);
        prog.use();
        vao.attach_attributes(vbo,attr);
        glDrawArrays(GL_TRIANGLES,0,attr.count);

        glfwSwapBuffers(win);
        glfwPollEvents();
        ctx.time.update();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
