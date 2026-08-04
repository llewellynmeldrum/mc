
#include <thread>

#include "FmtStyle.hpp"
#include "LM.hpp"
// #define TESTING_SOMETHING
#include "Breakpoints.hpp"
#include "Engine.hpp"
#include "preamble.hpp"
#include "ThreadTracker.hpp"
const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();

#define TEST_FN(fn, ...) test_fn(#fn, fn __VA_OPT__(,) __VA_ARGS__)



#include "SharedShaderConfig.hpp"
int TEST_MAIN(){
    // test bitwise stuff
    u32 p{};
    // 1. Ensure all return 0 
    assert_eq(0, get_blocklight_r(p));
    assert_eq(0, get_blocklight_g(p));
    assert_eq(0, get_blocklight_b(p));
    assert_eq(0, get_face_opacity(p));
    assert_eq(0, get_tex_atlas_id(p));
    assert_eq(0, get_face_dir(p));


    auto random_0_to = [](auto max){
        return LM::random(0,max) ;
    };
    auto modify_random_field = [](u32& p){
        u8 modified_val{};
        // 0. select which field will be modified
        size_t idx = LM::random(0,5);
        // 1. get the modified val based on the max of that field
        switch(idx){
            case 0: modified_val = LM::random(0u,BLOCKLIGHT_R_MAX); break; 
            case 1: modified_val = LM::random(0u,BLOCKLIGHT_G_MAX); break; 
            case 2: modified_val = LM::random(0u,BLOCKLIGHT_B_MAX); break; 
            case 3: modified_val = LM::random(0u,FACE_OPACITY_MAX); break; 
            case 4: modified_val = LM::random(0u,TEX_ATLAS_ID_MAX); break; 
            case 5: modified_val = LM::random(0u,FACE_DIR_MAX    ); break; 
        };
        // 2. cache all field vals currently
        std::array<u32, 6> expected{
            get_blocklight_r(p),
            get_blocklight_g(p),
            get_blocklight_b(p),
            get_face_opacity(p),
            get_tex_atlas_id(p),
            get_face_dir(p)    ,
        };

        // 3. apply the random modification to idx
        expected[idx] = modified_val;
        switch(idx){
            case 0: set32_blocklight_r(p, modified_val); break; 
            case 1: set32_blocklight_g(p, modified_val); break; 
            case 2: set32_blocklight_b(p, modified_val); break; 
            case 3: set32_face_opacity(p, modified_val); break; 
            case 4: set32_tex_atlas_id(p, modified_val); break; 
            case 5: set32_face_dir    (p, modified_val); break; 
        };

        // 3. assert all the others remain the same and the modified one was modified to the new val
        std::array<u32, 6> current{
            get_blocklight_r(p),
            get_blocklight_g(p),
            get_blocklight_b(p),
            get_face_opacity(p),
            get_tex_atlas_id(p),
            get_face_dir(p)    ,
        };
        for (int i = 0; i<6; i++){
            assert_eq(expected[i], current[i]);
        }
    };
    for (int i = 0; i< 10000; i++){
        modify_random_field(p);
    }

    return 0;
}

int MAIN(int argc, char** argv) {
    ThreadTracker::init();
    TraceSettings::init();
    Engine eng{};
    eng.setup();
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


