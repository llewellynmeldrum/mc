#pragma once 
// ===========
// Mesh_RawData.hpp
// ===========
#include "Block.hpp"
#include "ChunkConstants.hpp"
#include "Vertex.hpp"
#include <utility>
using QuadVertexList = std::array<Vertex, VTX_PER_QUAD>;

template<size_t T>
using ShapeQuadList = std::array<QuadVertexList, T>;

// clang-format off
using Quad = std::array<Vertex,VTX_PER_QUAD>;
namespace cross_vtx{
    static constexpr inline BlockShape shape = BlockShape::CROSS;
    static inline constexpr f32 P = +1.0;
    static inline constexpr f32 N = 0.0;
    static inline constexpr glm::vec3 NNN{ N, N, N};
    static inline constexpr glm::vec3 NPN{ N, P, N};
    static inline constexpr glm::vec3 NNP{ N, N, P};
    static inline constexpr glm::vec3 PNN{ P, N, N};
    static inline constexpr glm::vec3 PPN{ P, P, N};
    static inline constexpr glm::vec3 PNP{ P, N, P};
    static inline constexpr glm::vec3 NPP{ N, P, P};
    static inline constexpr glm::vec3 PPP{ P, P, P};


    // see docs/deriving_cross_geometry.md if interested
    inline const ShapeQuadList<QUADS_PER_CROSS> quads = {
        // Blue 
        Quad{
            make_vtx<shape>(NNP, {0,1}, 0),
            make_vtx<shape>(PNN, {1,1}, 0),
            make_vtx<shape>(PPN, {1,0}, 0),
            make_vtx<shape>(NPP, {0,0}, 0),
        },
        Quad{
            make_vtx<shape>(PNN, {1,1}, 0),
            make_vtx<shape>(NNP, {0,1}, 0),
            make_vtx<shape>(NPP, {0,0}, 0),
            make_vtx<shape>(PPN, {1,0}, 0),
        },
        // Red
        Quad{
            make_vtx<shape>(NNN, {0,1}, 1),
            make_vtx<shape>(PNP, {1,1}, 1),
            make_vtx<shape>(PPP, {1,0}, 1),
            make_vtx<shape>(NPN, {0,0}, 1),
        },
        Quad{
            make_vtx<shape>(PNP, {1,1}, 1),
            make_vtx<shape>(NNN, {0,1}, 1),
            make_vtx<shape>(NPN, {0,0}, 1),
            make_vtx<shape>(PPP, {1,0}, 1),
        },
    };
}

namespace cube_vtx{
    static constexpr inline BlockShape shape = BlockShape::CUBE;
    inline constexpr glm::vec3 NNN{ 0.0,0.0,0.0};
    inline constexpr glm::vec3 NPN{ 0.0, 1.0,0.0};
    inline constexpr glm::vec3 NNP{ 0.0,0.0, 1.0};
    inline constexpr glm::vec3 PNN{ 1.0,0.0,0.0};
    inline constexpr glm::vec3 PPN{ 1.0, 1.0,0.0};
    inline constexpr glm::vec3 PNP{ 1.0,0.0, 1.0};
    inline constexpr glm::vec3 NPP{ 0.0, 1.0, 1.0};
    inline constexpr glm::vec3 PPP{ 1.0, 1.0, 1.0};

