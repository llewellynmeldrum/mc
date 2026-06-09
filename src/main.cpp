
#include "DebugFormatSpecializations.hpp"

#include "AnsiCodes.hpp"
#include "App.hpp"
#include <print>
#include "Assertion.hpp"
#include "CommonUtils.hpp"
#define _DEBUG

//#define TESTING_SOMETHING

#if defined(TESTING_SOMETHING)
    #define MAIN __fake_entry_point__
    #define TEST_MAIN main
#else 
    #define MAIN main
    #define TEST_MAIN __fake_entry_point__
#endif




int TEST_MAIN(){
    std::println("NOTICE!!!! CURRENTLY IN TESTING MODE!!!");
    glm::ivec3 a{1,2,3};
    glm::ivec3 b{2,2,2};
    assert(1==1, "this is false");
    assert_eq(a,a);
    assert_eq(a,b);

    assert(2==1, "this is false");


    std::exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}


int MAIN(int argc, char** argv) {
    App app{};
    app.setup();
    while (!app.shouldClose()) {
        app.loop();
    }
    return app.exit(EXIT_SUCCESS);
}


