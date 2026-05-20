#pragma once

template <class T>
concept is_glm_type = requires(T obj) {
    true;
    // TODO: idk how to do this properly
};

// returns a constant address to a glm object
template <class T>
    requires is_glm_type<T>
const auto data(T obj) {
    return glm::value_ptr(obj);
}

inline auto operator<=>(const glm::ivec3& lhs, const glm::ivec3& rhs){
    auto cmp_x = lhs.x<=>rhs.x;
    if ( cmp_x != 0) return cmp_x; // NOLINT

    auto cmp_y = lhs.y<=>rhs.y;
    if ( cmp_y != 0) return cmp_y; // NOLINT

    return lhs.z<=>rhs.z;
}
