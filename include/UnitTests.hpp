#pragma once 

#include "Types.h"
#include "Logger.hpp"
#include <source_location>
using SrcLoc = std::source_location;

using namespace std;
static u32 pass_count = 0;
static u32 fail_count = 0;

std::string current_test_name = "None";
#define TEST(name) (current_test_name=name);

template<typename T>
concept HasComparisonOverload = requires(T a, T b){
    {a==b} -> std::same_as<bool>;
};

template<typename T>
requires HasComparisonOverload<T>
bool ASSERT_EQ(const T a, const T b, const SrcLoc loc = SrcLoc::current()){
    if (a==b){
        pass_count++;
        return true;
    } else {
        fail_count++;
        print("{}",fmt::red);
        println("TEST FAILURE! Test {}'{}'{} failed in:",fmt::clear, current_test_name, fmt::red);
        println("{} -> {}:{}", loc.function_name(), loc.file_name(), loc.line());
        return false;
    }
}

template<typename T>
concept isBoolean= std::same_as<T, bool>;
template<isBoolean T>
bool ASSERT(const T&& expr,  const SrcLoc loc = SrcLoc::current()){
    if (expr){
        pass_count++;
        return true;
    } else {
        fail_count++;
        print("{}",fmt::red);
        println("TEST FAILURE! Test {}'{}'{} failed in:",fmt::clear, current_test_name, fmt::red);
        println("{} -> {}:{}", loc.function_name(), loc.file_name(), loc.line());
        return false;
    }
}




#include "LinearAlgebra.hpp"
static inline void TEST_VEC2(){
    TEST("default ctor zeros elements"){
        vec2 a = vec2();
        ASSERT(a == vec2{0,0});
    }

}

static inline void RUN_TESTS(){
    TEST_VEC2();
    if (fail_count){
        print("{}",fmt::red);
    } else {
        print("{}",fmt::green);
    }
    println("[{}/{}] tests passed.", pass_count,pass_count+fail_count);
}


