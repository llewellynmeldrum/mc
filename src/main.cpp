#include "FmtStyle.hpp"
#include "LM.hpp"
//#define TESTING_SOMETHING
#include "Breakpoints.hpp"
#include "Engine.hpp"
#include "preamble.hpp"

#define TEST_FN(fn, ...) test_fn(#fn, fn __VA_OPT__(,) __VA_ARGS__)

template<typename Fn, typename ...Args>
void test_fn(std::string_view fn_name, Fn&& fn, Args... args){
    using namespace std;
    using namespace fmt;
    bool first = true;
    print( "{}(",fn_name);
    ((std::print("{}{}", first ? "" : ", ", std::forward<Args>(args)), first = false), ...);
    println(")->{}", std::invoke(fn,std::forward<Args>(args)...));
}

int TEST_MAIN(){
    return 0;
}

int MAIN(int argc, char** argv) {
    Engine eng{};
    eng.setup();
    eng.loop();
    return eng.exit(EXIT_SUCCESS);
}


