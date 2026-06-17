#pragma once 

#include "ChunkConstants.hpp"
#include "CoordTypes.hpp"
#include "CommonConcepts.hpp"
#include "Assertion.hpp"
#include "NothrowLookup.hpp"
#include <print>
#include <type_traits>
#include <vector>
#include <ranges>


template<typename Key, typename Mapped>
struct slot_map{
private:
    using DenseIdx = std::size_t;

    static constexpr std::size_t NULL_INDEX = std::numeric_limits<std::size_t>::max();
public:


    decltype(auto) at(this auto& self, Key victim_key){
        return AT(self.buf,AT(self.sparse,victim_key));
    }

    template<typename Fn>
    auto sorted_keys(Fn&& compar){
        auto all_keys = std::views::keys(sparse) | std::ranges::to<std::vector>();
        std::ranges::sort(all_keys,std::forward<Fn>(compar));
        return all_keys;
    }
    auto all_keys(){
        return std::views::keys(sparse) | std::ranges::to<std::vector>();
    }

    template<typename Fn>
    auto erase_if(Fn&& pred){
        // TODO: Implement
    }
    template<typename Fn, typename Fn2, typename Rt2=return_type<Fn2>>
        requires std::is_invocable_v<Fn,Mapped&>
              && std::is_invocable_v<Fn2>
              && same_type<
                    return_type<Fn,Mapped&>,return_type<Fn2>
                 >
              && (has_default_ctor<Rt2> || !(same_type<Rt2,void>)) 
             // if return type Rt2 is NOT void, Rt must be default constructible
    decltype(auto) if_contains_else(Key key, Fn&& on_found, Fn2&& on_missing){
        auto it = sparse.find(key);
        if (it!=sparse.end()){
            const auto& dense_idx = it->second;
            auto& val = AT(buf,dense_idx);
            if constexpr(return_type_is<void,Fn>){
                std::invoke(std::forward<Fn>(on_found),val);
                return;
            }else{
                return std::invoke(std::forward<Fn>(on_found),val);
            }
        }else{
            if constexpr(return_type_is<void,Fn>){
                std::invoke(std::forward<Fn2>(on_missing));
                return;
            }else{
                return std::invoke(std::forward<Fn2>(on_missing));
            }
        }
    }

    template<typename Fn>
        requires std::is_invocable_v<Fn,Mapped&>
              && std::is_default_constructible_v<return_type<Fn>>
    decltype(auto) if_contains(Key key, Fn&& on_found){
        return if_contains_else(key,on_found,[]{});
    }

    template<typename _mapped>
        requires std::convertible_to<_mapped, Mapped> // allows only _Mapped& or _Mapped&&
    bool insert_or_assign(Key key, _mapped&& val){
        auto [it,slot_empty] = sparse.try_emplace(key,buf.size());
        if (slot_empty){
            buf.emplace_back(std::forward<_mapped>(val));
            dense.push_back(key);
        }else{
            DenseIdx idx = it->second;
            buf[idx] = std::forward<_mapped>(val);
        }
        return slot_empty;
    }
    auto begin(){
        return buf.begin();
    }
    auto end(){
        return buf.end();
    }
    void clear(){
        buf.clear();
        dense.clear();
        sparse.clear();
    }
    template<typename ...Args>
    bool emplace_or_assign(Key key, Args&& ... vargs){
        auto [it,inserted] = sparse.try_emplace(key,buf.size());
        if (inserted){
            dense.push_back(key);
            buf.emplace_back(std::forward<Args>(vargs)...);
        }else{
            DenseIdx idx = it->second;
            std::destroy_at(&buf[idx]);
            std::construct_at(&buf[idx], std::forward<Args>(vargs)...);
        }
        return inserted;
    }

    bool contains(Key victim_key){
        return sparse.contains(victim_key);
    }


    void erase(Key victim_key){
        auto it = sparse.find(victim_key);
        assert(it != sparse.end()); // tried to erase non-existent element
        DenseIdx victim_idx = it->second;

        auto back_idx = buf.size()-1;

        // only perform the swap if the victim is not already at the back.
        if (victim_idx != back_idx){
            Key back_key = AT(dense,back_idx);

            AT(buf,victim_idx) = std::move(AT(buf,back_idx));
            AT(dense,victim_idx) = back_key;

            AT(sparse,back_key)=victim_idx;
        }
        dense.pop_back();
        buf.pop_back();
        sparse.erase(it);
    }

//private:

    // @Brief:
    // A dense, no gaps vector containing the actual data entries.
    // buf[DenseIdx] = Value
    std::vector<Mapped> buf{}; 

    // @Brief: DenseIdx->Key
    // A dense array containing the Key for a given DenseIdx.
    // sparse[dense[key]] = key
    // The inverse of `sparse`.
    // -> dense[] and buf[] share 'address spaces' so to speak.
    std::vector<Key> dense{};

    // @Brief: Key->DenseIdx
    // A sparse array containing the DenseIdx for a given Key
    // dense[sparse[key]] = key
    // The inverse of `dense`.
    std::unordered_map<Key, DenseIdx> sparse{}; 



};

template<typename K, typename V>
struct std::formatter<slot_map<K,V>>{
    inline constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for Point.");
        }
        return it;
    }
    inline auto format(const slot_map<K,V>& s, std::format_context& ctx) const{
        std::string str{};
        str+="slot_map::buf:\n";
        for (const auto& [idx, val]: std::views::enumerate(s.buf)){
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        str+="slot_map::dense:\n";
        for (const auto& [idx, val]: std::views::enumerate(s.dense)){
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        str+="slot_map::sparse:\n";
        for (const auto& [idx, val]: std::views::enumerate(s.sparse)){
            const auto& [key, v] = val;
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        return format_to(ctx.out(), "{}", str);
    }
};

