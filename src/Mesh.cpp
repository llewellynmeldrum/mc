

#include "Breakpoints.hpp"
#include "FormatSpecs.hpp"
#include <memory>
#include "Engine.hpp"
#include "Vertex.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"

#include "VertexAttributeType.hpp"

#include "glbindingWrapper.hpp"
#include "Assertion.hpp"
using namespace gl;

// NOTE: in gl, known as GL_TRIANGLES, GL_LINES, GL_STRIP
constexpr GLenum MeshBase::PrimitiveType() {
    return GL_TRIANGLES;
}
constexpr GLenum IndexedMesh::PrimitiveType() {
    return GL_TRIANGLES;
}

IndexedMesh::IndexedMesh(WorldChunkCoord _chunkCoord, const_span<Vertex> vertices, const_span<u32> offsets) :
    chunkCoord(_chunkCoord)
{
    IndexedMesh::setup_mesh(vertices, offsets);

    auto n_quads = offsets.size() / INDICES_PER_QUAD;
    center_points.reserve(n_quads);
    for (auto quad_idx = 0uz; quad_idx<n_quads; quad_idx++){
        auto vtx_idx_offset = quad_idx * VTX_PER_QUAD;
        // i.e 0, 4, 8, 12
        glm::vec3 p0 = vertices[vtx_idx_offset+0].pos;
        glm::vec3 p1 = vertices[vtx_idx_offset+1].pos;
        glm::vec3 p2 = vertices[vtx_idx_offset+2].pos;
        glm::vec3 p3 = vertices[vtx_idx_offset+3].pos;
        center_points.emplace_back(
            (p0.x+p1.x+p2.x+p3.x) / 4.0f,
            (p0.y+p1.y+p2.y+p3.y) / 4.0f,
            (p0.z+p1.z+p2.z+p3.z) / 4.0f
        );
    }

    ebo_cache.reserve(n_quads);
    auto range = offsets 
        | std::views::chunk(INDICES_PER_QUAD) 
        | std::views::transform(
            [](auto&& rg){
                QuadIndices q;
                std::ranges::copy(rg,q.arr.begin());
                return q;
            }
        );
    ebo_cache.assign_range(range);
}

void IndexedMesh::resort_quad_indices(WorldFloatPos src_world, bool near_to_far){
    using namespace std;
    i32 n_quads = ebo_cache.size();
    if (n_quads==0) return;


    glm::vec3 origin = toWorldOrigin(chunkCoord).raw();
    // NOTE: we offset the source_world by chunk origin,
    // st. src_glm is effectively 'translated' into the local chunk space
    auto src_glm = src_world.raw() - origin;
    // 1. store the sorted order of the quads:
    auto ids = 
        views::iota(0, n_quads)
        | ranges::to<std::vector<i32>>();

    if (near_to_far){
        ranges::sort(
            ids,
            [&](i32 id_1, i32 id_2){
                const auto& p1 = AT(center_points,id_1);
                const auto& p2 = AT(center_points,id_2);
                return glm::distance2(src_glm, p1) < glm::distance2(src_glm,p2);
            }
        );
    }else{
        ranges::sort(
            ids,
            [&](i32 id_1, i32 id_2){
                const auto& p1 = AT(center_points,id_1);
                const auto& p2 = AT(center_points,id_2);
                return glm::distance2(src_glm, p1) > glm::distance2(src_glm,p2);
            }
        );
    }
    auto& sorted_ids = ids;
    // 2. apply the new sorted order 
    // (in-place sort is not really possible here, memory overhead is negligble anyways)
    std::vector<QuadIndices> new_ebo_cache;
    std::vector<glm::vec3> new_center_points;
    new_ebo_cache.reserve(ebo_cache.size());
    for (auto& id: sorted_ids){
        new_ebo_cache.push_back(AT(ebo_cache,id));
        new_center_points.push_back(AT(center_points,id));
    }
    ebo_cache = std::move(new_ebo_cache);
    center_points = std::move(new_center_points);
    reupload_indices(ebo_cache);
}
void IndexedMesh::reupload_indices(const_span<QuadIndices> new_indices){
    vao.bind();
    ebo.load(new_indices.size_bytes(), new_indices.data());
    vao.unbind();
}
void IndexedMesh::setup_mesh(const_span<Vertex> vertices, const_span<u32> indices) {
    offset_count = indices.size();
    vertex_count = vertices.size();
    vao.bind();
    assert((indices.size() / 6) * 4 == vertices.size());
    vbo.load<Vertex>(vertices);
    ebo.load(indices);
    vao.apply_layout(Vertex::layout());
    vao.unbind();
}

void IndexedMesh::draw() const {
    assert(offset_count != 0);
    vao.bind();
    vao.drawElements(offset_count, PrimitiveType());
    vao.unbind();
}

void MeshBase::setupMesh(std::vector<Vertex> vertices) {
    vertex_count = vertices.size();
    vao.bind();
    vbo.load<Vertex>(vertices);
    vao.apply_layout(Vertex::layout());
    vao.unbind();
}
void MeshBase::draw() const {
    vao.bind();
    vao.drawArrays(vertex_count, PrimitiveType(), 0);
    vao.unbind();
}
