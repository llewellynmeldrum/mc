#pragma once

#include "glmWrapper.hpp"
#include "glm/ext/vector_int3.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/hash.hpp"

#include "cppslop.hpp"

#include "LM.hpp"

#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkConstants.hpp"


struct ChunkOffsetTag {};
struct BlockOffsetTag {};
struct WorldChunkCoordTag {};
struct WorldBlockPosTag {};
struct ChunkBlockPosTag {};
struct FloatOffsetTag {};
struct WorldFloatPosTag {};



template <typename Tag, typename ScalarType>
struct Coord3 {
    constexpr static size_t Extent = 3;
    using glm_vec_type = glm::vec<Extent, ScalarType>;
    using value_type = ScalarType;
    using type = Coord3<Tag, ScalarType>;

    ScalarType x{}, y{}, z{};
    constexpr Coord3() = default;
    constexpr ~Coord3() = default;
    constexpr Coord3(Coord3&& rhs) = default;
    constexpr Coord3(const Coord3& rhs) = default;
    constexpr Coord3& operator=(const Coord3& rhs) = default;
    constexpr Coord3& operator=(Coord3&& rhs) = default;

    constexpr Coord3(ScalarType all) noexcept : x(all), y(all), z(all) {}
    constexpr Coord3(ScalarType _x, ScalarType _y, ScalarType _z) noexcept : x(_x), y(_y), z(_z) {}

    constexpr decltype(auto) operator[](this auto& self, size_t idx) {
        switch (idx) {
        case 0:
            return self.x;
            break;
        case 1:
            return self.y;
            break;
        case 2:
            return self.z;
            break;
        }
        throw std::out_of_range("Error: index must be >=0 && <=2 ");
    }
    explicit constexpr Coord3(glm_vec_type v) noexcept : x(v.x), y(v.y), z(v.z) {}

    constexpr glm_vec_type  raw() const noexcept { return glm_vec_type{ x, y, z }; };

    friend constexpr bool operator==(Coord3 lhs, Coord3 rhs) = default;
    constexpr auto        operator<=>(const Coord3& rhs) const = default;
};

template <typename Tag, typename ScalarType>
struct Coord2 {
    constexpr static size_t Extent = 2;
    using glm_vec_type = glm::vec<Extent, ScalarType, glm::defaultp>;
    using value_type = ScalarType;
    using type = Coord2<Tag, ScalarType>;
    ScalarType x{}, z{};
    constexpr Coord2() = default;
    constexpr ~Coord2() = default;
    constexpr Coord2(Coord2&& rhs) = default;
    constexpr Coord2(const Coord2& rhs) = default;
    constexpr Coord2& operator=(const Coord2& rhs) = default;
    constexpr Coord2& operator=(Coord2&& rhs) = default;

    constexpr Coord2(ScalarType all) noexcept : x(all), z(all) {}
    constexpr Coord2(ScalarType _x, ScalarType _z) noexcept : x(_x), z(_z) {}

    constexpr decltype(auto) operator[](this auto& self, size_t idx) {
        switch (idx) {
        case 0:
            return self.x;
            break;
        case 1:
            return self.z;
            break;
        default:
            throw std::out_of_range("Error: index must be >=0 && <=1 ");
            break;
        }
    }
    explicit constexpr Coord2(glm_vec_type v) noexcept : x(v.x), z(v.y) {}

    constexpr glm_vec_type  raw() const noexcept { return glm_vec_type{ x, z }; };
    constexpr glm_vec_type& rawref() const noexcept { return glm_vec_type{ x, z }; };

    friend constexpr bool operator==(Coord2 lhs, Coord2 rhs) = default;
    constexpr auto        operator<=>(const Coord2& rhs) const = default;
};

// used with worldChunkCoord to index into adjacent chunks
using ChunkOffset = Coord2<ChunkOffsetTag, i32>;
// Identifies a CHUNK in the WORLD. (world space)
using WorldChunkCoord = Coord2<WorldChunkCoordTag, i32>;

// used with WorldBlockPos & chunkBlockPos to index into nearby blocks
using BlockOffset = Coord3<BlockOffsetTag, i32>;
// Identifies a BLOCK in the WORLD. (world space)
using WorldBlockPos = Coord3<WorldBlockPosTag, i32>;
// Identifies a BLOCK in a CHUNK. (chunk local space)
using ChunkBlockPos = Coord3<ChunkBlockPosTag, i32>;
// Identifies some position that may be inbetween blocks
using WorldFloatPos = Coord3<WorldFloatPosTag, f32>;
using FloatOffset = Coord3<FloatOffsetTag, f32>;


namespace std {
template <typename tag, typename datatype>
struct hash<Coord3<tag,datatype>> {
    using glm_vec_type = Coord3<tag,datatype>::glm_vec_type;
    auto operator()(const WorldChunkCoord& val) const noexcept {
        return std::hash<glm_vec_type>{}(val.raw());
    };
};
template <typename tag, typename datatype>
struct hash<Coord2<tag,datatype>> {
    using glm_vec_type = Coord2<tag,datatype>::glm_vec_type;
    auto operator()(const WorldChunkCoord& val) const noexcept {
        return std::hash<glm_vec_type>{}(val.raw());
    };
};
};  // namespace std