    inline ShapeQuadList<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            make_vtx<shape>(PNN, {0,1}, 0),
            make_vtx<shape>(NNN, {1,1}, 0),
            make_vtx<shape>(NPN, {1,0}, 0),
            make_vtx<shape>(PPN, {0,0}, 0),
        },
        // Direction::Backward
        Quad{
            make_vtx<shape>(NNP, {0,1}, 1),
            make_vtx<shape>(PNP, {1,1}, 1),
            make_vtx<shape>(PPP, {1,0}, 1),
            make_vtx<shape>(NPP, {0,0}, 1),
        },
        // Direction:: Left
        Quad{
            make_vtx<shape>(NNN, {0,1}, 2),
            make_vtx<shape>(NNP, {1,1}, 2),
            make_vtx<shape>(NPP, {1,0}, 2),
            make_vtx<shape>(NPN, {0,0}, 2),
        },

        // Direction::Right
        Quad{
            make_vtx<shape>(PNP, {0,1}, 3),
            make_vtx<shape>(PNN, {1,1}, 3),
            make_vtx<shape>(PPN, {1,0}, 3),
            make_vtx<shape>(PPP, {0,0}, 3),
        },
        // Direction::Down
        Quad{
            make_vtx<shape>(NNN, {0,0}, 4),
            make_vtx<shape>(PNN, {1,0}, 4),
            make_vtx<shape>(PNP, {1,1}, 4),
            make_vtx<shape>(NNP, {0,1}, 4),
        },
        // Direction::Up
        Quad{
            make_vtx<shape>(NPP, {0,0}, 5),
            make_vtx<shape>(PPP, {1,0}, 5),
            make_vtx<shape>(PPN, {1,1}, 5),
            make_vtx<shape>(NPN, {0,1}, 5),
        },
    };
}
// NOTE: I forgot how this works once so im writing it out here properly so i cant forget again
// We use an index buffer, which is basically just a memory conscious way to define the 'order' of vertices in some 
// set of triangles.
// ---
// The vertices we defined in the `cube_vertices` array contains each corner of each quad that forms a cube.
// ---
// For each of these quads, or `Face`'s, the corners are in the following order:
// (CCW winding:)
//   3   <━━    2
//   ┏━━━━━━━━━━┓
//   ┃ ╲        ┃
//  ┃┃   ╲      ┃ Λ
//  V┃     ╲    ┃ ┃
//   ┃       ╲  ┃
//   ┗━━━━━━━━━━┛
//   0   ━━>    1
//---
// (In case the unicode is weird, the bottom left=0, bottom right=1, top right=2, and top left=3)
// 
//
// The INDICES that we define below describe TWO triangles, which make up our 'unit quad' so to speak.
// The first 3 vertices define the bottom left triangle ( 3, 0, 1 ), and the latter 3 define the bottom right.
// The last connection is implied by openGL spec, i.e it is assumed that the last vertex connects to the first, importantly FOR EACH TRIANGLE. Not for each quad, FOR EACH TRIANGLE.
//
// First triangle (bot left)
// 3 ━━> 0 ━━> 1 ┅┅> 3
//               ^
//            (implied)
//
// Second triangle (top right)
// 1 ━━> 2 ━━> 3 ┅┅> 1
//               ^
//            (implied)
//
// NOTE: 
// Important stuff summary:
// -> indices 0,1,2,3 describe corners
// -> Winding order is CCW for the triangles, and also for the indices.
// -> quad_indices describes two SEPARATE triangles.
//
inline constexpr std::array<u32,INDICES_PER_QUAD> quad_indices{
    { 3, 0, 1, 1, 2, 3,},
};
//static_assert(cube_vtx::quads.size()==6, "Cube must have 6 faces");
//static_assert(cross_vtx::quads.size()==4, "Cross must have 4 faces");

namespace cactus_vtx{
    static constexpr inline BlockShape shape = BlockShape::CACTUS;
    static inline constexpr f32 P = +1.0f;
    static inline constexpr f32 Z = 0.0f;
    static inline constexpr f32 HP = +0.9375;
    static inline constexpr f32 HZ = +0.0625;
    static inline constexpr glm::vec3 PPP{ P, P, P};

    inline const ShapeQuadList<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            make_vtx<shape>({P,Z,HZ}, {0,1}, 0),
            make_vtx<shape>({Z,Z,HZ}, {1,1}, 0),
            make_vtx<shape>({Z,P,HZ}, {1,0}, 0),
            make_vtx<shape>({P,P,HZ}, {0,0}, 0),
        },
        // Direction::Backward
        Quad{
            make_vtx<shape>({Z,Z,HP}, {1,1}, 1),
            make_vtx<shape>({P,Z,HP}, {0,1}, 1),
            make_vtx<shape>({P,P,HP}, {0,0}, 1),
            make_vtx<shape>({Z,P,HP}, {1,0}, 1),
        },
        // Direction:: Left
        Quad{
            make_vtx<shape>({HZ,Z,Z}, {0,1}, 2),
            make_vtx<shape>({HZ,Z,P}, {1,1}, 2),
            make_vtx<shape>({HZ,P,P}, {1,0}, 2),
            make_vtx<shape>({HZ,P,Z}, {0,0}, 2),
        },

        // Directio{n,:,:}Right
        Quad{
            make_vtx<shape>({HP,Z,P}, {0,1}, 3),
            make_vtx<shape>({HP,Z,Z}, {1,1}, 3),
            make_vtx<shape>({HP,P,Z}, {1,0}, 3),
            make_vtx<shape>({HP,P,P}, {0,0}, 3),
        },
        // Directio{n,:,:}Down
        Quad{
            make_vtx<shape>({Z,HZ,Z}, {0,0}, 4),
            make_vtx<shape>({P,HZ,Z}, {1,0}, 4),
            make_vtx<shape>({P,HZ,P}, {1,1}, 4),
            make_vtx<shape>({Z,HZ,P}, {0,1}, 4),
        },
        // Directio{n,:,:Up
        Quad{
            make_vtx<shape>({Z,HP,P}, {0,0}, 5),
            make_vtx<shape>({P,HP,P}, {1,0}, 5),
            make_vtx<shape>({P,HP,Z}, {1,1}, 5),
            make_vtx<shape>({Z,HP,Z}, {0,1}, 5),
        },
    };
}

