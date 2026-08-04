#pragma once
#include "AttributeTraits.hpp"

template <typename T>
constexpr VertexAttribute make_attr(size_t _location, std::uintptr_t offset, u32 _divisor=0) {
    return VertexAttribute{
        .location = static_cast<u32>(_location),
        .count = attribute_traits<T>::count,
        .offset_ptr = offset,
        .normalized = attribute_traits<T>::normalized,
        .is_integer = attribute_traits<T>::is_integer,
        .vat = attribute_traits<T>::vertex_attribute_type,
        .divisor = _divisor,
    };
}
template <typename T, size_t location, std::uintptr_t offset>
consteval VertexAttribute make_attr_t(u32 _divisor=0) {
    return VertexAttribute{
        .location = location,
        .count = attribute_traits<T>::count,
        .offset_ptr = offset,
        .normalized = attribute_traits<T>::normalized,
        .is_integer = attribute_traits<T>::is_integer,
        .vat = attribute_traits<T>::vertex_attribute_type,
        .divisor = _divisor,
    };
}

template<std::size_t I, class T>
using field_type_t = std::remove_cvref_t<
    decltype(reflect::get<I>(std::declval<T&>()))
>;

template<typename T>
consteval auto make_attrs(){
    constexpr auto attr_count = reflect::size<T>();
    std::array<VertexAttribute, attr_count> attrs{};
    reflect::for_each<T>([&](auto I){
        constexpr std::size_t location = decltype(I)::value;
        using field_t = field_type_t<location, T>;
        constexpr auto offset = std::uintptr_t{reflect::offset_of<location,T>()};
        attrs[location] = make_attr_t<field_t, location, offset>();
    });
    return attrs;
}
template<typename T>
consteval auto make_layout(){
    constexpr auto attr_count = reflect::size<T>();
    return VertexLayout<attr_count>{
        .stride = sizeof(T),
        .attrs = make_attrs<T>()
    };
}
// Vertex layouts which use instancing require setting the divisor of each field. 
// They must use the old static constexpr auto layout() api
template<typename T>
concept has_layout_member_fn = requires(T v){
    T::instanced_layout(); 
};
template<typename T>
static constexpr auto non_instanced_vtx_layout = make_layout<T>();

template<typename T>
constexpr void apply_vertex_layout(){
    if constexpr(has_layout_member_fn<T>){
    // Vertex layouts which use instancing require setting the divisor of each field.. 
        // The factory function make_layout<T>() cannot account for this.
    // Thus, we must use a member function. We could also make explicit exceptions of each.
        apply_layout(T::instanced_layout());
    }else{
        apply_layout(non_instanced_vtx_layout<T>);
    }
}
