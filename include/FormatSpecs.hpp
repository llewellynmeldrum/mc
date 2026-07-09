#pragma once
#include "Mesh.hpp"
#include "PendingBlockWrites.hpp"

#include <format>

#include "Direction.hpp"
#include "Block.hpp"
#include "DebugFormat.hpp"
#include "FmtStyle.hpp"
#include "CoordTypes.hpp"
#include "Types.h"
#include "glmWrapper.hpp"
#include "LM.hpp"
#include "UnixHelpers.hpp"
#include "ChunkEntry.hpp"
#include "Assertion.hpp"

// specializations of dbg_fmt for glm types
//
#define dbg_fmt(val) assert_failure(#val, SRC_LOC_CURRENT(), "Tried to use deprecated dbg_fmt. Replace with std::formatter specialization")

template<>
struct std::formatter<glm::vec4>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec4& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}, {}{}{}]", "", val.x, "", "",
                       val.y, "", "", val.z, "", "", val.w,
                       "");
    }
};

template<>
struct std::formatter<glm::vec3>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec3& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}]", "", val.x, "", "", val.y,
                       "", "", val.z, "");
    }
};

template<>
struct std::formatter<glm::vec2>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec2& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}]", "", val.x, "", "", val.y,
                       "");
    }
};

template<>
struct std::formatter<glm::ivec4>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec4& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}, {}{}{}]", "", val.x, "", "",
                       val.y, "", "", val.z, "", "", val.w,
                       "");
    }
};

template<>
struct std::formatter<glm::ivec3>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec3& val, std::format_context& ctx)const {
		return 
std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}]", "", val.x, "", "", val.y,
                       "", "", val.z, "");
    }
};
template<>
struct std::formatter<glm::ivec2>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec2& val, std::format_context& ctx)const {
		return 
std::format_to(ctx.out(), "[{}{}{}, {}{}{}]", "", val.x, "", "", val.y,
                       "");
    }
};
template<>
struct std::formatter<glm::mat4>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::mat4& val, std::format_context& ctx)const {
        std::string expr_str{};
        expr_str.append("\n");
        i64 ext = 4;
        for (i64 row = 0; row < ext; row++) {
            expr_str.append("| ");
            for (i64 col = 0; col < ext; col++) {
                expr_str.append(std::format("{: 3.1f}", val[col][row]));
                if (col != ext - 1) {
                    expr_str.append(" ");
                }
            }
            expr_str.append(" |");
            if (row != ext - 1)
                expr_str.append("\n");
        }
        return format_to(ctx.out(), "{}",expr_str);
    }
};

#include "Vertex.hpp"
#include "KeyCodes.hpp"


template<>
struct std::formatter<Vertex>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Vertex& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}, {}]", val.pos, val.txCoords);
    }
};

template<>
struct std::formatter<KeyState>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const KeyState& val, std::format_context& ctx)const {
        std::string s{"INVALID_KEYSTATE"};
        switch (val) {
        case KeyState::JustPressed:
            s="JustPressed";
            break;
        case KeyState::JustReleased:
            s="JustReleased";
            break;
        case KeyState::Held:
            s="Held";
            break;
        case KeyState::Released:
            s="Released";
            break;

        case KeyState::INVALID:
            [[fallthrough]];
        default:
            break;
        }
        return format_to(ctx.out(), "{}",s);
    }
};

template<>
struct std::formatter<BlockType>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const BlockType& val, std::format_context& ctx)const {
        std::string s{"INVALID_BLOCK_TYPE"};
        #define X(var, ...) case BlockType:: var: s=#var; break;
        switch (val) {
            BLOCK_TYPE_LIST
        default:
            break;
        }
        #undef X
        return format_to(ctx.out(), "{}",s);
    }
};
template<>
struct std::formatter<OverwritePolicy>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const OverwritePolicy& val, std::format_context& ctx)const {
        std::string s{"INVALID_overwrite_policy"};
        #define X(var, ...) case OverwritePolicy:: var: s=#var; break;
        switch (val) {
            OVERWRITE_POLICY_LIST
        default:
            break;
        }
        #undef X
        return format_to(ctx.out(), "{}",s);
    }
};

template<>
struct std::formatter<Block>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Block& val, std::format_context& ctx)const {
		return std::formatter<BlockType>{}.format(val.type,ctx);
    }
};

