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
    app.loop();
    return app.exit(EXIT_SUCCESS);
}


