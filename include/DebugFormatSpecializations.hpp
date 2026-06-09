#pragma once
#include "Block.hpp"
#ifdef INCLUDE_LOGGER_LAST
#error "Logger.hpp must be included AFTER DebugFormat* headers!"
#endif
#include "DebugFormat.hpp"
#include <format>
#include "AnsiCodes.hpp"
#include "Types.h"
#include "glmWrapper.hpp"

// specializations of dbg_fmt for glm types
//
template <glm::length_t L, typename T, glm::qualifier Q>
inline constexpr std::string dbg_fmt(const glm::vec<L, T, Q>& v) {
    return "CHOSE THE GLM ONE";
}
template<>
struct std::formatter<glm::vec4>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec4& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green,
                       val.y, fmt::reset, fmt::fg_blue, val.z, fmt::reset, fmt::fg_grey, val.w,
                       fmt::reset);
    }
};

template<>
struct std::formatter<glm::vec3>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec3& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green, val.y,
                       fmt::reset, fmt::fg_blue, val.z, fmt::reset);
    }
};

template<>
struct std::formatter<glm::vec2>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::vec2& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green, val.y,
                       fmt::reset);
    }
};

template<>
struct std::formatter<glm::ivec4>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec4& val, std::format_context& ctx)const {
		return std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green,
                       val.y, fmt::reset, fmt::fg_blue, val.z, fmt::reset, fmt::fg_grey, val.w,
                       fmt::reset);
    }
};

template<>
struct std::formatter<glm::ivec3>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec3& val, std::format_context& ctx)const {
		return 
std::format_to(ctx.out(), "[{}{}{}, {}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green, val.y,
                       fmt::reset, fmt::fg_blue, val.z, fmt::reset);
    }
};
template<>
struct std::formatter<glm::ivec2>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::ivec2& val, std::format_context& ctx)const {
		return 
std::format_to(ctx.out(), "[{}{}{}, {}{}{}]", fmt::fg_red, val.x, fmt::reset, fmt::fg_green, val.y,
                       fmt::reset);
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
		return std::format_to(ctx.out(), "[{}, {}]", dbg_fmt(val.pos), dbg_fmt(val.txCoords));
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
        switch (val) {
        case BlockType::AIR:
            s="BlockType::AIR"; 
            break;
        case BlockType::GRASS_BLOCK:
            s="BlockType::GRASS_BLOCK";
            break;
        case BlockType::DIRT_BLOCK:
            s="BlockType::DIRT_BLOCK ";
            break;
        case BlockType::STONE_BLOCK:
            s="BlockType::STONE_BLOCK ";
            break;
        case BlockType::COUNT:
            s="BlockType::COUNT";
            break;
        default:
            break;
        }
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
        return std::formatter<T>{}.format(bounded.val);
    }
};
