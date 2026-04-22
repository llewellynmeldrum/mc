#pragma once 

#include "Context.hpp"
#include "Types.h"
#include <memory>

struct Window;

struct App{
    App()=default;
    ~App()=default;
public:
    u64 frameCount = 0;
    void setup();
    void loop();
    void exit(i32 exit_code);

    Context ctx;
    bool shouldClose();
private:
};
