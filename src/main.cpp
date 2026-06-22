#include <random>
#define _DEBUG

//#define TESTING_SOMETHING

#if defined(TESTING_SOMETHING)
    #define MAIN __fake_entry_point__
    #define TEST_MAIN main
#else 
    #define MAIN main
    #define TEST_MAIN __fake_entry_point__
#endif

#if defined(_MSC_VER)
    #error "This project makes use of unix specific syscalls, and gcc/clang exclusives."
    #error "These gcc/clang exclusives include:" 
    #error "-> \\e for escape sequences (equivalent on msvc to \x1b)"
    #error "-> Parsing of the __PRETTY_FUNCTION__ macro to get pretty typenames"
#endif

#include "FmtStyle.hpp"
#include "NothrowLookup.hpp"


#include "FormatSpecs.hpp"

#include "FmtStyle.hpp"
#include "App.hpp"
#include "LM.hpp"
#include <print>
#include "Assertion.hpp"
#include "CommonUtils.hpp"

#include "SlotMap.hpp"

int TEST_MAIN(){
    return 0;
}



int MAIN(int argc, char** argv) {
    App app{};
    app.setup();
    if (g_StyleConfig::isEnabled()){
        std::println("Debugger detected, disabling ansi styling.");
    }
    while (!app.shouldClose()) {
        app.loop();
    }
    return app.exit(EXIT_SUCCESS);
}


