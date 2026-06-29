//#define TESTING_SOMETHING
#include "Breakpoints.hpp"
#include "Engine.hpp"
#include "preamble.hpp"
int TEST_MAIN(){
    BREAKPOINT();
    return 0;
}

int MAIN(int argc, char** argv) {
    Simulation eng{};
    eng.setup();
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


