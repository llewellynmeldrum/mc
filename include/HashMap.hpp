#pragma once 

#include <memory>
#include <unordered_map>
#include <type_traits>
#include "CommonConcepts.hpp"
#include "NothrowLookup.hpp"

// @Brief:
// A hashmap for storing by value or by smart pointer.
// Essentially just a thin wrapper around std::unordered_map. This was for two reasons:
//      1. I kept remaking functions like try_get_<map_name>() 
//      2. I wanted consistent raw pointer semantics, without forcing the backing/mapped object to 
//          be a raw pointer. 
//     3. I wanted to enforce `nullptr`== 'no entry', instead of using `std::optional` everywhere.
//     4. i wanted to be able to switch out backend details consistently, for example changing to a flat_map 
//     or using my AT() exception safe out of bounds handlers. (check include/NothrowLookup.hpp)
// Anyways, details:
// Whether the stored type (St) is a smart pointer or a regular value, 
//  a pointer is returned from any of the get functions.
// ---
// Why no `std::optional`s?
// I got sick of doing (*opt).function() and opt.value(). 
// I just want to do `if (opt) opt->function()`
// Yeah sure, its not exception safe, but who gives a fuck. 
//  std::bad_optional_access is no more useful to me than segfault core dumped. (skill issue probs)

template<typename T, typename = void>
struct mapped_type{
    using type = T;
};

template<typename T>
struct mapped_type<T, std::enable_if_t<is_unique_ptr<T>>>{
    using type = typename T::element_type;
};

template<typename T>
using mapped_type_t = mapped_type<T>::type;


template<typename Map>
concept HashMapStoresUniquePtr = requires{
    Map::St != Map::Mt;
};

template<typename Kt_, typename Mt_>
struct HashMap{
    // NOTE:
    // having explicit return types for getter style functions ruins const correctness.
    // The Mt* will fail to compile on const references/pointers to a hashmap, as it will
    // try to return a const Mt* but the explicit return type is Mt*.
    // It makes the shift+K info way better tho. Im conflicted.

public:
    using Kt = Kt_; 
    using St = Mt_; // Stored Type (what exists in the internal map)
    using Mt = mapped_type_t<Mt_>; // Mapped type (what returns from get functions) 

    template<typename ...Args>
        requires paren_constructible<Mt, Args...>
    decltype(auto) make_mapped(Args&& ...vargs){
        if constexpr(is_unique_ptr<Mt>){
            return std::make_unique<Mt>(std::forward<Args>(vargs)...);
        }else{
            return Mt(std::forward<Args>(vargs)...);
        }
    }
    // if entry exists, return. else return nullptr.
    Mt* at(this auto& self, const Kt& key){
        auto it = self.map.find(key);
        if (it==self.map.end()) {
            report_oor_intermediate(self.map,key,"HashMap","key",SRC_LOC_CURRENT());
        }
        return resolve_ptr(it->second);
    }
    // if entry exists, return. else return nullptr.
    Mt* operator[](this auto& self, const Kt& key){
        return self.at(self,key);
    }

    void erase(const Kt& key){
        map.erase(key);
    }
    // inserts val @ key if possible, else returns false.
    template<typename M>
        requires std::convertible_to<M,Mt>
    bool try_insert(const Kt& key, M&& val){
        auto [_,inserted] = map.try_emplace(key,make_mapped(std::forward<M>(val)));
        return inserted;
    }
    // construct_at() with vargs if possible, else returns false.
    template<typename ...Args>
        requires paren_constructible<Mt, Args...>
    bool try_emplace(const Kt& key, Args&& ...vargs){
        auto [_,inserted] = map.try_emplace(key,make_mapped(std::forward<Args>(vargs)...));
        return inserted;
    }

    
    template<typename ...Args>
        requires paren_constructible<Mt, Args...>
    Mt* emplace(const Kt& key, Args&& ...vargs){
        auto [it,inserted] = map.try_emplace(key,make_mapped(std::forward<Args>(vargs)...));
        // TODO: replace all the constructing ones with make_mapped 
        assert(inserted);
        return resolve_ptr(it->second);
    }


    template<typename M>
        requires std::convertible_to<M,Mt>
    Mt* insert(const Kt& key, M&& val){
        auto [it,inserted] = map.try_emplace(key,make_mapped(std::forward<M>(val)));
        assert(inserted);
        return resolve_ptr(it->second);
    }

    // inserts val @ key if possible, else replaces whatever entry exists currently @ key with val.
    template<typename M>
    bool insert_or_assign(const Kt& key, M&& val){
        auto [_,inserted] = map.insert_or_assign(key,make_mapped(std::forward<M>(val)));
        return inserted;
    }

    // if entry exists, return. Else insert val @ key
    template<typename M>
    Mt* get_or_insert(const Kt& key, M&& val){
        auto [it,inserted] = map.try_emplace(key,make_mapped(std::forward<M>(val)));
        return resolve_ptr(it->second);
    }

    // if entry exists, return. Else insert val @ key
    template<typename ...Args>
        requires paren_constructible<Mt, Args...>
    Mt* get_or_emplace(const Kt& key, Args&& ...vargs){
        auto [it,inserted] = map.try_emplace(key,make_mapped(std::forward<Args>(vargs)...));
        return resolve_ptr(it->second);
    }

    // if entry exists, return. else return nullptr.
    Mt* try_get(this auto& self, const Kt& key){
        auto it = self.map.find(key);
        if (it!=self.map.end()) return resolve_ptr(it->second);
        else return nullptr;
    }

    // invokes callable pred(value) if key->value mapping exists. Else returns false.
    template<typename Fn>
        requires std::invocable<Fn, Mt&>
    decltype(auto) if_contains(const Kt& key, Fn&& on_found){
        auto it = map.find(key);
        if (it!=map.end()){
            return std::invoke(std::forward<Fn>(on_found),*(resolve_ptr(it->second)));
        }
        return return_type<Fn,Mt&>{};
    }
    // invokes callable pred(value) if key->value mapping exists. Else returns false.
    template<typename Fn, typename Fn2>
        requires std::invocable<Fn, Mt&>
             && std::invocable<Fn2>
    decltype(auto) if_contains_else(const Kt& key, Fn&& on_found, Fn2&& on_missing){
        auto it = map.find(key);
        if (it!=map.end()){
            return std::invoke(std::forward<Fn>(on_found),*(resolve_ptr(it->second)));
        }else{
            return std::invoke(std::forward<Fn2>(on_missing));
        }
    }

    // Calls pred(key,val) for each [key,value] pair in the map
    template<typename Fn>
        requires std::invocable<Fn, Kt, Mt&>
    void for_each(Fn&& pred){
        for (auto& [key, val]: map){
            std::invoke(std::forward<Fn>(pred),key,*(resolve_ptr(val)));
        }
    }
    void clear(){
        map.clear();
    }
private:
    template <typename S>
    static constexpr auto resolve_ptr(S& ptr){
        if constexpr (is_unique_ptr<std::remove_const_t<St>>) return ptr.get();
        else return &ptr;
    }
    std::unordered_map<Kt, St> map;
};
