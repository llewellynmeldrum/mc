#pragma once
// c++ specific types
#include <unordered_map>
template <class K, class V, class H>
using HashMap = std::unordered_map<K, V, H>;
