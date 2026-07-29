#pragma once 

#include <vector>

#include "CommonConcepts.hpp"
#include "cppslop.hpp"
#include "Chunk.hpp"
#include "ChunkView.hpp"

template<typename V>
    requires has_default_ctor<V>
struct GenericChunkStore{
public:
    #define CLASS_NAME GenericChunkStore
    using mapped_type = V;
    using this_type = GenericChunkStore;
    static constexpr size_t x_extent = ChunkInfo::XWIDTH;
    static constexpr size_t y_extent = ChunkInfo::HEIGHT;
    static constexpr size_t z_extent = ChunkInfo::ZWIDTH;

    // Default construct to be ChunkInfo::SIZE
    GenericChunkStore() {init();}
    void init() noexcept{
        buf.resize(ChunkInfo::SIZE, mapped_type{});
    }
    void reset() noexcept{
        buf.clear();
        init();
    }
    COPY_CTOR(default)
    MOVE_CTOR(default)
    COPY_ASSN(default)
    MOVE_ASSN(default)
    ~GenericChunkStore() = default;


    // Construct from view
    explicit GenericChunkStore(GenericChunkView<const mapped_type> src): 
        buf(src.data_handle(), src.data_handle()+ChunkInfo::SIZE) {}


    std::vector<mapped_type> buf ={};

private:
    constexpr decltype(auto) span(this auto& self){
        return std::mdspan(self.buf.data(), ChunkInfo::XWIDTH, ChunkInfo::HEIGHT, ChunkInfo::ZWIDTH);
    }
public:
    constexpr decltype(auto) at(this auto& self, ChunkBlockPos p){
        self.bounds_check(p.x,p.y,p.z);
        return self.span()[p.x,p.y,p.z];
    }

    constexpr decltype(auto) at(this auto& self, i32 x, i32 y, i32 z){
        self.bounds_check(x,y,z);
        return self.span()[x,y,z];
    }

    constexpr decltype(auto) operator[](this auto& self, i32 x, i32 y, i32 z){
        self.bounds_check(x,y,z);
        return self.span()[x,y,z];
    }

    constexpr auto empty() const noexcept{
        return buf.empty();
    }

    // Obtain a non owning ChunkView from this chunk.
    constexpr auto view (){
        return GenericChunkView<mapped_type>{span()};
    }

    // Obtain a non owning, const ChunkView from this chunk.
    constexpr auto view () const{
        return GenericChunkView<const mapped_type>{span()};
    }


    // Construct a clone from the view ctor
    GenericChunkStore clone() const {
        return GenericChunkStore{view()};
    }

    // iterator support
    constexpr auto begin(this auto& self){
        return self.buf.begin();
    }
    constexpr auto end(this auto& self){
        return self.buf.end();
    }


    // HACK: required stuff for MapLike<> so we dont upset my AT() wrapper
    using key_type = ChunkBlockPos;
    constexpr size_t size()const noexcept{ return ChunkInfo::SIZE;}
    constexpr bool contains(ChunkBlockPos p)const noexcept{
        return  0 <= p.x && p.x < ChunkInfo::Extents3D.x && 
                0 <= p.y && p.y < ChunkInfo::Extents3D.y && 
                0 <= p.z && p.z < ChunkInfo::Extents3D.z ;
    }

private:
    void bounds_check(i32 cx, i32 cy, i32 cz) const {
        if (
            cx < 0 || cx >= ChunkInfo::XWIDTH ||
            cy < 0 || cy >= ChunkInfo::HEIGHT ||
            cz < 0 || cz >= ChunkInfo::ZWIDTH
        ){
            throw std::out_of_range("Outside of local chunk bounds");
        }
    }
    #undef CLASS_NAME
};
