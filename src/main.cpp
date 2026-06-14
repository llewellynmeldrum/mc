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

#include "NothrowLookup.hpp"


#include "DebugFormatSpecializations.hpp"

#include "AnsiCodes.hpp"
#include "App.hpp"
#include "LM.hpp"
#include <print>
#include "Assertion.hpp"
#include "CommonUtils.hpp"

#include "SlotMap.hpp"

int TEST_MAIN(){
    slot_map<char, const char*> map;
    map.insert_or_assign('b', "Beta");
    map.insert_or_assign('a', "FAIL");
    map.insert_or_assign('a', "Apple");

    map.insert_or_assign('d', "Delta");
    map.insert_or_assign('c', "Charlie");
    map.erase('b');
    std::println("{}",map);
    auto sorted_keys = map.sorted_keys([](auto a, auto b){
        return a<b;
    });
    for (const auto& key: sorted_keys){
        std::println("({},{})",key,map.at(key));
    }

    // contains
    // some type of sorted/unsorted iteration.
    return 0;
}



int MAIN(int argc, char** argv) {
    App app{};
    app.setup();
    while (!app.shouldClose()) {
        app.loop();
    }
    return app.exit(EXIT_SUCCESS);
}


