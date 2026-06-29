#include "Engine.hpp"
//#define TESTING_SOMETHING
#include "preamble.hpp"
int TEST_MAIN(){
    BREAKPOINT();
    return 0;
}

int MAIN(int argc, char** argv) {
    Engine eng{};
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


