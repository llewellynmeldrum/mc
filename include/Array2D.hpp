#pragma once 
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
};

