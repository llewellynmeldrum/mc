#pragma once
#include "ChunkState.hpp"
#include "Mesh.hpp"
#include "PendingBlockWrites.hpp"

#include <format>
#include <type_traits>

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
#include "qlibs-mp.hpp"

template<glm::length_t L, typename T, glm::qualifier Q> 
struct std::formatter<glm::vec<L,T,Q>>{
    using vec_t = glm::vec<L,T,Q>;

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(vec_t const& val, auto& ctx)const {
        std::string res{""};
        for (glm::length_t i = 0; i<L; i++){
            res += std::format("{}", val[i]);
            if (i<L-1)
            res += std::format(", ", val[i]);
        }
        return std::format_to(ctx.out(), "[{}]",res);
    }
};

template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct std::formatter<glm::mat<C,R,T,Q>>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const glm::mat4& val, auto& ctx)const {
        std::string res{};
        res.append("\n");
        for (i64 row = 0; row < R; row++) {
            res.append("| ");
            for (i64 col = 0; col < C; col++) {
                res.append(std::format("{: 3.1f}", val[col][row]));
                if (col != C - 1) {
                    res.append(" ");
                }
            }
            res.append(" |");
            if (row != R - 1)
                res.append("\n");
        }
        return format_to(ctx.out(), "{}",res);
    }
};

#include "Vertex.hpp"
#include "KeyCodes.hpp"



// generic formatters for those without one already defined:
// NOTE: enum formatter
template<typename T>
    requires std::is_enum_v<T>
struct std::formatter<T>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const T& val, auto& ctx)const {
        return format_to(ctx.out(), "{}",reflect::enum_name(val));
    }
};
// NOTE: qlibs reflect based POD class formatter
// Should work for 99% of aggregate classes.
template<class T>
concept qlibs_reflect_formattable =
    std::is_class_v<T> &&
    std::is_aggregate_v<T> &&
    std::is_trivially_copyable_v<T>;


template<qlibs_reflect_formattable T>
struct std::formatter<T, char> {
    bool verbose = false;

    constexpr auto parse(format_parse_context& ctx)
    {
        auto it = ctx.begin();

        if (it != ctx.end() && *it == 'v') {
            verbose = true;
            ++it;
        }

        if (it != ctx.end() && *it != '}')
            throw format_error{"invalid POD format specifier"};

        return it;
    }

    template<class FormatContext>
    auto format(T const& obj, FormatContext& ctx) const
    {
        auto out = ctx.out();
        bool first = true;
        auto append = [&out](){
        };

        reflect::for_each([&](auto I) {
            auto const& value = reflect::get<I>(obj);

            if (verbose) {
                if (!first)
                    out = std::format_to(out, "\n");

                out = std::format_to(
                    out,
                    ".{} : {} = {}",
                    reflect::member_name<I>(obj),
                    reflect::type_name(value),
                    value
                );
            }
            else {
                if (!first)
                    out = std::format_to(out, ", ");

                out = std::format_to(out, "{}", value);
            }

            first = false;
        }, obj);

        return out;
    }
};

template<>
struct std::formatter<Vertex>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const Vertex& val, auto& ctx)const {
		return std::format_to(ctx.out(), "[{}, {}]", val.pos, val.tx_coords);
    }
};

template<>
struct std::formatter<BlockDef>{
  constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
  auto format(const BlockDef& val, auto& ctx)const {
    return std::format_to(ctx.out(),
      "BlockDef{{name={}, block_type={}, shape={}, tex_idx={}, rend_layer={}, opacity={}}}",
      val.name,
      val.block_type,
      std::to_underlying(val.shape),
      val.tex_idx,
      std::to_underlying(val.rend_layer),
      val.opacity);
    }
};
template<>
struct std::formatter<OverwritePolicy>{

	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const OverwritePolicy& val, auto& ctx)const {
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
	auto format(const Block& val, auto& ctx)const {
		return std::formatter<BlockType>{}.format(val.type,ctx);
    }
};

#include "CommonUtils.hpp"

#include "glbinding/gl/enum.h"
#include "glbinding-aux/Meta.h"

