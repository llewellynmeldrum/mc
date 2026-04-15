
#include <cassert>
#include <concepts>
#include <print>
#include <string>
#include <unordered_map>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include "annotated_gl.h"

#define GL_SILENCE_DEPRECATION
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
    bool wireframe = false;
}ctx;

static void glfw_ErrorCallback(int error, const char* description){
    LOG_ERROR("GLFW({}): {}",error, description);
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
    const f64 x = 1;//ctx.time.elapsed_ms*1000.0;
    glClearColor(0.2f, mod.run(x), 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void resize_callback(GLFWwindow* win, i32 w, i32 h){
    (void)w; 
    (void)h;
//  no resizing >:(
//  glViewport(ctx.win_x,ctx.win_y,ctx.win_w,ctx.win_h)
}

static const std::array<float, 6 * 3> triangle_vertices ={
     0.5,  0.5, 0.0,
     0.5, -0.5, 0.0,
    -0.5,  0.5, 0.0,
    -0.5, -0.5, 0.0,
};
static const std::array<u32, 3> topRight_offsets{ 
	 0 , 	// idx::0 slot occupied by vtx::0
	 1 , 	// idx::1 slot occupied by vtx::1
	 2 , 	// idx::2 slot occupied by vtx::2 
};

static const std::array<u32,3> botLeft_offsets{
	 1 , 	// idx::3 slot occupied by vtx::1 (vtx::1==vtx::3)
	 3 ,		// idx::4 slot occupied by vtx::3 
	 2 ,		// idx::5 slot occupied by vtx::2  (vtx::2==vtx::5)
};

static const char *vertex_shader_src =\
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const char* fragment_shader_src_red =\
"#version 330 core\n"
"out vec4 frag_color;\n"
"\n"
"void main(){\n"
"    frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"} \n";

static const char* fragment_shader_src_blue =\
"#version 330 core\n"
"out vec4 frag_color;\n"
"\n"
"void main(){\n"
"    frag_color = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
"} \n";

template <typename C>
concept ContiguousContainer = std::ranges::contiguous_range<C>;

const std::unordered_map<GLenum,std::string> glErrorString = {
{GL_INVALID_ENUM , " An unacceptable value was specified for an enumerated argument."},
{GL_INVALID_VALUE , " A numeric argument is out of range."},
{GL_INVALID_OPERATION , " The specified operation is not allowed in the current state of the OpenGL state machine."},
{GL_INVALID_FRAMEBUFFER_OPERATION , " The command is applied to a framebuffer that is not complete."},
{GL_OUT_OF_MEMORY , " There is not enough memory left to execute the command."},
//{GL_STACK_OVERFLOW , " A command would cause a stack overflow (used in legacy fixed-function stacks)."},
//{GL_STACK_UNDERFLOW , " A command would cause a stack underflow."},
};

#define glLogError() _GL_printError(false,__FILE_NAME__, __LINE__);
#define glBreakpoint() _GL_printError(true,__FILE_NAME__, __LINE__);

void _GL_printError(bool quit, const char* name, int line){
    // glad is eating all my errors. Need to disable that. Perhaps its because its a debug build.
    GLenum err= glGetError();
    if (err){
        auto ms = ms_since_start()/1000.0;
        std ::println("{:03.3f} {}{:<8}{} {}{}:{:<3}{} {}{}| {} ({})",
                ms, RED, "[GL ERR]", "\e[0m", "\e[1m", name, line, "\e[0m",
                      RED, "\e[0m", glErrorString.at(err),err);
        if (quit){LOG_EXIT(EXIT_FAILURE);}
    }
}
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
u32 currentlyBoundBuffer = 0;
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
        if (currentlyBoundBuffer != id){
            LOG_ERROR("Tried to load data into a buffer that was not the one bound to.");
        }
        using T = C::value_type;
        static_assert(std::same_as<T,u32>);

        glBufferData(buffer_type, sizeof(c), c.data(), usage_type);
        glBreakpoint();
    }
};
struct VertexBuffer{
    u32 id;
    u32 size; // size in bytes
    GLenum buffer_type={};
    GLenum value_type_enum={};
    u64 value_type_size={};
    u32 location={};

    VertexBuffer(u32 buffer_type, u32 size_i = 1){
        this->buffer_type = buffer_type;
        glGenBuffers(size_i, &this->id);
    }