// ============================
// OPERATOR OVERLOADS
// ============================
#define DECL_ADD_OPER(T1, T2)                                                                      \
    constexpr T1 operator+(T1 pos, T2 offset) {                                                    \
        return T1{ pos.raw() + static_cast<T1::glm_vec_type>(offset.raw()) };                           \
    }                                                                                              \
    constexpr T1 operator+=(T1& pos, T2 offset) {                                                  \
        return pos = { T1{ pos.raw() + static_cast<T1::glm_vec_type>(offset.raw()) } };                 \
    }

#define DECL_SUB_OPER(T1, T2)                                                                      \
    constexpr T1 operator-(T1 pos, T2 offset) {                                                    \
        return T1{ pos.raw() - static_cast<T1::glm_vec_type>(offset.raw()) };                           \
    }                                                                                              \
    constexpr T1 operator-=(T1& pos, T2 offset) {                                                  \
        return pos = { T1{ pos.raw() - static_cast<T1::glm_vec_type>(offset.raw()) } };                 \
    }

// NOTE: These operations perform casts to transform the second value into the first.
#define DECL_ADD_SUB_OPER(T1, T2)                                                                  \
    DECL_ADD_OPER(T1, T2)                                                                          \
                                                                                                   \
    DECL_SUB_OPER(T1, T2)

// You can offset a position by a block offset
DECL_ADD_SUB_OPER(WorldFloatPos, BlockOffset)
// You can offset a position by a float offset.
DECL_ADD_SUB_OPER(WorldFloatPos, FloatOffset)
// You can offset a position by a chunk offset.
// DECL_ADD_SUB_OPER(WorldFloatPos,ChunkOffset)
constexpr WorldFloatPos operator+(WorldFloatPos pos, ChunkOffset offset) {
    return WorldFloatPos{ pos.raw() + glm::vec3{offset.x,offset.z, 0}};
}
constexpr WorldFloatPos operator+=(WorldFloatPos& pos, ChunkOffset offset) {
    return pos = { WorldFloatPos{ pos.raw() +glm::vec3{offset.x,offset.z, 0}}};
}
constexpr WorldFloatPos operator-(WorldFloatPos pos, ChunkOffset offset) {
    return WorldFloatPos{ pos.raw() - glm::vec3{offset.x,offset.z, 0}};
}
constexpr WorldFloatPos operator-=(WorldFloatPos& pos, ChunkOffset offset) {
    return pos = { WorldFloatPos{ pos.raw() -glm::vec3{offset.x,offset.z, 0}}};
}

// You can offset a block coord by a chunk offset.
//DECL_ADD_SUB_OPER(WorldBlockPos, ChunkOffset)
constexpr WorldBlockPos operator+(WorldBlockPos pos, ChunkOffset offset) {
    return WorldBlockPos{ pos.raw() + glm::ivec3{offset.x, 0, offset.z}};
}
constexpr WorldBlockPos operator+=(WorldBlockPos& pos, ChunkOffset offset) {
    return pos =  WorldBlockPos{ pos.raw() + glm::ivec3{offset.x, 0, offset.z}} ;
}
constexpr WorldBlockPos operator-(WorldBlockPos pos, ChunkOffset offset) {
    return WorldBlockPos{ pos.raw() - glm::ivec3{offset.x, 0, offset.z}};
}
constexpr WorldBlockPos operator-=(WorldBlockPos& pos, ChunkOffset offset) {
    return pos =  WorldBlockPos{pos.raw() - glm::ivec3{offset.x, 0, offset.z}} ;
}

// You can offset a block coord by a block offset
DECL_ADD_SUB_OPER(WorldBlockPos, BlockOffset)

// You can offset a chunk local block pos by a block offset.
DECL_ADD_SUB_OPER(ChunkBlockPos, BlockOffset)

// You can offset a chunk coord by a chunk offset.
DECL_ADD_SUB_OPER(WorldChunkCoord, ChunkOffset)

DECL_ADD_SUB_OPER(ChunkBlockPos, ChunkBlockPos)


// ======================
// CONVERSION FUNCTIONS
// ======================
// worldChunkCoord -> worldBlockPos
// Returns the world pos of the block at [0,0,0] in the chunk identified by wChunkCoord
inline constexpr auto toWorldOrigin(WorldChunkCoord chunk) -> WorldBlockPos {
    return WorldBlockPos{
        chunk.x  * ChunkInfo::XWIDTH,
        0,
        chunk.z  * ChunkInfo::XWIDTH,
    };
}

// chunkBlockPos->worldBlockPos
// Returns the world pos of the block at [cBlockPos] in the chunk identified by wChunkCoord
inline constexpr auto toWorldBlockPos(WorldChunkCoord chunk, ChunkBlockPos local) -> WorldBlockPos {
    return WorldBlockPos{
        chunk.x  * ChunkInfo::XWIDTH + local.x,
        local.y,
        chunk.z  * ChunkInfo::XWIDTH + local.z,
    };
}