template<>
struct std::formatter<gl::GLenum>{
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const gl::GLenum& val, auto& ctx)const {
		return glbinding::aux::Meta::getString(val);
    }
};
struct verbose_parse{
    bool is_verbose{false};
    constexpr auto operator()(auto & ctx){
        auto it = ctx.begin();
        auto end = ctx.end();
        if (it == end || *it == '}') {
            return it;
        }
        if (*it == 'v'){
            is_verbose = true;
            it++;
        }else{
            throw std::format_error("Invalid format specifier for Bounded<T>.");
        }
        if (it != end && *it != '}') {
            throw std::format_error("Invalid format specifier syntax.");
        }
        return it;
    }
};
template<typename T>
    requires std::formattable<T,char>
struct std::formatter<Bounded<T>>{
    verbose_parse verbose_parser;
	constexpr auto parse(auto& ctx){ return verbose_parser(ctx); }
	constexpr auto format(Bounded<T> const& v, auto& ctx)const {
        if (verbose_parser.is_verbose){
            return format_to(ctx.out(), "cur: {}, default: {}, min: {}, max: {}", 
                             v.get(),v.get_default(), v.get_min(), v.get_max());
        }else{
            return format_to(ctx.out(), "{}", v.get());
        }
    }
};
template<typename T>
    requires std::formattable<T,char>
struct std::formatter<DebugVal<T>>{
    verbose_parse verbose_parser;
	constexpr auto parse(auto& ctx){ return verbose_parser(ctx); }
	auto format(Bounded<T> const& v, auto& ctx)const noexcept{
        if (verbose_parser.is_verbose){
            return format_to(ctx.out(), "cur: {}, default: {}, min: {}, max: {}", 
                             v.get(),v.get_default(), v.get_min(), v.get_max());
        }else{
            return format_to(ctx.out(), "{}", v.get());
        }
    }
};

template<typename Tag, typename ScalarType>
struct std::formatter<Coord3<Tag,ScalarType>>{
    using T = Coord3<Tag,ScalarType>;
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(const T& val, auto& ctx)const {
        return format_to(ctx.out(), "{}",val.raw());
    }
};

// glm_vec_type is just:
// using glm_vec_type = glm::vec<Extent, ScalarType, glm::defaultp>;
template<typename Tag, typename ScalarType>
struct std::formatter<Coord2<Tag,ScalarType>>{
    using T = Coord2<Tag,ScalarType>;
	constexpr auto parse(std::format_parse_context& ctx){return ctx.begin();}
	auto format(T const& val, auto& ctx)const {
        return format_to(ctx.out(), "{}", val.raw());
    }
};

template <typename T, typename CharT>
requires std::is_pointer_v<T>  && (!std::same_as<std::remove_const_t<T>,char*>)
struct std::formatter<T, CharT> : std::formatter<const void*, CharT> {
    
    // Format the pointer by casting it to a const void*
    template <typename FormatContext>
    auto format(T ptr, FormatContext& ctx) const {
        return std::formatter<const void*, CharT>::format(
            static_cast<const void*>(ptr), ctx
        );
    }
};
template<typename T>
    requires std::formattable<T,char>
struct std::formatter<std::optional<T>>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(std::optional<T> s, auto& ctx) const{
        if (s){
            return format_to(ctx.out(), "{}",s.value());
        }else{
            return format_to(ctx.out(), "std::nullopt");
        }
    }
};
template<>
struct std::formatter<KeyModifiers>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(KeyModifiers s, auto& ctx) const{
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

// Classes with unions kinda need this
template<>
struct std::formatter<BlockLight>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(BlockLight const& s, auto& ctx) const{
		return std::format_to(
            ctx.out(), "r={}, g={}, b={}",
            s.r,s.g,s.b);
    }
};
template<>
struct std::formatter<UnpackedLightValue>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(UnpackedLightValue const& s, auto& ctx) const{
		return std::format_to(
            ctx.out(), "r={}, g={}, b={}, s={}",
            s.r,s.g,s.b,s.s);
    }
};
template<>
struct std::formatter<PackedLightValue>{
    inline constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin();}
    inline auto format(PackedLightValue const& s, auto& ctx) const{
		return std::format_to( ctx.out(), "{}",unpack(s));
    }
};

