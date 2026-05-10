#pragma once
#include "Types.h"
#include "Vertex.hpp"
#include "cppslop.hpp"
#include <cstddef>
#include <utility>

FORWARD_DECL_ENUM_STRUCT_NS(gl, GLenum, unsigned int)

struct VertexArray {
    DECL_NO_COPY(VertexArray);
    VertexArray() { make(); }
    ~VertexArray() { destroy(); }
    // T lhs(std::move(rhs));
    VertexArray(VertexArray&& rhs) noexcept : id(rhs.id) { rhs.id = 0; }

    // lhs = std::move(rhs)
    VertexArray& operator=(this auto&& lhs, VertexArray&& rhs) noexcept {
        if (&lhs != &rhs) {
            lhs.destroy();
            std::swap(lhs.id, rhs.id);
        }
        return lhs;
    }

    void make();
    void bind() const;
    void unbind() const;
    void destroy();

    void drawElements(i32 num, gl::GLenum usage_hint) const;
    void drawArrays(i32 count, gl::GLenum usage_hint, i32 offset = 0) const;

    template <std::size_t AttrCount>
    void apply_layout(const VertexLayout<AttrCount>& layout) {
        using VertexAttrSpan = std::span<const VertexAttribute, AttrCount>;
        apply_layout_impl(layout.stride, VertexAttrSpan(layout.attrs));
    }

  private:
    void apply_layout_impl(i32 stride, std::span<const VertexAttribute> attrs);
    u32  id;
};

struct VertexBuffer {
    DECL_NO_COPY(VertexBuffer);
    VertexBuffer() { make(); }
    ~VertexBuffer() { destroy(); }
    // T lhs(std::move(rhs));
    VertexBuffer(VertexBuffer&& rhs) noexcept : id(rhs.id) { rhs.id = 0; }

    // lhs = std::move(rhs)
    VertexBuffer& operator=(this auto&& lhs, VertexBuffer&& rhs) noexcept {
        if (&lhs != &rhs) {
            lhs.destroy();
            std::swap(lhs.id, rhs.id);
        }
        return lhs;
    }

    void make();
    void bind() const;
    void unbind() const;
    void destroy();

    void load(std::span<const Vertex> c, i32 offset = 0);

  private:
    constexpr static gl::GLenum BufferUsage();
    constexpr static gl::GLenum BufferTarget();
    u32                         id;
};

struct ElementBuffer {
    DECL_NO_COPY(ElementBuffer);
    ElementBuffer() { make(); }
    ~ElementBuffer() { destroy(); }

    // T lhs(std::move(rhs));
    ElementBuffer(ElementBuffer&& rhs) noexcept : id(rhs.id) { rhs.id = 0; }

    // lhs = std::move(rhs)
    ElementBuffer& operator=(this auto&& lhs, ElementBuffer&& rhs) noexcept {
        if (&lhs != &rhs) {
            lhs.destroy();
            std::swap(lhs.id, rhs.id);
        }
        return lhs;
    }

    void make();
    void bind() const;
    void unbind() const;
    void destroy();

    void load(std::span<const u32> indices, i32 offset = 0);

  private:
    constexpr static gl::GLenum BufferUsage();
    constexpr static gl::GLenum BufferTarget();
    u32                         id{};
};
