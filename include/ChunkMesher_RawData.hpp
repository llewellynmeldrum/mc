#pragma once 
// ===========
// Mesh_RawData.hpp
// ===========
#include "ChunkConstants.hpp"
#include "Vertex.hpp"
using QuadVertexData = std::array<Vertex, VTX_PER_QUAD>;

template<size_t T>
using ShapeQuads = std::array<QuadVertexData, T>;

// clang-format off
using Quad = std::array<Vertex,VTX_PER_QUAD>;
namespace cross_vtx{
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
    inline constexpr ShapeQuads<QUADS_PER_CROSS> quads = {
        // Blue 
        Quad{
            Vertex{NNP,glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{PNN,glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{PPN,glm::vec2(1,0),glm::vec4(0.0f),0},
            Vertex{NPP,glm::vec2(0,0),glm::vec4(0.0f),0},
        },
        Quad{
            Vertex{PNN,glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{NNP,glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{NPP,glm::vec2(0,0),glm::vec4(0.0f),0},
            Vertex{PPN,glm::vec2(1,0),glm::vec4(0.0f),0},
        },
        // Red
        Quad{
            Vertex{NNN,glm::vec2(0,1),glm::vec4(0.0f),1},
            Vertex{PNP,glm::vec2(1,1),glm::vec4(0.0f),1},
            Vertex{PPP,glm::vec2(1,0),glm::vec4(0.0f),1},
            Vertex{NPN,glm::vec2(0,0),glm::vec4(0.0f),1},
        },
        Quad{
            Vertex{PNP,glm::vec2(1,1),glm::vec4(0.0f),1},
            Vertex{NNN,glm::vec2(0,1),glm::vec4(0.0f),1},
            Vertex{NPN,glm::vec2(0,0),glm::vec4(0.0f),1},
            Vertex{PPP,glm::vec2(1,0),glm::vec4(0.0f),1},
        },
    };
}

namespace cube_vtx{
    inline constexpr glm::vec3 NNN{ 0.0,0.0,0.0};
    inline constexpr glm::vec3 NPN{ 0.0, 1.0,0.0};
    inline constexpr glm::vec3 NNP{ 0.0,0.0, 1.0};
    inline constexpr glm::vec3 PNN{ 1.0,0.0,0.0};
    inline constexpr glm::vec3 PPN{ 1.0, 1.0,0.0};
    inline constexpr glm::vec3 PNP{ 1.0,0.0, 1.0};
    inline constexpr glm::vec3 NPP{ 0.0, 1.0, 1.0};
    inline constexpr glm::vec3 PPP{ 1.0, 1.0, 1.0};

    inline constexpr ShapeQuads<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            Vertex{PNN,glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{NNN,glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{NPN,glm::vec2(1,0),glm::vec4(0.0f),0},
            Vertex{PPN,glm::vec2(0,0),glm::vec4(0.0f),0},
        },
        // Direction::Backward
        Quad{
            Vertex{NNP,glm::vec2(0,1),glm::vec4(0.0f),1},
            Vertex{PNP,glm::vec2(1,1),glm::vec4(0.0f),1},
            Vertex{PPP,glm::vec2(1,0),glm::vec4(0.0f),1},
            Vertex{NPP,glm::vec2(0,0),glm::vec4(0.0f),1},
        },
        // Direction:: Left
        Quad{
            Vertex{NNN,glm::vec2(0,1),glm::vec4(0.0f),2},
            Vertex{NNP,glm::vec2(1,1),glm::vec4(0.0f),2},
            Vertex{NPP,glm::vec2(1,0),glm::vec4(0.0f),2},
            Vertex{NPN,glm::vec2(0,0),glm::vec4(0.0f),2},
        },

        // Direction::Right
        Quad{
            Vertex{PNP,glm::vec2(0,1),glm::vec4(0.0f),3},
            Vertex{PNN,glm::vec2(1,1),glm::vec4(0.0f),3},
            Vertex{PPN,glm::vec2(1,0),glm::vec4(0.0f),3},
            Vertex{PPP,glm::vec2(0,0),glm::vec4(0.0f),3},
        },
        // Direction::Down
        Quad{
            Vertex{NNN,glm::vec2(0,0),glm::vec4(0.0f),4},
            Vertex{PNN,glm::vec2(1,0),glm::vec4(0.0f),4},
            Vertex{PNP,glm::vec2(1,1),glm::vec4(0.0f),4},
            Vertex{NNP,glm::vec2(0,1),glm::vec4(0.0f),4},
        },
        // Direction::Up
        Quad{
            Vertex{NPP,glm::vec2(0,0),glm::vec4(0.0f),5},
            Vertex{PPP,glm::vec2(1,0),glm::vec4(0.0f),5},
            Vertex{PPN,glm::vec2(1,1),glm::vec4(0.0f),5},
            Vertex{NPN,glm::vec2(0,1),glm::vec4(0.0f),5},
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
static_assert(cube_vtx::quads.size()==6, "Cube must have 6 faces");
static_assert(cross_vtx::quads.size()==4, "Cross must have 4 faces");

namespace cactus_vtx{
    static inline constexpr f32 P = +1.0f;
    static inline constexpr f32 N = 0.0f;
    static inline constexpr f32 HP = +0.9375;
    static inline constexpr f32 HN = +0.0625;
    static inline constexpr glm::vec3 NNN{ N, N, N};
    static inline constexpr glm::vec3 NPN{ N, P, N};
    static inline constexpr glm::vec3 NNP{ N, N, P};
    static inline constexpr glm::vec3 PNN{ P, N, N};
    static inline constexpr glm::vec3 PPN{ P, P, N};
    static inline constexpr glm::vec3 PNP{ P, N, P};
    static inline constexpr glm::vec3 NPP{ N, P, P};
    static inline constexpr glm::vec3 PPP{ P, P, P};

    inline constexpr ShapeQuads<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            Vertex{{P,N,HN},glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{{N,N,HN},glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{{N,P,HN},glm::vec2(1,0),glm::vec4(0.0f),0},
            Vertex{{P,P,HN},glm::vec2(0,0),glm::vec4(0.0f),0},
        },
        // Direction::Backward
        Quad{
            Vertex{{N,N,HP},glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{{P,N,HP},glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{{P,P,HP},glm::vec2(0,0),glm::vec4(0.0f),0},
            Vertex{{N,P,HP},glm::vec2(1,0),glm::vec4(0.0f),0},
        },
        // Direction:: Left
        Quad{
            Vertex{{HN,N,N},glm::vec2(0,1),glm::vec4(0.0f),2},
            Vertex{{HN,N,P},glm::vec2(1,1),glm::vec4(0.0f),2},
            Vertex{{HN,P,P},glm::vec2(1,0),glm::vec4(0.0f),2},
            Vertex{{HN,P,N},glm::vec2(0,0),glm::vec4(0.0f),2},
        },

        // Directio{n,:,:}Right
        Quad{
            Vertex{{HP,N,P},glm::vec2(0,1),glm::vec4(0.0f),3},
            Vertex{{HP,N,N},glm::vec2(1,1),glm::vec4(0.0f),3},
            Vertex{{HP,P,N},glm::vec2(1,0),glm::vec4(0.0f),3},
            Vertex{{HP,P,P},glm::vec2(0,0),glm::vec4(0.0f),3},
        },
        // Directio{n,:,:}Down
        Quad{
            Vertex{{N,HN,N},glm::vec2(0,0),glm::vec4(0.0f),4},
            Vertex{{P,HN,N},glm::vec2(1,0),glm::vec4(0.0f),4},
            Vertex{{P,HN,P},glm::vec2(1,1),glm::vec4(0.0f),4},
            Vertex{{N,HN,P},glm::vec2(0,1),glm::vec4(0.0f),4},
        },
        // Directio{n,:,:Up
        Quad{
            Vertex{{N,HP,P},glm::vec2(0,0),glm::vec4(0.0f),5},
            Vertex{{P,HP,P},glm::vec2(1,0),glm::vec4(0.0f),5},
            Vertex{{P,HP,N},glm::vec2(1,1),glm::vec4(0.0f),5},
            Vertex{{N,HP,N},glm::vec2(0,1),glm::vec4(0.0f),5},
        },
    };
}

namespace lower_half_slab_vtx{
    static inline constexpr f32 P = +1.0f;
    static inline constexpr f32 N = 0.0f;
    static inline constexpr f32 H = +0.5f;
    static inline constexpr glm::vec3 NNN{ N, N, N};
    static inline constexpr glm::vec3 NPN{ N, P, N};
    static inline constexpr glm::vec3 NNP{ N, N, P};
    static inline constexpr glm::vec3 PNN{ P, N, N};
    static inline constexpr glm::vec3 PPN{ P, P, N};
    static inline constexpr glm::vec3 PNP{ P, N, P};
    static inline constexpr glm::vec3 NPP{ N, P, P};
    static inline constexpr glm::vec3 PPP{ P, P, P};

    inline constexpr ShapeQuads<QUADS_PER_CUBE> quads = {
        // Direction::forward
        Quad{
            Vertex{{P,N,N},glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{{N,N,N},glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{{N,H,N},glm::vec2(1,.5),glm::vec4(0.0f),0},
            Vertex{{P,H,N},glm::vec2(0,.5),glm::vec4(0.0f),0},
        },
        // Direction::Backward
        Quad{
            Vertex{{N,N,P},glm::vec2(1,1),glm::vec4(0.0f),0},
            Vertex{{P,N,P},glm::vec2(0,1),glm::vec4(0.0f),0},
            Vertex{{P,H,P},glm::vec2(0,.5),glm::vec4(0.0f),0},
            Vertex{{N,H,P},glm::vec2(1,.5),glm::vec4(0.0f),0},
        },
        // Direction:: Left
        Quad{
            Vertex{{N,N,N},glm::vec2(0,1),glm::vec4(0.0f),2},
            Vertex{{N,N,P},glm::vec2(1,1),glm::vec4(0.0f),2},
            Vertex{{N,H,P},glm::vec2(1,.5),glm::vec4(0.0f),2},
            Vertex{{N,H,N},glm::vec2(0,.5),glm::vec4(0.0f),2},
        },

        // Directio{n,:,:}Right
        Quad{
            Vertex{{P,N,P},glm::vec2(0,1),glm::vec4(0.0f),3},
            Vertex{{P,N,N},glm::vec2(1,1),glm::vec4(0.0f),3},
            Vertex{{P,H,N},glm::vec2(1,.5),glm::vec4(0.0f),3},
            Vertex{{P,H,P},glm::vec2(0,.5),glm::vec4(0.0f),3},
        },
        // Directio{n,:,:}Down
        Quad{
            Vertex{{N,N,N},glm::vec2(0,0 ),glm::vec4(0.0f),4},
            Vertex{{P,N,N},glm::vec2(1,0 ),glm::vec4(0.0f),4},
            Vertex{{P,N,P},glm::vec2(1,1),glm::vec4(0.0f),4},
            Vertex{{N,N,P},glm::vec2(0,1),glm::vec4(0.0f),4},
        },
        // Directio{n,:,:Up
        Quad{
            Vertex{{N,H,P},glm::vec2(0,0),glm::vec4(0.0f),5},
            Vertex{{P,H,P},glm::vec2(1,0),glm::vec4(0.0f),5},
            Vertex{{P,H,N},glm::vec2(1,1),glm::vec4(0.0f),5},
            Vertex{{N,H,N},glm::vec2(0,1),glm::vec4(0.0f),5},
        },
    };
}
