#include "App.hpp"
#include <print>
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


