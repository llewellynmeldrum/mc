#pragma once 

#include <memory>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include "CommonConcepts.hpp"
#include "NothrowLookup.hpp"

// lookup functions: 
// nullptr = no match.
// TODO: make all lookup functions return nullptr on missing, regular pointer on success
// HACK: We use decltype(auto) everywhere to propogate 'const'ness of the reference/pointer the
// user currently has to the map. For example, HashMap::at() on a const HashMap* SHOULD return const HashMap::value_type*.
//
// @BRIEF: 
//  A small wrapper around std::unordered_map which provides some convinience functions and a more limited interface.
//  -> `get()/at()` style functions return a HashMap::mapped_type *
//  -> 
template<typename key_type_, typename mapped_type_>
struct HashMap{
public:
    using key_type = key_type_; 
    using mapped_type = mapped_type_; 
    using value_type = std::pair<key_type,mapped_type>; 
    using pointer_type = mapped_type_*; 

private:
    std::unordered_map<key_type, mapped_type> map;

public:
    
    // @BRIEF: Returns true if insertion occured, false if not (nothing).
    template<typename ...Args>
    bool try_emplace(const key_type& key, Args&& ...vargs){
        auto [it, inserted] = map.try_emplace(key, std::forward<Args>(vargs)...);
        return inserted;
    }

    // @BRIEF: Returns true if insertion occured, false if not (assignment).
    template<typename ...Args>
    decltype(auto) emplace_or_assign(const key_type& key, Args&& ...args){
        auto [it, inserted] = map.try_emplace(key,std::forward<args>(args)...);
        if (!inserted) it->second = T(args...);
        return inserted;
    }

    // @BRIEF: If a value exists at `[key]`, returns that, no insertion.
    //         Else, inserts a value at `[key]` and returns that.
    template<typename ...Args>
        requires paren_constructible<mapped_type, Args...>
    decltype(auto) get_or_emplace(const key_type& key, Args&& ...vargs){
        auto [it,inserted] = map.try_emplace(key,std::forward<Args>(vargs)...);
        return addr_of(it->second);
        // on failure, std::um::try_emplace returns an iterator to the element which 'caused the insertion
        // to fail', which in other words is the element which occupies the slot at `[key]`
    }


    template<typename M>
    bool insert_or_assign(const key_type& key, M&& val){
        return map.insert_or_assign(key,std::forward<M>(val));
    }

    template<typename M>
    decltype(auto) try_insert(const key_type& key, M&& val){
        auto [it, inserted] = map.insert(key,std::forward<M>(val));
        return inserted;
    }

    template<typename M>
    decltype(auto) get_or_insert(const key_type& key, M&& val){
        auto [it, inserted] = map.insert_or_assign(key,std::forward<M>(val));
        return addr_of(it->second);
    }

    
    // if entry exists, return it. else return nullptr.
    decltype(auto) at(this auto& self, const key_type& key){
        auto it = self.map.find(key);
        if (it==self.map.end()) {
            report_oor_intermediate(self.map,key,"HashMap","key",SRC_LOC_CURRENT());
        }
        return self.addr_of(it->second);
    }

    // if entry exists, return it. else return nullptr.
    inline decltype(auto) try_get(this auto& self, const key_type& key){
        auto it = self.map.find(key);
        if (it==self.map.end()) {
            return self.null_result();
        }
        return self.addr_of(it->second);
    }


    // if entry exists, return. else return nullptr.
    decltype(auto) operator[](this auto& self, const key_type& key){
        return self.at(key);
    }

    // propogates constness
    constexpr decltype(auto) addr_of(this auto& self, const auto& val){
        if constexpr(std::is_const_v<decltype(self)>){
            return const_cast<const mapped_type*>(&val);
        }else {
            return const_cast<mapped_type*>(&val);
        }
    }

