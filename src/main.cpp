
#define _DEBUG

#include "FormatSpecs.hpp"
#include "App.hpp"





//#define TESTING_SOMETHING
#include "preamble.hpp"
int TEST_MAIN(){
    BREAKPOINT();
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


