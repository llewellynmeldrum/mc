#define TESTING_SOMETHING
#include "Breakpoints.hpp"
#include "Engine.hpp"
#include "preamble.hpp"
#
int TEST_MAIN(){
    LM::ieuclid_mod(18,CHUNK_XWIDTH);
    LM::ieuclid_mod(17,CHUNK_XWIDTH);
    LM::ieuclid_mod(16,CHUNK_XWIDTH);
    LM::ieuclid_mod(15,CHUNK_XWIDTH);
    BREAKPOINT();
    return 0;
}

int MAIN(int argc, char** argv) {
    Engine eng{};
    eng.setup();
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