    // binds OPENGL to the currently set *buffer_type*
    void bind(){
		currentlyBoundBuffer=id;
        glBindBuffer(buffer_type, id);
    }

//    glDebugMessageCallback();
//    void funcname(GLenum source, GLenum type, GLuint id,
//   GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    // populates the VBO's buffer of the currently bound type, specifying the usage_type.
    //  GL_STREAM_DRAW:       for data set once and used a few times by GPU.
    //  GL_STATIC_DRAW:       for data set once and used ALOT by the GPU.   (faster reads)
    // GL_DYNAMIC_DRAW:       for data set ALOT and used ALOT by the GPU    (faster reads/writes) 
    template <ContiguousContainer C>
    void load_data(C c, GLenum usage_type, u32 offset=0){
        using T = C::value_type;
        value_type_size = sizeof(T);
        value_type_enum = get_type_enum<T>();

        glBufferData(buffer_type, sizeof(c), c.data()+offset, usage_type);
        glBreakpoint();
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
struct VertexArray{
    u32 id;
    i32 count;
    VertexArray(i32 _count=1){
        this->count  =_count;
        glGenVertexArrays(count, &id);  
    }
    void bind(){
		currentlyBoundBuffer=id;
        glBindVertexArray(id);
    }
    void unbind(){
        glBindVertexArray(0);
    }
    //GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer
    void set_and_enable_vattrs(VertexAttribute attr){
        glVertexAttribPointer(attr.location, attr.count, attr.type.v,attr.normalized,attr.stride, attr.offset);
        glEnableVertexAttribArray(attr.location);
    }
    // draws *num* vertices, chosen via whichever EBO is bound to this VAO
    void drawElements(u32 num, GLenum elem_t){
        glDrawElements(elem_t, num, GL_UNSIGNED_INT, (void*)nullptr);
    }

};

int main(int argc, char** argv) {
    const double x = 20.5;
    const double & xref = x;
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


    // ensure we pass the true pixel size to openGL
    i32 viewport_w, viewport_h;
    glfwGetFramebufferSize(win,&viewport_w,&viewport_h);
    // init viewport
    //glViewport(ctx.win_x, ctx.win_y, viewport_w, viewport_h);
    glViewport(ctx.win_x, ctx.win_y, viewport_w, viewport_h);

    // register resize callback
    glfwSetFramebufferSizeCallback(win, resize_callback);


    VertexShader vtx;
    vtx.readInlineSource(vertex_shader_src);
    if (!vtx.compileSource()){
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    FragmentShader frag_red;
    frag_red.readInlineSource(fragment_shader_src_red);
    if (!frag_red.compileSource()){
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    FragmentShader frag_blue;
    frag_blue.readInlineSource(fragment_shader_src_blue);
    if (!frag_blue.compileSource()){
        glfwTerminate();
        LOG_EXIT(EXIT_FAILURE);
    }

    ShaderProgram prog_red;
    prog_red.attach(vtx);
    prog_red.attach(frag_red);
    prog_red.link();

    ShaderProgram prog_blue;
    prog_blue.attach(vtx);
    prog_blue.attach(frag_blue);
    prog_blue.link();




    VertexBuffer shared_vbo(GL_ARRAY_BUFFER);
    ElementBuffer ebo_tr, ebo_bl;
    VertexArray vao_tr, vao_bl; 

    vao_tr.bind();
        shared_vbo.bind();
        shared_vbo.load_data(triangle_vertices, GL_STATIC_DRAW);

        ebo_tr.bind();
        ebo_tr.load(topRight_offsets, GL_STATIC_DRAW);
        vao_tr.set_and_enable_vattrs({
                .location=0,
                .count = 3,                 // 3 floats per vec
                .type={GL_FLOAT},
                .normalized = false,
                .stride = 3* sizeof(float), // 3 floats per vec, packed
                .offset=nullptr,
        });
        glBreakpoint();
    vao_tr.unbind();

    vao_bl.bind();
    shared_vbo.bind();
    shared_vbo.load_data(triangle_vertices, GL_STATIC_DRAW);

    ebo_bl.bind();
    ebo_bl.load(botLeft_offsets, GL_STATIC_DRAW);
    vao_bl.set_and_enable_vattrs({
            .location=0,
            .count = 3,                 // 3 floats per vec
            .type={GL_FLOAT},
            .normalized = false,
            .stride = 3* sizeof(float), // 3 floats per vec, packed
            .offset=nullptr,
    });
    vao_bl.unbind();



    glBreakpoint();
    ctx.time.init();
    while (!glfwWindowShouldClose(win)){
        handleInputs(win);

        if (ctx.wireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        }
        // i want a consteval/constexpr function which swaps u32->GL_UNSIGNED_INT
        // to_Glenum<T>()-> ?
        
        renderCommands(win);

        prog_red.use();
        vao_tr.bind();
        vao_tr.drawElements(3, GL_TRIANGLES);
        vao_tr.unbind();

        prog_blue.use();
        vao_bl.bind();
        vao_bl.drawElements(3, GL_TRIANGLES);
        vao_bl.unbind();


        glfwSwapBuffers(win);
        glfwPollEvents();
        ctx.time.update();
        glBreakpoint();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