#include "CommonUtils.hpp"
template<>
struct std::formatter<Direction>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Direction& val, std::format_context& ctx)const {
        std::string s{"INVALID_DIRECTION"};
        switch (val) {
        case Direction::FORWARD:
            s="Direction::FORWARD";
			break;
        case Direction::BACKWARD:
            s="Direction::BACKWARD";
			break;
        case Direction::LEFT:
            s="Direction::LEFT";
			break;
        case Direction::RIGHT:
            s="Direction::RIGHT";
			break;
        case Direction::DOWN:
            s="Direction::DOWN";
			break;
        case Direction::UP:
            s="Direction::UP";
			break;
        default:
            break;
        }
        return format_to(ctx.out(), "{}",s);
    }
};

#include "glbinding/gl/enum.h"
#include "glbinding-aux/Meta.h"

template<>
struct std::formatter<gl::GLenum>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const gl::GLenum& val, std::format_context& ctx)const {
		return glbinding::aux::Meta::getString(val);
    }
};
template<typename T>
struct std::formatter<Bounded<T>>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Bounded<T>& bounded, std::format_context& ctx)const {
        return std::formatter<T>{}.format(bounded.val,ctx);
    }
};
template<typename Tag, typename ScalarType>
struct std::formatter<Coord3<Tag,ScalarType>>{
    using T = Coord3<Tag,ScalarType>;
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const T& bounded, std::format_context& ctx)const {
        return std::formatter<typename T::VecType>{}.format(bounded.raw(),ctx);
    }
};
template<typename Tag, typename ScalarType>
struct std::formatter<Coord2<Tag,ScalarType>>{
    using T = Coord2<Tag,ScalarType>;
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const T& bounded, std::format_context& ctx)const {
        return std::formatter<typename T::VecType>{}.format(bounded.raw(),ctx);
    }
};

template <typename T, typename CharT>
requires std::is_pointer_v<T> 
struct std::formatter<T, CharT> : std::formatter<const void*, CharT> {
    
    // Format the pointer by casting it to a const void*
    template <typename FormatContext>
    auto format(T ptr, FormatContext& ctx) const {
        return std::formatter<const void*, CharT>::format(
            static_cast<const void*>(ptr), ctx
        );
    }
};
#include "ChunkEntry.hpp"

#define state_set_str(Enum, str_identifier, name) case Enum :: name: str_identifier=#name; break;
#define X(name) state_set_str(GenState, str, name)
template<>
struct std::formatter<GenState>{
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for Point.");
        }
        return it;
    }
    auto format(GenState s, std::format_context& ctx) const{
        std::string_view str = "???GenState???";
        switch (s){
            GEN_STATE_LIST
        }
        return format_to(ctx.out(), "{}", str);
    }
};
#if defined(X)
    #undef X
#endif
#define X(name) state_set_str(MeshState, str, name)
template<>
struct std::formatter<MeshState>{
    inline constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for MeshState.");
        }
        return it;
    }
    inline auto format(MeshState s, std::format_context& ctx) const{
        std::string_view str = "???ChunkMeshState???";
        switch (s){
            MESH_STATE_LIST 
        }
        return format_to(ctx.out(), "{}", str);
    }
};
#undef X
#undef state_set_str

template<>
struct std::formatter<ChunkState>{
    inline constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for ChunkState.");
        }
        return it;
    }
    inline auto format(ChunkState s, std::format_context& ctx) const{
        std::string_view str = "?ChunkState?";
        return format_to(ctx.out(), "G:{},M:{}", s.gen,s.mesh);
    }
};
template<typename T>
    requires std::formattable<T,char>
struct std::formatter<std::optional<T>>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(std::optional<T> s, std::format_context& ctx) const{
        if (s){
            return format_to(ctx.out(), "{}",s.value());
        }else{
            return format_to(ctx.out(), "nullopt");
        }
    }
};
template<>
struct std::formatter<QuadIndices>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(const QuadIndices& s, std::format_context& ctx) const{
            return format_to(ctx.out(), "??");
    }
};
template<>
struct std::formatter<KeyModifiers>{
    inline constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for ChunkState.");
        }
        return it;
    }
    inline auto format(KeyModifiers s, std::format_context& ctx) const{
        std::string_view str = "?ChunkState?";
		return std::format_to( ctx.out(), 
            "shift:{}\n"
            "ctrl:{}\n"
            "alt:{}\n"
            "super:{}\n"
            "caps:{}\n"
            "num_lock:{}\n"
            ,
            s.shift,
            s.ctrl,
            s.alt,
            s.super,
            s.caps,
            s.num_lock
        );
    }
};

template<>
struct std::formatter<IndexedMesh>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const IndexedMesh& val, std::format_context& ctx)const {
		return std::format_to(
            ctx.out(), 
            "vao:{}\n"
            "vbo:{}\n"
            "ebo:{}\n"
            "offset_count:{}\n"
            "vertex_count:{}\n"
            ,
            val.vao.id,
            val.vbo.id,
            val.ebo.id,
    val.offset_count,
    val.vertex_count);
    }
};
