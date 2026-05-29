#pragma once 
#include "LM.hpp"
#include "ChunkConstants.hpp"
#include "glm/ext/vector_int3.hpp"
#include "cppslop.hpp"
#include "ChunkHelpers.hpp"
#include <optional>
struct ChunkOffsetTag{};
struct BlockOffsetTag{};
struct WorldChunkCoordTag{};
struct WorldBlockPosTag{};
struct ChunkBlockPosTag{};

struct FloatOffsetTag{};
struct WorldFloatPosTag{};

// type trait
template<typename Scalar>
struct Vector3Type;

template<>
struct Vector3Type<i32>{
    using type = glm::ivec3;
};
template<>
struct Vector3Type<f32>{
    using type = glm::vec3;
};

template<typename T>
using Vector3Type_t = Vector3Type<T>::type;

// TODO: currently making type trait thing so that coord3 can be used for floating point and integer types based on the tag and underlying type
template<typename Tag, typename ScalarType>
struct Coord3{
    using VecType = Vector3Type_t<ScalarType>;
    using value_type = ScalarType;
    using type = Coord3<Tag,ScalarType>;
    ScalarType x{}, y{}, z{};
    constexpr Coord3()=default;
    constexpr ~Coord3()=default;
    constexpr Coord3(Coord3&& rhs)=default;
    constexpr Coord3(const Coord3& rhs)=default;
    constexpr Coord3& operator=(const Coord3& rhs)=default;
    constexpr Coord3& operator=(Coord3&& rhs)=default;


    constexpr Coord3(ScalarType all) noexcept :x(all),y(all), z(all) {}
    constexpr Coord3(ScalarType _x, ScalarType _y, ScalarType _z) noexcept :x(_x),y(_y), z(_z) {}

    explicit constexpr Coord3(VecType v) noexcept :x(v.x),y(v.y), z(v.z) {}


    constexpr VecType raw() const noexcept { return VecType{x,y,z}; };
    constexpr VecType& rawref() const noexcept { return VecType{x,y,z}; };

    friend constexpr bool operator==(Coord3 lhs, Coord3 rhs)=default;
    constexpr auto operator<=>(const Coord3& rhs)const =default;
};





// used with worldChunkCoord to index into adjacent chunks
using ChunkOffset = Coord3<ChunkOffsetTag,i32>;
// used with WorldBlockPos & chunkBlockPos to index into nearby blocks
using BlockOffset = Coord3<BlockOffsetTag,i32>;
// Identifies a CHUNK in the WORLD. (world space)
using WorldChunkCoord = Coord3<WorldChunkCoordTag,i32>;
// Identifies a BLOCK in the WORLD. (world space)
using WorldBlockPos = Coord3<WorldBlockPosTag,i32>;
// Identifies a BLOCK in a CHUNK. (chunk local space)
using ChunkBlockPos = Coord3<ChunkBlockPosTag,i32>;
// Identifies some position that may be inbetween blocks
using WorldFloatPos = Coord3<WorldFloatPosTag,f32>;
using FloatOffset = Coord3<FloatOffsetTag,f32>;

namespace std{
    template<> 
    struct hash<WorldChunkCoord>{
        auto operator()(const WorldChunkCoord& val)const noexcept{
            return std::hash<glm::ivec3>{}(val.raw());
        };
    };  
}; // namespace std

#define DECL_ADD_OPER(T1, T2)                        \
constexpr T1 operator+(T1 pos, T2 offset){    \
    return T1{pos.raw()+static_cast<T1::VecType>(offset.raw())};        \
}\
constexpr T1 operator+=(T1& pos, T2 offset){    \
    return pos={T1{pos.raw()+static_cast<T1::VecType>(offset.raw())}};        \
}

#define DECL_SUB_OPER(T1, T2)                      \
constexpr T1 operator-(T1 pos, T2 offset){    \
    return T1{pos.raw()-static_cast<T1::VecType>(offset.raw())};        \
}\
constexpr T1 operator-=(T1& pos, T2 offset){    \
    return pos={T1{pos.raw()-static_cast<T1::VecType>(offset.raw())}};        \
}

// NOTE: These operations perform casts to transform the second value into the first.
#define DECL_ADD_SUB_OPER(T1,T2) \
DECL_ADD_OPER(T1, T2) \
\
DECL_SUB_OPER(T1,T2)

// You can offset a position by a block offset
DECL_ADD_SUB_OPER(WorldFloatPos,BlockOffset)
// You can offset a position by a float offset.
DECL_ADD_SUB_OPER(WorldFloatPos,FloatOffset)
// You can offset a position by a chunk offset.
DECL_ADD_SUB_OPER(WorldFloatPos,ChunkOffset)




// You can offset a block coord by a chunk offset.
DECL_ADD_SUB_OPER(WorldBlockPos, ChunkOffset)
// You can offset a block coord by a block offset
DECL_ADD_SUB_OPER(WorldBlockPos, BlockOffset)

// You can offset a chunk local block pos by a block offset.
DECL_ADD_SUB_OPER(ChunkBlockPos, BlockOffset)

// You can offset a chunk coord by a chunk offset.
DECL_ADD_SUB_OPER(WorldChunkCoord, ChunkOffset)





// worldChunkCoord -> worldBlockPos 
// Returns the world pos of the block at [0,0,0] in the chunk identified by wChunkCoord
inline auto toWorldOrigin(WorldChunkCoord chunk) -> WorldBlockPos{
    return WorldBlockPos{chunk.raw() * Chunk_Extents};
}

// chunkBlockPos->worldBlockPos
// Returns the world pos of the block at [cBlockPos] in the chunk identified by wChunkCoord
inline auto toWorldBlockPos(WorldChunkCoord chunk, BlockOffset local) -> WorldBlockPos{
    return WorldBlockPos{toWorldOrigin(chunk) + local};
}

inline auto toWorldFloatPos(WorldBlockPos pos) -> WorldFloatPos{
    return WorldFloatPos{pos.raw()};
}

// Floatpos->worldBlockPos
// Rounds to the correct block
inline auto toWorldBlockPos(WorldFloatPos pos) -> WorldBlockPos{
    return WorldBlockPos{LM::floor(pos.raw())};
}

// worldBlockPos->      chunkBlockPos
// returns the chunk local pos, in the chunk within wBlockPos, of the block at [wBlockPos] in the world
inline auto toChunkBlockPos(WorldBlockPos block) -> ChunkBlockPos{
    return ChunkBlockPos{LM::euclid_mod(block.raw(), Chunk_Extents)};
}

inline auto toBlockOffset(glm::ivec3 vec){
    auto r = vec;
    return BlockOffset{r};
}

inline auto toBlockOffset(WorldBlockPos coord){
    return toBlockOffset(coord.raw());
}

// should simple casts like this use a different templated function maybe? 
// They arent really active transformations or anyhting.
// Also this function itself is suspicious. investigate callsites
inline auto toBlockOffset(ChunkOffset chunkOffset){
    return toBlockOffset(chunkOffset.raw());
}

inline auto toChunkOffset(BlockOffset boffset){
    auto r = boffset.raw();
    return ChunkOffset{r};
}

inline auto toChunkOffset(WorldChunkCoord coord){
    auto r = coord.raw();
    return ChunkOffset{r};
}
// worldBlockPos->      worldChunkCoord
inline auto toWorldChunkCoord(WorldBlockPos block) -> WorldChunkCoord {
    return WorldChunkCoord{LM::floor_div(block.raw(), Chunk_Extents)};
}
// worldBlockPos->      worldChunkCoord
inline auto toWorldChunkCoord(WorldFloatPos pos) -> WorldChunkCoord {
    return WorldChunkCoord{LM::floor_div(pos.raw(), Chunk_Extents)};
}