// chunkBlockPos->worldBlockPos
// Returns the world pos of the block at [cBlockPos] in the chunk identified by wChunkCoord
inline constexpr auto toWorldBlockPos(WorldChunkCoord chunk, BlockOffset local) -> WorldBlockPos {
    return WorldBlockPos{ toWorldOrigin(chunk) + local };
}

// Floatpos->worldBlockPos
// Rounds to the correct block
inline constexpr auto toWorldBlockPos(WorldFloatPos pos) -> WorldBlockPos {
    return WorldBlockPos{ glm::floor(pos.raw()) };
}
inline constexpr auto toWorldFloatPos(WorldBlockPos pos) -> WorldFloatPos {
    return WorldFloatPos{ pos.raw() };
}


// worldBlockPos->      chunkBlockPos
// returns the chunk local pos, in the chunk within wBlockPos, of the block at [wBlockPos] in the
// world
inline constexpr auto toChunkBlockPos(WorldFloatPos p) -> ChunkBlockPos {
    return ChunkBlockPos{
        static_cast<i32>(LM::euclid_mod(p.x,ChunkInfo::XWIDTH)),
        static_cast<i32>(p.y),
        static_cast<i32>(LM::euclid_mod(p.z,ChunkInfo::ZWIDTH)),
    };
}
inline constexpr auto toChunkBlockPos(WorldBlockPos p) -> ChunkBlockPos {
    return ChunkBlockPos{
        LM::ieuclid_mod(p.x,ChunkInfo::XWIDTH),
        p.y,
        LM::ieuclid_mod(p.z,ChunkInfo::ZWIDTH)
    };
}

inline constexpr auto toWorldChunkCoord(WorldBlockPos block) -> WorldChunkCoord {
    return WorldChunkCoord{
        LM::floor_div(block.x,ChunkInfo::XWIDTH),
        LM::floor_div(block.z,ChunkInfo::ZWIDTH)
    };
}

inline constexpr auto toWorldChunkCoord(WorldFloatPos pos) -> WorldChunkCoord {
    return WorldChunkCoord{
        static_cast<i32>(LM::floor_div(pos.x,ChunkInfo::XWIDTH)),
        static_cast<i32>(LM::floor_div(pos.z,ChunkInfo::ZWIDTH))
    };
}





// SECTION: CONVERSION
// SUBSECTION: ALIASES

inline constexpr auto toChunkLocal(WorldFloatPos p) -> ChunkBlockPos {
    return toChunkBlockPos(p);
}
inline constexpr auto toChunkLocal(WorldBlockPos p) -> ChunkBlockPos {
    return toChunkBlockPos(p);
}





// SECTION: ASSERTIONS
#include "NumericConcepts.hpp"
static_assert(!is_fvec2<ChunkOffset>);
static_assert(!is_fvec3<ChunkOffset>);
static_assert(!is_fvec4<ChunkOffset>);
static_assert( is_ivec2<ChunkOffset>);
static_assert(!is_ivec3<ChunkOffset>);
static_assert(!is_ivec4<ChunkOffset>);

static_assert(!is_fvec2<WorldChunkCoord>);
static_assert(!is_fvec3<WorldChunkCoord>);
static_assert(!is_fvec4<WorldChunkCoord>);
static_assert( is_ivec2<WorldChunkCoord>);
static_assert(!is_ivec3<WorldChunkCoord>);
static_assert(!is_ivec4<WorldChunkCoord>);

static_assert(!is_fvec2<WorldBlockPos>);
static_assert(!is_fvec3<WorldBlockPos>);
static_assert(!is_fvec4<WorldBlockPos>);
static_assert(!is_ivec2<WorldBlockPos>);
static_assert( is_ivec3<WorldBlockPos>);
static_assert(!is_ivec4<WorldBlockPos>);

static_assert(!is_fvec2<ChunkBlockPos>);
static_assert(!is_fvec3<ChunkBlockPos>);
static_assert(!is_fvec4<ChunkBlockPos>);
static_assert(!is_ivec2<ChunkBlockPos>);
static_assert( is_ivec3<ChunkBlockPos>);
static_assert(!is_ivec4<ChunkBlockPos>);

static_assert(!is_fvec2<WorldFloatPos>);
static_assert( is_fvec3<WorldFloatPos>);
static_assert(!is_fvec4<WorldFloatPos>);
static_assert(!is_ivec2<WorldFloatPos>);
static_assert(!is_ivec3<WorldFloatPos>);
static_assert(!is_ivec4<WorldFloatPos>);

static_assert(!is_fvec2<FloatOffset>);
static_assert( is_fvec3<FloatOffset>);
static_assert(!is_fvec4<FloatOffset>);
static_assert(!is_ivec2<FloatOffset>);
static_assert(!is_ivec3<FloatOffset>);
static_assert(!is_ivec4<FloatOffset>);

