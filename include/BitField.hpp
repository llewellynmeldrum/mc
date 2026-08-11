#pragma once 
#include "Types.h"
#include "qlibs-mp.hpp"
template<size_t bitwidth, bool is_signed>
class ubits{
    static_assert(bitwidth<8);
    using setter_type = u32;
    using storage_type = std::conditional_t<is_signed,int8_t,uint8_t>;
public:
    constexpr ubits(setter_type val) {
        *this = val; 
    }
    
    constexpr ubits& operator=(setter_type rhs) noexcept{
        data = rhs & bitmask();
        return *this;
    }
    
    constexpr setter_type bitmask(){
        return (~(0u)) >> bitwidth;
    }

    // Implicit conversion
    operator setter_type() const noexcept{
        return data; 
    } 

private:
    storage_type data : bitwidth; 
};

using u7 = ubits<7, false>;
using u6 = ubits<6, false>;
using u5 = ubits<5, false>;
using u4 = ubits<4, false>;
using u3 = ubits<2, false>;
using u2 = ubits<2, false>;
using u1 = ubits<1, false>;

using i7 = ubits<7, true>;
using i6 = ubits<6, true>;
using i5 = ubits<5, true>;
using i4 = ubits<4, true>;
using i3 = ubits<2, true>;
using i2 = ubits<2, true>;
using i1 = ubits<1, true>;

// helper template to resolve the field type
template<size_t n_bits>
struct field_type_resolver{
    using type = 
        std::conditional_t<                 n_bits<=8,  u8,
            std::conditional_t<             n_bits<=16, u16,
                std::conditional_t<         n_bits<=32, u32, 
                    std::conditional_t<     n_bits<=64, u64, 
                                            nullptr_t
                    >
                >
            >
        >;
};

// helper fn to get the bitmask
template<typename storage_type, size_t n_bits, size_t storage_offset>
static consteval storage_type get_bitmask(){
    storage_type res{0};
    for (storage_type i = 0; i<n_bits; i++){
        res |= (1 << (i+storage_offset));
    }
    return res;
}
template <typename T>
constexpr std::size_t bit_width = sizeof(T) * CHAR_BIT; 

template<typename field_type, size_t t_n_bits>
concept field_type_ok = !std::same_as<field_type,nullptr_t>;
template<typename F>
static constexpr auto is_mask_contiguous = std::countr_one(u64(F::mask >> F::storage_offset))  == std::popcount(F::mask);
template<size_t t_storage_offset, size_t t_n_bits, typename t_storage_type=u32>
struct BitFieldMember{
    static constexpr u64 n_bits = t_n_bits;
    static constexpr u64 storage_offset = t_storage_offset;
    using storage_type = t_storage_type;
    using field_type = field_type_resolver<n_bits>::type;
    static_assert(field_type_ok<field_type,n_bits>);


    static constexpr storage_type mask = get_bitmask<storage_type,n_bits,storage_offset>();
    
    static constexpr field_type get(storage_type const& store){
        return static_cast<field_type>(
            ( store & mask ) >> storage_offset
            // ^ reset others
        );
    }
    static constexpr storage_type get_unshifted(storage_type const& store){
        return  ( store & mask ); // ^ reset others
    }
    static constexpr void set(storage_type & store, field_type val){
        store &= ~(mask);      // clear out whatever might reside in the fields new home
        store |= ((static_cast<storage_type>(val)<<storage_offset)&mask);         // apply the field to store
    }
    static_assert(bit_width<t_storage_type> > t_storage_offset, "Field offset is out of bounds for storage type!");
    static_assert(bit_width<t_storage_type> >= t_storage_offset+t_n_bits, "Field overflows storage type!");
    static_assert(is_mask_contiguous<BitFieldMember>);
};

// NOTE: 
// Automatically creates a more convinient getter and setter for each member
#define BITFIELD_MEMBER(name, offset, len, ...) \
static constexpr BitFieldMember<offset, len, storage_type> name;\
constexpr auto get_##name()const noexcept{return name .get(store);}\
constexpr auto set_##name(decltype(name)::field_type v)noexcept{return name .set(store,v);}

struct BitField{
    using storage_type = u16;
    storage_type store{0};

    #include "BitFieldMembers.def"
};


template <typename T>
concept bitfield_like = requires(T v){
    std::unsigned_integral<typename T::storage_type>;
    {v.store} -> std::same_as<typename T::storage_type&>;
};

static_assert(bitfield_like<BitField>);

inline void test_bitfield(){
    static BitField packed0      {0B1011'1010'1010'1010U};
//    res &= packed.foo.get(packed.store) ==           0B0110'0000U;
//    packed.bar.set(packed.store, 0B1110);
    assert(packed0.baz.get(packed0.store) == 0B10111);
    assert(packed0.bar.get(packed0.store) == 0B010);
    assert(packed0.foo.get(packed0.store) == 0B1010'1010);
    assert(packed0.get_bar() == 0B010);
    assert(packed0.foo.get(packed0.store) == 0B1010'1010);
    static BitField packed{};
    packed.bar.set(packed.store, 0b111);
    assert(packed.bar.get(packed.store) == 0B111);
    packed.foo.set(packed.store, 0b0000'0000);
    assert(packed.bar.get(packed.store) == 0B111);
    packed.foo.set(packed.store, 0b1010'1010);
    assert(packed.foo.get(packed.store) == 0B1010'1010);
    std::println("Bitfield passed all runtime tests.");
}




// TODO: When c++26 reflection comes out for clang, i can complete this function. I need a list of static members, but qlibs cant provide that unfortunately
// template<typename bit_field>
// static consteval bool verify_bitfield(){
//     // 1st check: overlapping masks
//     // you could go through, ANDING them all together, and checking to see if the bit difference equals the bitlength of the newly added field.
//     // For example:
//     // a=00011 & b=11100 -> b n_bits = 3, 
//     //                  a popcount = 2, 
//     //                  a&b popcount = 5
//     //                  popcount(a&b)-n_bits(b) = 2, valid
//     //                  5-3 = 2, valid
//     // 00111 & 11100 -> field1 bitlength = 3
//     //                  field2 bitlength = 3, 
//     //                  00011 popcount = 3, 
//     //                  11111 popcount = 5
//     //                  5-(field2 popcount) -> 5-3 = 2, and 2 != field2 bitlength, therefore data was lost
//     //
//     //
//     // Check for overlapping masks
//     using storage_type = bit_field::storage_type;
//     using T = bit_field;
//     reflect::for_each<bit_field>([&](auto I) {
//         if (I >= reflect::size<bit_field>()-1) return; // skip last one 
//         auto const& a_field = reflect::get<I,bit_field>();
//         auto const& b_field = reflect::get<I+1,bit_field>();
//         storage_type a_b= a_field.mask & b_field.mask;
//         static_assert(std::popcount(a_b) - decltype(b_field)::n_bits);
//     });
// //    for (auto i = 0uz; i<field_count; i++){
// //
// //    }
// //    // 2nd check: bit contiguous masks
// //    auto is_mask_contiguous = [](){
// //    };
// //    static_assert(std::countl_one(a) == std::popcount(a));
//     // To find overlapping masks, XOR all the members and 
//     return true;
// }
// static_assert(verify_bitfield<BitField>());

