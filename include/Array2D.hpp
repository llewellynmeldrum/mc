#pragma once 
#include "NumericConcepts.hpp"
#include "Types.h"
template<typename value_type, std::size_t extentX, std::size_t extentZ>
struct Array2D{
    std::array<value_type, extentX*extentZ> buf;

    using mapped_type = value_type;
    using key_type = glm::ivec2;
    auto contains(key_type p) const noexcept{
        return p[0]>=0 && p[0]<static_cast<i32>(extentX)
            && p[1]>=0 && p[1]<static_cast<i32>(extentZ);
    }
    auto size()const noexcept{
        return buf.size();
    }

    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),extentX, extentZ);
    }
    decltype(auto) at(this auto& self, glm::ivec2 v){
        return self.span()[v.x,v.y]; 
    }
    decltype(auto) operator[](this auto& self, size_t x, size_t z){
        return self.span()[x,z]; 
    }
    auto begin(this auto& self){
        return self.buf.begin();
    }
    auto end(this auto& self){
        return self.buf.end();
    }
};

template<typename value_type>
struct ArrayList2D{
    std::size_t extentX{0};
    std::size_t extentZ{0};
    std::vector<value_type> buf;

    constexpr ArrayList2D(std::size_t _extentX, std::size_t _extentZ):
        extentX(_extentX),
        extentZ(_extentZ),
        buf(_extentX*_extentZ)
    {}
    template<typename Vec2>
        requires is_ivec2<Vec2>
    constexpr ArrayList2D(const Vec2& extents):ArrayList2D(extents[0],extents[1]){}

    decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(),self.extentX, self.extentZ);
    }
    decltype(auto) operator[](this auto& self, size_t x, size_t z){
        return self.span()[x,z]; 
    }
    auto begin(this auto& self){
        return self.buf.begin();
    }
    auto end(this auto& self){
        return self.buf.end();
    }
};