    // propogates constness
    constexpr decltype(auto) null_result(this auto& self){
        if constexpr(std::is_const_v<decltype(self)>){
            return static_cast<const mapped_type*>(nullptr);
        }else {
            return static_cast<mapped_type*>(nullptr);
        }
    }




    
    template<typename Fn, typename Fn2, typename Rt2=return_type<Fn2>>
        requires callable_with<Fn,mapped_type&>
              && callable_with<Fn2>
              && same_type<
                    return_type<Fn,mapped_type&>,return_type<Fn2>
                 >
              && (has_default_ctor<Rt2> || same_type<Rt2,void>) 
             // if return type Rt2 is NOT void, Rt must be default constructible
    decltype(auto) if_contains_else(const key_type& key, Fn&& on_found, Fn2&& on_missing){
        auto it = map.find(key);
        if (it!=map.end()){
            if constexpr(return_type_is<void,Fn>){
                std::invoke(std::forward<Fn>(on_found),it->second);
                return;
            }else{
                return std::invoke(std::forward<Fn>(on_found),it->second);
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
    decltype(auto) if_contains(const key_type& key, Fn&& on_found){
        using return_type = return_type<Fn,mapped_type&>;

        if constexpr(same_type<return_type, void>){
            if_contains_else(key,std::forward<Fn>(on_found),[]{});
            return;
        }else{
            return if_contains_else(key,std::forward<Fn>(on_found),[]{return return_type();});
        }
    }

    // Calls pred(key,val) for each [key,value] pair in the map
    template<typename Fn>
        requires std::invocable<Fn, key_type, mapped_type&>
    void for_each(Fn&& pred){
        for (auto& [key, val]: map){
            std::invoke(std::forward<Fn>(pred),key,val);
        }
    }



    constexpr bool contains(const key_type& key)const noexcept{
        return map.contains(key);
    }
    constexpr std::size_t size()const noexcept{
        return map.size();
    }
    constexpr bool empty() const noexcept{
        return map.size()==0;
    }

    constexpr void clear()noexcept{
        map.clear();
    }
    void erase(const key_type& key)noexcept{
        map.erase(key);
    }
private:
};

/*
 * NOTE: 
 * Old, more complicated version which i might use 
template<typename key_type_, typename mapped_type_, typename StoragePolicy = InlineStorage>
template<typename T, typename = void>
struct element_type{
    using type = T;
};

template<typename T>
struct element_type<T, std::enable_if_t<is_unique_ptr<T>>>{
    using type = typename T::element_type;
};

template<typename T>
using element_type_t = element_type<T>::type;




struct InlineStorage;   // mapped_value is T
struct StableAddressing; // mapped_value is unique_ptr<T>

template<typename T, typename StoragePolicy>
struct hm_storage_type;


template<typename T>
struct hm_storage_type<T, InlineStorage>{
    using type = T;
};

template<typename T>
struct hm_storage_type<T, StableAddressing>{
    using type = std::unique_ptr<T>;
};

template<typename T, typename StoragePolicy>
using hashmap_storage_type_t = hm_storage_type<T, StoragePolicy>::type;
struct HashMap{
public:
    enum struct Ret{
        ptr,
        success,
    };
    using key_type = key_type_; 
    using mapped_type = mapped_type_; 
    using pointer_type = mapped_type_*; 

    // The underlying unordered_map::value_type, which exists in the map.
    using storage_type = hashmap_storage_type_t<mapped_type, StoragePolicy>;

    static constexpr bool inlineStorage = std::same_as<StoragePolicy,InlineStorage>;
private:
    std::unordered_map<key_type, storage_type> map;

public:
    void print_types(){
        println("\tkey_type:{}",pretty_type_name<key_type>());
        println("\tmapped_type:{}",pretty_type_name<mapped_type>());
        println("\tpointer_type:{}",pretty_type_name<pointer_type>());
        println("\tstorage_type:{}",pretty_type_name<storage_type>());
        println("\tstorage_policy:{}",pretty_type_name<StoragePolicy>());
    }
    
    template<typename ...Args>
    decltype(auto) try_emplace(const key_type& key, Args&& ...vargs){
        if constexpr(inlineStorage){
            return map.try_emplace(key, std::forward<Args>(vargs)...);
        } else{
            return map.try_emplace(key, std::move(std::make_unique<mapped_type>(std::forward<Args>(vargs)...)));
        }
    }

    // SECTION: 
    
    template<typename ...Args>
    decltype(auto) emplace_or_assign(const key_type& key, Args&& ...args){
        if constexpr(inlineStorage){
            return map.insert_or_assign(key,std::forward<args>(args)...);
        } else{ 
            return map.insert_or_assign(key, std::move(std::make_unique<mapped_type>(std::forward<args>(args)...)));
        }
    }

    template<typename ...Args>
        requires paren_constructible<mapped_type, Args...>
    decltype(auto) get_or_emplace(const key_type& key, Args&& ...vargs){
        auto [it,inserted] = try_emplace(key,std::forward<Args>(vargs)...);
        return resolve_ptr(it->second);
    }



    template<typename M>
    bool insert_or_assign(const key_type& key, M&& val){
        static_check_storage_policy();
        return emplace_or_assign(key,std::forward<M>(val));
    }


    template<typename M>
    decltype(auto) try_insert(const key_type& key, M&& val){
        static_check_storage_policy();
        return try_emplace(key,std::forward<M>(val));
    }

    template<typename M>
    decltype(auto) get_or_insert(const key_type& key, M&& val){
        static_check_storage_policy();
        return get_or_emplace(key,std::forward<M>(val));
    }

    
    // better semantic match, worse STL likeness. i cant decide, porque no los dos?
    inline mapped_type* try_get(this auto& self, const key_type& key){
        return self.at(key);
    }
    // if entry exists, return it. else return nullptr.
    mapped_type* at(this auto& self, const key_type& key){
        auto it = self.map.find(key);
        if (it==self.map.end()) {
            report_oor_intermediate(self.map,key,"HashMap","key",SRC_LOC_CURRENT());
            return resolve_ptr(nullptr);
        }
        return resolve_ptr(it->second);
    }

    // if entry exists, return. else return nullptr.
    mapped_type* operator[](this auto& self, const key_type& key){
        return self.at(key);
    }





    
    template<typename Fn, typename Fn2, typename Rt2=return_type<Fn2>>
        requires callable_with<Fn,mapped_type&>
              && callable_with<Fn2>
              && same_type<
                    return_type<Fn,mapped_type&>,return_type<Fn2>
                 >
              && (has_default_ctor<Rt2> || same_type<Rt2,void>) 
             // if return type Rt2 is NOT void, Rt must be default constructible
    decltype(auto) if_contains_else(const key_type& key, Fn&& on_found, Fn2&& on_missing){
        auto it = map.find(key);
        if (it!=map.end()){
            if constexpr(return_type_is<void,Fn>){
                std::invoke(std::forward<Fn>(on_found),*(resolve_ptr(it->second)));
                return;
            }else{
                return std::invoke(std::forward<Fn>(on_found),*(resolve_ptr(it->second)));
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
    decltype(auto) if_contains(const key_type& key, Fn&& on_found){
        if constexpr(return_type_is<void,Fn>){
            if_contains_else(key,std::forward<Fn>(on_found),[]{});
            return;
        }else{
            return if_contains_else(key,std::forward<Fn>(on_found),[]{});
        }
    }
    // invokes callable pred(value) if key->value mapping exists. Else returns false.
    template<typename Fn, typename Fn2>
        requires std::invocable<Fn, mapped_type&>
             && std::invocable<Fn2>
    decltype(auto) if_contains_else(const key_type& key, Fn&& on_found, Fn2&& on_missing){
        auto it = map.find(key);
        if (it!=map.end()){
            return std::invoke(std::forward<Fn>(on_found),*(resolve_ptr(it->second)));
        }else{
            return std::invoke(std::forward<Fn2>(on_missing));
        }
    }
    // invokes callable pred(value) if key->value mapping exists. Else returns false.
    template<typename Fn>
        requires std::invocable<Fn, mapped_type&>
    decltype(auto) if_contains(const key_type& key, Fn&& on_found){
        if constexpr(return_type_is<void,Fn>){
            if_contains_else(key,std::forward<Fn>(on_found),[](){});
            return;
        }else{
            return if_contains_else(key,std::forward<Fn>(on_found),[](){return return_type<Fn,key_type&>{};});
        }
    }
    


    // Calls pred(key,val) for each [key,value] pair in the map
    template<typename Fn>
        requires std::invocable<Fn, key_type, mapped_type&>
    void for_each(Fn&& pred){
        for (auto& [key, val]: map){
            std::invoke(std::forward<Fn>(pred),key,*(resolve_ptr(val)));
        }
    }



    constexpr bool contains(const key_type& key)const noexcept{
        return map.contains(key);
    }
    constexpr std::size_t size()const noexcept{
        return map.size();
    }
    constexpr bool empty() const noexcept{
        return map.size()==0;
    }
    constexpr void clear()const noexcept{
        map.clear();
    }
    void erase(const key_type& key)noexcept{
        map.erase(key);
    }
private:
    consteval void static_check_storage_policy(){
        static_assert(inlineStorage, "\n\t-> Storage policy incompatible with insert() methods! Use emplace() instead.");
    }
    template <typename S>
    static constexpr pointer_type resolve_ptr(S&& ptr){
        if constexpr(same_type<S,std::nullptr_t>) return nullptr;
        else if constexpr (is_unique_ptr<std::remove_const_t<storage_type>>) return ptr.get();
        else return &ptr;
    }
};
*/