namespace lower_half_slab_vtx{
    static constexpr inline BlockShape shape = BlockShape::BOT_HALF_SLAB;
    static inline constexpr f32 P = +1.0f;
    static inline constexpr f32 Z = 0.0f;
    static inline constexpr f32 H = +0.5f;

    inline const ShapeQuadList<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            make_vtx<shape>({P,Z,Z},{0, 1},0),
            make_vtx<shape>({Z,Z,Z}, {1, 1}, 0),
            make_vtx<shape>({Z,H,Z}, {1,.5}, 0),
            make_vtx<shape>({P,H,Z}, {0,.5}, 0),
        },
        // Direction::Backward
        Quad{
            make_vtx<shape>({Z,Z,P}, {1, 1}, 1),
            make_vtx<shape>({P,Z,P}, {0, 1}, 1),
            make_vtx<shape>({P,H,P}, {0,.5}, 1),
            make_vtx<shape>({Z,H,P}, {1,.5}, 1),
        },
        // Direction:: Left
        Quad{
            make_vtx<shape>({Z,Z,Z}, {0, 1}, 2),
            make_vtx<shape>({Z,Z,P}, {1, 1}, 2),
            make_vtx<shape>({Z,H,P}, {1,.5}, 2),
            make_vtx<shape>({Z,H,Z}, {0,.5}, 2),
        },

        // Directio{n,:,:}Right
        Quad{
            make_vtx<shape>({P,Z,P}, {0, 1}, 3),
            make_vtx<shape>({P,Z,Z}, {1, 1}, 3),
            make_vtx<shape>({P,H,Z}, {1,.5}, 3),
            make_vtx<shape>({P,H,P}, {0,.5}, 3),
        },
        // Directio{n,:,:}Down
        Quad{
            make_vtx<shape>({Z,Z,Z}, {0, 0}, 4),
            make_vtx<shape>({P,Z,Z}, {1, 0}, 4),
            make_vtx<shape>({P,Z,P}, {1, 1}, 4),
            make_vtx<shape>({Z,Z,P}, {0, 1}, 4),
        },
        // Directio{n,:,:Up
        Quad{
            make_vtx<shape>({Z,H,P}, {0, 0}, 5),
            make_vtx<shape>({P,H,P}, {1, 0}, 5),
            make_vtx<shape>({P,H,Z}, {1, 1}, 5),
            make_vtx<shape>({Z,H,Z}, {0, 1}, 5),
        },
    };
}
namespace top_half_slab_vtx{
    static constexpr inline BlockShape shape = BlockShape::TOP_HALF_SLAB;
    static inline constexpr f32 P = +1.0f;
    static inline constexpr f32 Z = 0.0f;
    static inline constexpr f32 H = +0.5f;

