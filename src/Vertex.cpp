#include "Vertex.hpp"
#include "SharedShaderConfig.hpp"
#include "BlockAmbientOcclusion.hpp"
#include "AONeighbours.hpp"
#include "Types.h"
#include <type_traits>
#define AOState_NONE        0x00 // 0b00
#define AOState_ONE_SIDE    0x10 // 0b01
#define AOState_ONLY_CORNER 0x01 // 0b10
#define AOState_BOTH_SIDES  0x03 // 0b11
// ao has 4 states per vertex
// none -> 00     <- zero ambient occlusion
// c   ->  01     <- Least ambient occlusion
// a|b ->  10     <- medium ambient occlusion
// a&b ->  11     <- most ambient occlusion

//constexpr u2 Vertex::pack_ao_state(AONeighbours const& n){
//    auto pred = [](Block a){
//        return !a.is_air();
//    };
//    if (pred(n.a) && pred(n.b)) return AOState_BOTH_SIDES; // both
//    if (pred(n.corner)) return AOState_ONLY_CORNER; // corner
//    if (pred(n.a) || pred(n.b)) return AOState_ONLY_CORNER; // or one side
//    return AOState_NONE;
//    // else none
//}
//constexpr void Vertex::set_ao_state(AONeighbours const& n){
//    packed_0 &= ~(AO_STATE_MASK); // clear out whatever might reside in the fields new location
//    u2 input = pack_ao_state(n) & (AO_STATE_MASK);
//    packed_0 &= input; // apply the field, constricting the input to the proper range
//}
//


// it would be really good if i could describe a class at compile time which has some bitfield members,
// and it automatically generated setters and getters for me.
// like:

