#pragma once 
#include <print>
#include <type_traits>
#include <functional>
#include <vector>
#include <limits>

#include "ChunkConstants.hpp"
#include "CommonConcepts.hpp"
#include "Assertion.hpp"
#include "NothrowLookup.hpp"

#include "cpp23_ranges.hpp"

template<typename Key, typename Mapped>
struct slot_map{
public:
    using DenseIdx = size_t;
    using mapped_type = Mapped;
    using key_type = Key;

    auto size()const noexcept{
        return dense.size();
    }

    decltype(auto) at(this auto& self, Key victim_key){
        return AT(self.buf,AT(self.sparse,victim_key));
    }

    template<typename Fn>
    auto sorted_keys(Fn&& compar){
        auto all_keys = views::keys(sparse) | ranges::to<std::vector>();
        ranges::sort(all_keys,std::forward<Fn>(compar));
        return all_keys;
    }
    auto all_keys()const{
        return views::keys(sparse) | ranges::to<std::vector>();
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

    template<typename Fn>
    auto erase_if(Fn&& pred){
        std::vector<Key> victim_keys;
        for (const auto&[key, dense_idx]: sparse){
            if (pred(AT(buf,dense_idx))){
                victim_keys.push_back(key);
            }
        }
        for (const auto& key: victim_keys){
            erase(key);
        }
        return victim_keys.size();
    }

    template<typename Fn, typename Fn2>
    auto erase_if(Fn&& pred, Fn2&& on_each){
        std::vector<Key> victim_keys;
        for (const auto&[key, dense_idx]: sparse){
            if (pred(AT(buf,dense_idx))){
                victim_keys.push_back(key);
            }
        }
        for (const auto& key: victim_keys){
            on_each(this->at(key));
            erase(key);
        }
        return victim_keys.size();
    }
    template<typename Fn>
    auto for_each(Fn&& action){
        for (const auto&[key, dense_idx]: sparse){
            action(AT(buf,dense_idx));
        }
    }
    template<typename Fn, typename Fn2>
    auto for_each_if(Fn&& pred, Fn2&& action){
        std::vector<Mapped*> element_matches;
        for (const auto&[key, dense_idx]: sparse){
            Mapped* element = &(AT(buf,dense_idx));
            if (pred(*element)){
                element_matches.push_back(element);
            }
        }
        for (Mapped* val: element_matches){
            action(*val);
        }
        return element_matches.size();
    }
    template<typename Fn, typename Fn2, typename Fn3>
    auto for_each_if_else(Fn&& pred, Fn2&& on_true, Fn3&& on_false){
        std::vector<std::pair<bool,Mapped*>> element_matches;
        auto true_count = 0uz;
        for (const auto&[key, dense_idx]: sparse){
            Mapped* element = &(AT(buf,dense_idx));
            element_matches.emplace_back(pred(*element),element);
        }
        for (auto& [match, ptr]: element_matches){
            true_count += match;
            match ? on_true(*ptr) :  on_false(*ptr);
        }
        return true_count;
    }
    template<typename Fn, typename Fn2, typename Rt2=return_type<Fn2>>
        requires callable_with<Fn,Mapped&>
              && callable_with<Fn2>
              && same_type<
                    return_type<Fn,Mapped&>,return_type<Fn2>
                 >
              && (has_default_ctor<Rt2> || same_type<Rt2,void>) 
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
    decltype(auto) if_contains(Key key, Fn&& on_found){
        if constexpr(return_type_is<void,Fn>){
            if_contains_else(key,std::forward<Fn>(on_found),[]{});
            return;
        }else{
            return if_contains_else(key,std::forward<Fn>(on_found),[]{});
        }
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
    auto begin()const{
        return buf.begin();
    }
    auto end()const{
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
            dense.at(idx) = key;
            std::destroy_at(&buf[idx]);
            std::construct_at(&buf[idx], std::forward<Args>(vargs)...);
        }
        return inserted;
    }

    bool contains(Key victim_key)const noexcept{
        return sparse.contains(victim_key);
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
        for (const auto& [idx, val]: views::enumerate(s.buf)){
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        str+="slot_map::dense:\n";
        for (const auto& [idx, val]: views::enumerate(s.dense)){
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        str+="slot_map::sparse:\n";
        for (const auto& [idx, val]: views::enumerate(s.sparse)){
            const auto& [key, v] = val;
            str+=std::format("\t[{}]->{}\n",idx,val);
        }
        return format_to(ctx.out(), "{}", str);
    }
};

