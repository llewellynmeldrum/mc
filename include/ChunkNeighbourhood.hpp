#include "ChunkHelpers.hpp"
#include "ChunkStorage.hpp"
#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include <print>
struct ChunkNeighbourhood{
    WorldChunkCoord center_coord;
    ChunkBlockStore* blocks;
    const_span<ChunkBlockSlice> neighbour_block_slices;

    ChunkLightStore* lights;
    const_span<ChunkLightSlice> neighbour_light_slices;
    

    static constexpr bool is_in_center(ChunkBlockPos pos)noexcept{
        return is_in_chunk(pos);
    }


    template<typename V> 
    constexpr const_span<GenericChunkSlice<V>> which_slices(){
        if constexpr(std::same_as<V,Block>){
            return neighbour_block_slices;
        }else if constexpr (std::same_as<V,PackedLightValue>){
            return neighbour_light_slices;
        }else{
            static_assert(false);
        }
    }
    template<typename V> 
    constexpr GenericChunkStore<V>* which_store(){
        if constexpr(std::same_as<V,Block>){
            return blocks;
        }else if constexpr (std::same_as<V,PackedLightValue>){
            return lights;
        }else{
            static_assert(false);
        }
    }
    template<typename V>
    V get(ChunkBlockPos pos) {
        auto const& slices = which_slices<V>();
        auto const* store = which_store<V>();
        if (is_in_center(pos)){
            return store->at(pos);
        }else{
            auto const neighbour_dir = get_cpos_overflow_direction(pos);
            auto const neighbour_dir_idx = std::to_underlying(neighbour_dir);
            auto corrected_pos = LM::euclid_mod(pos, ChunkInfo::Extents3D);
            if (slices[neighbour_dir_idx].is_empty){
                return V{};
            }else{
                return slices[neighbour_dir_idx].at(corrected_pos);
            }
        }
    }
    template<typename T>
    void const_check(){
        static_assert(!std::is_const_v<std::remove_reference_t<T>>, "Attempted to modify a neighbour slice declared as const");
    }
    template<typename V>
    void set(ChunkBlockPos pos, V v) {
        auto* store = which_store<V>();
        if (is_in_center(pos)){
            store->at(pos) = v;
            assert(get<V>(pos)==v);
        }else{
            throw std::out_of_range("ChunkNeighbourhood: attempted to write to a value outside the center chunk.");
        }
    }
    void set_blocklight(ChunkBlockPos pos, BlockLight v){
        if (is_in_center(pos)){
            lights->at(pos).set_blocklight_r(v.r);
            lights->at(pos).set_blocklight_g(v.g);
            lights->at(pos).set_blocklight_b(v.b);
        }else{
            throw std::out_of_range("ChunkNeighbourhood: attempted to write to a value outside the center chunk.");
        }
    }
    void set_sunlight(ChunkBlockPos pos, u8 v){
        if (is_in_center(pos)){
            lights->at(pos).set_sunlight_intensity(v);
        }else{
            throw std::out_of_range("ChunkNeighbourhood: attempted to write to a value outside the center chunk.");
        }
    }
    void set_block(ChunkBlockPos pos, auto v){set<Block>(pos,v );}
    PackedLightValue light_at(ChunkBlockPos pos){return get<PackedLightValue>(pos);}
    Block block_at(ChunkBlockPos pos){return get<Block>(pos);}
};
