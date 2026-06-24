#pragma once 

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
    #error "Occasional builtins"
#endif
