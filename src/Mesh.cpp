
#define GL_SILENCE_DEPRECATION
#include "glbinding/gl/enum.h"
#include "glbinding/glbinding.h"
#include "glbinding/gl/gl.h"
using namespace gl;

#include "Context.hpp"

#include "Vertex.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"
#include <memory>

ElementBuffer::ElementBuffer() {
    glGenBuffers(1, &this->id);
}
void ElementBuffer::bind() {
    glBindBuffer(static_cast<GLenum>(buffer_type), id);
}

template <ContiguousContainer C>
void ElementBuffer::load(C c, u32 usage_type){
    bind();
    using T = C::value_type;
    static_assert(std::same_as<T,u32>);

    glBufferData(buffer_type, sizeof(c), c.data(), static_cast<GLenum>(usage_type));
}
const u32 ElementBuffer::buffer_type=static_cast<u32>(GL_ELEMENT_ARRAY_BUFFER);
const u32 VertexBuffer::buffer_type=static_cast<u32>(GL_ARRAY_BUFFER);

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &this->id);
}
void VertexBuffer::bind() {
    glBindBuffer(static_cast<GLenum>(buffer_type), id);
}

void VertexBuffer::load(const std::vector<Vertex>& c, u32 usage_type, u32 offset=0){
    bind();
    glBufferData(static_cast<GLenum>(buffer_type), sizeof(c)* c.size(), c.data()+offset, static_cast<GLenum>(usage_type));
}

VertexArray::VertexArray(){
    glGenVertexArrays(1, &id);
}
void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
void VertexArray::drawElements(u32 num, u32 elem_t) {
    glDrawElements(static_cast<GLenum>(elem_t), num, GL_UNSIGNED_INT, nullptr);
}
void VertexArray::drawArrays(u32 count, u32 elem_t, u32 offset) const {
    glDrawArrays(static_cast<GLenum>(elem_t), offset, count);
}

template <class VT>
void VertexArray::set_vtx_attributes(u32 location, i32 count, u64 offset){
    // assumed packed non normalized vertex data
    void* offset_ptr = (void*)(offset*sizeof(VT));
    i32 stride  = buffer_cols * sizeof(VT);
    GLenum type = gl_type<VT>();
    glVertexAttribPointer(location, count, type, false,stride, offset_ptr);
    glEnableVertexAttribArray(location);
    sum+=count;
}




enum CubeFace{
    CubeFaceBACK,
    CubeFaceFRONT,
    CubeFaceLEFT,
    CubeFaceRIGHT,
    CubeFaceBOT,
    CubeFaceTOP,
};

const std::vector<std::vector<Vertex>> defaultCubeFaces = {
    // FRONT (+X)
    std::vector<Vertex>{
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={-0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={-0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },

    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},//{1.0f, 1.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},//{1.0f, 0.0f}
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},//{0.0f, 0.0f}
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},//{0.0f, 1.0f}

    },
    
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
    },
    
    std::vector<Vertex>{
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
    },
    
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f, -0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f, -0.5f,  0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f,  0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={    -0.5f, -0.5f, -0.5f,  }, .txCoords = {1.0f, 0.0f}},
    }, 
    std::vector<Vertex>{
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f, -0.5f,  }, .txCoords = {1.0f, 1.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={     0.5f,  0.5f,  0.5f,  }, .txCoords = {1.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f,  0.5f,  }, .txCoords = {0.0f, 0.0f}},
        Vertex{.pos={    -0.5f,  0.5f, -0.5f,  }, .txCoords = {0.0f, 1.0f}},
    },
};
Mesh::Mesh(){
    setup();
}
void Mesh::setup() {
    vao.bind();
    std::vector<Vertex> _vertices;
    for (u64 faceID = 0; faceID < CUBE_FACE_COUNT; faceID++) {
        if (isFaceVisible[faceID]) {
            const auto& uv_tex_coords = ctx.atlas.remapUVs(0, faceID, defaultCubeFaces[faceID]);
            for (u64 vtx = 0; Vertex v1 : defaultCubeFaces[faceID]) {
                v1.txCoords = uv_tex_coords[vtx++];
                LOG_DEBUG("face:{},vtx:{}", faceID, vtx);
                LOG_EXPR(v1.txCoords);
                _vertices.push_back(v1);
            }
            visibleFaceCount += 6;
        }
    }
    vbo.load(_vertices, static_cast<u32>(GL_STATIC_DRAW));
    vao.buffer_cols = 5;                    // x,y,z, s,t
    vao.set_vtx_attributes<f32>(0, 3, +0);  // x,y,z
    vao.set_vtx_attributes<f32>(1, 2, +3);  // s,t
    vao.unbind();
}

inline void Mesh::draw(ShaderProgram& prog, mat4& model_matrix) const {
    prog.use();
    vao.bind();
    prog.setUniform("view", ctx.cam.getViewMatrix());
    prog.setUniform("proj", ctx.cam.getProjectionMatrix());
    prog.setUniform("model", model_matrix);

    vao.drawArrays(visibleFaceCount, static_cast<u32>(GL_TRIANGLES), 0);  // front
    vao.unbind();
    prog.stop();
}