    inline const ShapeQuadList<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            make_vtx<shape>({P,H,Z}, {0, 1}, 0),
            make_vtx<shape>({Z,H,Z}, {1, 1}, 0),
            make_vtx<shape>({Z,P,Z}, {1,.5}, 0),
            make_vtx<shape>({P,P,Z}, {0,.5}, 0),
        },
        // Direction::Backward
        Quad{
            make_vtx<shape>({Z,H,P}, {1, 1}, 1),
            make_vtx<shape>({P,H,P}, {0, 1}, 1),
            make_vtx<shape>({P,P,P}, {0,.5}, 1),
            make_vtx<shape>({Z,P,P}, {1,.5}, 1),
        },
        // Direction:: Left
        Quad{
            make_vtx<shape>({Z,H,Z}, {0, 1}, 2),
            make_vtx<shape>({Z,H,P}, {1, 1}, 2),
            make_vtx<shape>({Z,P,P}, {1,.5}, 2),
            make_vtx<shape>({Z,P,Z}, {0,.5}, 2),
        },

        // Directio{n,:,:}Right
        Quad{
            make_vtx<shape>({P,H,P}, {0, 1}, 3),
            make_vtx<shape>({P,H,Z}, {1, 1}, 3),
            make_vtx<shape>({P,P,Z}, {1,.5}, 3),
            make_vtx<shape>({P,P,P}, {0,.5}, 3),
        },
        // Directio{n,:,:}Down
        Quad{
            make_vtx<shape>({Z,H,Z}, {0, 0}, 4),
            make_vtx<shape>({P,H,Z}, {1, 0}, 4),
            make_vtx<shape>({P,H,P}, {1, 1}, 4),
            make_vtx<shape>({Z,H,P}, {0, 1}, 4),
        },
        // Directio{n,:,:Up
        Quad{
            make_vtx<shape>({Z,P,P}, {0, 0}, 5),
            make_vtx<shape>({P,P,P}, {1, 0}, 5),
            make_vtx<shape>({P,P,Z}, {1, 1}, 5),
            make_vtx<shape>({Z,P,Z}, {0, 1}, 5),
        },
    };
}
// 1,2,...,15, (16 is just a full block)
namespace snow_vtx{

static inline constexpr f32 P = +1.0f;
static inline constexpr f32 Z = 0.0f;

template<size_t N>
static consteval inline Vertex make_snow_vtx(glm::vec3 pos, glm::vec2 uv, i32 dir){
    return make_vtx<shape_of_snow_level<N>>(pos, uv, dir);
}
template<size_t N, f32 PY=(N/16.0f)>
static constexpr inline ShapeQuadList make_snow_quadlist{
        // Direction::forward
        Quad{
            make_snow_vtx<N>({P,Z,Z},  {0,  PY}, 0),
            make_snow_vtx<N>({Z,Z,Z},  {1,  PY}, 0),
            make_snow_vtx<N>({Z,PY,Z}, {1, PY}, 0),
            make_snow_vtx<N>({P,PY,Z}, {0, PY}, 0),
        },
        // Direction::Backward
        Quad{
            make_snow_vtx<N>({Z,Z,P}, {1,  PY}, 1),
            make_snow_vtx<N>({P,Z,P}, {0,  PY}, 1),
            make_snow_vtx<N>({P,PY,P}, {0, PY}, 1),
            make_snow_vtx<N>({Z,PY,P}, {1, PY}, 1),
        },
        // Direction:: Left
        Quad{
            make_snow_vtx<N>({Z,Z,Z}, {0,  PY}, 2),
            make_snow_vtx<N>({Z,Z,P}, {1,  PY}, 2),
            make_snow_vtx<N>({Z,PY,P}, {1, PY}, 2),
            make_snow_vtx<N>({Z,PY,Z}, {0, PY}, 2),
        },

        // Directio{n,:,:}Right
        Quad{
            make_snow_vtx<N>({P,Z,P},  {0, PY}, 3),
            make_snow_vtx<N>({P,Z,Z},  {1, PY}, 3),
            make_snow_vtx<N>({P,PY,Z}, {1, PY}, 3),
            make_snow_vtx<N>({P,PY,P}, {0, PY}, 3),
        },
        // Directio{n,:,:}Down
        Quad{
            make_snow_vtx<N>({Z,Z,Z}, {0, 0}, 4),
            make_snow_vtx<N>({P,Z,Z}, {1, 0}, 4),
            make_snow_vtx<N>({P,Z,P}, {1, 1}, 4),
            make_snow_vtx<N>({Z,Z,P}, {0, 1}, 4),
        },
        // Directio{n,:,:Up
        Quad{
            make_snow_vtx<N>({Z,PY,P}, {0, 0}, 5),
            make_snow_vtx<N>({P,PY,P}, {1, 0}, 5),
            make_snow_vtx<N>({P,PY,Z}, {1, 1}, 5),
            make_snow_vtx<N>({Z,PY,Z}, {0, 1}, 5),
        },
};


inline constexpr std::array<ShapeQuadList<QUADS_PER_CUBE>,15> quad_n{
    make_snow_quadlist<1>, // SNOW_1
    make_snow_quadlist<2>, // SNOW_2
    make_snow_quadlist<3>,
    make_snow_quadlist<4>, // SNOW_4
    make_snow_quadlist<5>,
    make_snow_quadlist<6>,
    make_snow_quadlist<7>,
    make_snow_quadlist<8>,
    make_snow_quadlist<9>,
    make_snow_quadlist<10>,
    make_snow_quadlist<11>,
    make_snow_quadlist<12>,
    make_snow_quadlist<13>,
    make_snow_quadlist<14>,
    make_snow_quadlist<15>,
};

} // NAMESPACE snow_vtx
