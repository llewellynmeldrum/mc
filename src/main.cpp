
#include <thread>

#include "FmtStyle.hpp"
#include "LM.hpp"
//#define TESTING_SOMETHING
#include "Breakpoints.hpp"
#include "Engine.hpp"
#include "preamble.hpp"
#include "BitField.hpp"
#include "ThreadTracker.hpp"
const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();

#define TEST_FN(fn, ...) test_fn(#fn, fn __VA_OPT__(,) __VA_ARGS__)

inline void test_bitfield2() {
    // ------------------------------------------------------------
    // Known-value smoke test
    // ------------------------------------------------------------
    {
        BitField packed{0b1011'1010'1010'1010U};

        ASSERT(packed.baz.get(packed.store) == 0b10111, packed.store);
        ASSERT(packed.bar.get(packed.store) == 0b010,   packed.store);
        ASSERT(packed.foo.get(packed.store) == 0b1010'1010, packed.store);

        ASSERT(packed.get_bar() == 0b010);
    }

    // ------------------------------------------------------------
    // Masks themselves
    // ------------------------------------------------------------
    ASSERT(BitField::foo.mask == 0b0000'0000'1111'1111U);
    ASSERT(BitField::bar.mask == 0b0000'0111'0000'0000U);
    ASSERT(BitField::baz.mask == 0b1111'1000'0000'0000U);

    ASSERT(
        (BitField::foo.mask | BitField::bar.mask | BitField::baz.mask)
            == 0xffffU
    );

    ASSERT((BitField::foo.mask & BitField::bar.mask) == 0);
    ASSERT((BitField::foo.mask & BitField::baz.mask) == 0);
    ASSERT((BitField::bar.mask & BitField::baz.mask) == 0);

    // ------------------------------------------------------------
    // Exhaustively test GET against every possible u16 store.
    // 65536 iterations is tiny and gives excellent coverage here.
    // ------------------------------------------------------------
    for (u32 raw = 0; raw <= 0xffffU; ++raw) {
        BitField packed{static_cast<u16>(raw)};

        ASSERT(
            static_cast<u32>(packed.foo.get(packed.store))
                == ((raw >> 0) & 0xffU),
            "foo extraction failed",
            raw,
            packed.store
        );

        ASSERT(
            static_cast<u32>(packed.bar.get(packed.store))
                == ((raw >> 8) & 0x7U),
            "bar extraction failed",
            raw,
            packed.store
        );

        ASSERT(
            static_cast<u32>(packed.baz.get(packed.store))
                == ((raw >> 11) & 0x1fU),
            "baz extraction failed",
            raw,
            packed.store
        );
    }

    // ------------------------------------------------------------
    // Exercise a member's setter:
    //
    // 1. resulting field == requested value
    // 2. bits outside the field are unchanged
    // 3. get_unshifted() agrees with the raw representation
    //
    // Try every representable field value against several stores.
    // ------------------------------------------------------------
    auto test_member = []<typename M>(M) {
        constexpr u16 seeds[] = {
            0x0000,
            0xffff,
            0xaaaa,
            0x5555,
            0x1234,
            0x8001
        };

        constexpr u32 max_value =
            (u32{1} << M::n_bits) - 1;

        for (u16 seed : seeds) {
            for (u32 value = 0; value <= max_value; ++value) {
                u16 store = seed;
                const u16 before = store;

                M::set(
                    store,
                    static_cast<typename M::field_type>(value)
                );

                // Correct logical value comes back out.
                ASSERT(
                    static_cast<u32>(M::get(store)) == value,
                    "set/get round trip failed",
                    seed,
                    value,
                    store,
                    M::mask
                );

                // Setter must not modify unrelated bits.
                ASSERT(
                    (store & static_cast<u16>(~M::mask))
                        == (before & static_cast<u16>(~M::mask)),
                    "setter modified bits outside its field",
                    before,
                    store,
                    value,
                    M::mask
                );

                // Raw field position should also be correct.
                ASSERT(
                    M::get_unshifted(store)
                        == static_cast<u16>(
                            (value << M::storage_offset) & M::mask
                        ),
                    "get_unshifted failed",
                    seed,
                    value,
                    store,
                    M::mask
                );
            }
        }
    };

    test_member(BitField::foo);
    test_member(BitField::bar);
    test_member(BitField::baz);

    // ------------------------------------------------------------
    // Repeated writes: specifically ensure old bits get cleared.
    // ------------------------------------------------------------
    {
        BitField packed{0xffff};

        packed.bar.set(packed.store, 0b000);
        ASSERT(packed.bar.get(packed.store) == 0b000);
        ASSERT(packed.foo.get(packed.store) == 0xff);
        ASSERT(packed.baz.get(packed.store) == 0x1f);

        packed.bar.set(packed.store, 0b101);
        ASSERT(packed.bar.get(packed.store) == 0b101);

        packed.bar.set(packed.store, 0b010);
        ASSERT(packed.bar.get(packed.store) == 0b010);
    }

    // ------------------------------------------------------------
    // Test ubits truncation explicitly.
    // ------------------------------------------------------------
    {
        ubits<1, false> a{0xff};
        ubits<2, false> b{0xff};
        ubits<3, false> c{0xff};
        ubits<7, false> d{0xff};

        ASSERT(static_cast<u32>(a) == 0b1);
        ASSERT(static_cast<u32>(b) == 0b11);
        ASSERT(static_cast<u32>(c) == 0b111);
        ASSERT(static_cast<u32>(d) == 0b111'1111);
    }

    std::println("Bitfield passed all runtime tests.");
}

#include "SharedShaderConfig.hpp"
int TEST_MAIN(){
    test_bitfield();
    return 0;
}

int MAIN(int argc, char** argv) {
    test_bitfield2();
    ThreadTracker::init();

    TraceSettings::init();
    Engine eng{};
    eng.setup();
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


