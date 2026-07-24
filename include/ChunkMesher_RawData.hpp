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
            Vertex{NNP, {0,1}, 0,shape},
            Vertex{PNN, {1,1}, 0,shape},
            Vertex{PPN, {1,0}, 0,shape},
            Vertex{NPP, {0,0}, 0,shape},
        },
        Quad{
            Vertex{PNN, {1,1}, 0,shape},
            Vertex{NNP, {0,1}, 0,shape},
            Vertex{NPP, {0,0}, 0,shape},
            Vertex{PPN, {1,0}, 0,shape},
        },
        // Red
        Quad{
            Vertex{NNN, {0,1}, 1,shape},
            Vertex{PNP, {1,1}, 1,shape},
            Vertex{PPP, {1,0}, 1,shape},
            Vertex{NPN, {0,0}, 1,shape},
        },
        Quad{
            Vertex{PNP, {1,1}, 1,shape},
            Vertex{NNN, {0,1}, 1,shape},
            Vertex{NPN, {0,0}, 1,shape},
            Vertex{PPP, {1,0}, 1,shape},
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
            Vertex{PNN, {0,1}, 0,shape},
            Vertex{NNN, {1,1}, 0,shape},
            Vertex{NPN, {1,0}, 0,shape},
            Vertex{PPN, {0,0}, 0,shape},
        },
        // Direction::Backward
        Quad{
            Vertex{NNP, {0,1}, 1,shape},
            Vertex{PNP, {1,1}, 1,shape},
            Vertex{PPP, {1,0}, 1,shape},
            Vertex{NPP, {0,0}, 1,shape},
        },
        // Direction:: Left
        Quad{
            Vertex{NNN, {0,1}, 2,shape},
            Vertex{NNP, {1,1}, 2,shape},
            Vertex{NPP, {1,0}, 2,shape},
            Vertex{NPN, {0,0}, 2,shape},
        },

        // Direction::Right
        Quad{
            Vertex{PNP, {0,1}, 3,shape},
            Vertex{PNN, {1,1}, 3,shape},
            Vertex{PPN, {1,0}, 3,shape},
            Vertex{PPP, {0,0}, 3,shape},
        },
        // Direction::Down
        Quad{
            Vertex{NNN, {0,0}, 4,shape},
            Vertex{PNN, {1,0}, 4,shape},
            Vertex{PNP, {1,1}, 4,shape},
            Vertex{NNP, {0,1}, 4,shape},
        },
        // Direction::Up
        Quad{
            Vertex{NPP, {0,0}, 5,shape},
            Vertex{PPP, {1,0}, 5,shape},
            Vertex{PPN, {1,1}, 5,shape},
            Vertex{NPN, {0,1}, 5,shape},
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
            Vertex{{P,Z,HZ}, {0,1}, 0,shape},
            Vertex{{Z,Z,HZ}, {1,1}, 0,shape},
            Vertex{{Z,P,HZ}, {1,0}, 0,shape},
            Vertex{{P,P,HZ}, {0,0}, 0,shape},
        },
        // Direction::Backward
        Quad{
            Vertex{{Z,Z,HP}, {1,1}, 1,shape},
            Vertex{{P,Z,HP}, {0,1}, 1,shape},
            Vertex{{P,P,HP}, {0,0}, 1,shape},
            Vertex{{Z,P,HP}, {1,0}, 1,shape},
        },
        // Direction:: Left
        Quad{
            Vertex{{HZ,Z,Z}, {0,1}, 2,shape},
            Vertex{{HZ,Z,P}, {1,1}, 2,shape},
            Vertex{{HZ,P,P}, {1,0}, 2,shape},
            Vertex{{HZ,P,Z}, {0,0}, 2,shape},
        },

        // Directio{n,:,:}Right
        Quad{
            Vertex{{HP,Z,P}, {0,1}, 3,shape},
            Vertex{{HP,Z,Z}, {1,1}, 3,shape},
            Vertex{{HP,P,Z}, {1,0}, 3,shape},
            Vertex{{HP,P,P}, {0,0}, 3,shape},
        },
        // Directio{n,:,:}Down
        Quad{
            Vertex{{Z,HZ,Z}, {0,0}, 4,shape},
            Vertex{{P,HZ,Z}, {1,0}, 4,shape},
            Vertex{{P,HZ,P}, {1,1}, 4,shape},
            Vertex{{Z,HZ,P}, {0,1}, 4,shape},
        },
        // Directio{n,:,:Up
        Quad{
            Vertex{{Z,HP,P}, {0,0}, 5,shape},
            Vertex{{P,HP,P}, {1,0}, 5,shape},
            Vertex{{P,HP,Z}, {1,1}, 5,shape},
            Vertex{{Z,HP,Z}, {0,1}, 5,shape},
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
            Vertex{{P,Z,Z},{0, 1},0,shape},
            Vertex{{Z,Z,Z}, {1, 1}, 0,shape},
            Vertex{{Z,H,Z}, {1,.5}, 0,shape},
            Vertex{{P,H,Z}, {0,.5}, 0,shape},
        },
        // Direction::Backward
        Quad{
            Vertex{{Z,Z,P}, {1, 1}, 1,shape},
            Vertex{{P,Z,P}, {0, 1}, 1,shape},
            Vertex{{P,H,P}, {0,.5}, 1,shape},
            Vertex{{Z,H,P}, {1,.5}, 1,shape},
        },
        // Direction:: Left
        Quad{
            Vertex{{Z,Z,Z}, {0, 1}, 2,shape},
            Vertex{{Z,Z,P}, {1, 1}, 2,shape},
            Vertex{{Z,H,P}, {1,.5}, 2,shape},
            Vertex{{Z,H,Z}, {0,.5}, 2,shape},
        },

        // Directio{n,:,:}Right
        Quad{
            Vertex{{P,Z,P}, {0, 1}, 3,shape},
            Vertex{{P,Z,Z}, {1, 1}, 3,shape},
            Vertex{{P,H,Z}, {1,.5}, 3,shape},
            Vertex{{P,H,P}, {0,.5}, 3,shape},
        },
        // Directio{n,:,:}Down
        Quad{
            Vertex{{Z,Z,Z}, {0, 0}, 4,shape},
            Vertex{{P,Z,Z}, {1, 0}, 4,shape},
            Vertex{{P,Z,P}, {1, 1}, 4,shape},
            Vertex{{Z,Z,P}, {0, 1}, 4,shape},
        },
        // Directio{n,:,:Up
        Quad{
            Vertex{{Z,H,P}, {0, 0}, 5,shape},
            Vertex{{P,H,P}, {1, 0}, 5,shape},
            Vertex{{P,H,Z}, {1, 1}, 5,shape},
            Vertex{{Z,H,Z}, {0, 1}, 5,shape},
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
            Vertex{{P,H,Z}, {0, 1}, 0, shape},
            Vertex{{Z,H,Z}, {1, 1}, 0, shape},
            Vertex{{Z,P,Z}, {1,.5}, 0, shape},
            Vertex{{P,P,Z}, {0,.5}, 0, shape},
        },
        // Direction::Backward
        Quad{
            Vertex{{Z,H,P}, {1, 1}, 1, shape},
            Vertex{{P,H,P}, {0, 1}, 1, shape},
            Vertex{{P,P,P}, {0,.5}, 1, shape},
            Vertex{{Z,P,P}, {1,.5}, 1, shape},
        },
        // Direction:: Left
        Quad{
            Vertex{{Z,H,Z}, {0, 1}, 2, shape},
            Vertex{{Z,H,P}, {1, 1}, 2, shape},
            Vertex{{Z,P,P}, {1,.5}, 2, shape},
            Vertex{{Z,P,Z}, {0,.5}, 2, shape},
        },

        // Directio{n,:,:}Right
        Quad{
            Vertex{{P,H,P}, {0, 1}, 3, shape},
            Vertex{{P,H,Z}, {1, 1}, 3, shape},
            Vertex{{P,P,Z}, {1,.5}, 3, shape},
            Vertex{{P,P,P}, {0,.5}, 3, shape},
        },
        // Directio{n,:,:}Down
        Quad{
            Vertex{{Z,H,Z}, {0, 0}, 4, shape},
            Vertex{{P,H,Z}, {1, 0}, 4, shape},
            Vertex{{P,H,P}, {1, 1}, 4, shape},
            Vertex{{Z,H,P}, {0, 1}, 4, shape},
        },
        // Directio{n,:,:Up
        Quad{
            Vertex{{Z,P,P}, {0, 0}, 5, shape},
            Vertex{{P,P,P}, {1, 0}, 5, shape},
            Vertex{{P,P,Z}, {1, 1}, 5, shape},
            Vertex{{Z,P,Z}, {0, 1}, 5, shape},
        },
    };
}
// 1,2,...,15, (16 is just a full block)
namespace snow_vtx{

static inline constexpr f32 P = +1.0f;
static inline constexpr f32 Z = 0.0f;

template<size_t N>
static constexpr inline Vertex make_snow_vtx(glm::vec3 pos, glm::vec2 uv, i32 dir){
    return Vertex{ pos, uv, dir,shape_of_snow_level<N>};
}
template<size_t N, f32 PY=(N/16.0f)>
static constexpr inline ShapeQuadList make_snow_quadlist{
        // Direction::forward
        Quad{
            make_snow_vtx<N>({P,Z,Z}, {0,  PY}, 0),
            make_snow_vtx<N>({Z,Z,Z}, {1,  PY}, 0),
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
