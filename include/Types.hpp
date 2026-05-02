#pragma once
// c++ specific types
#include <unordered_map>
template <class K, class V, class H = std::hash<K>>
using HashMap = std::unordered_map<K, V, H>;
