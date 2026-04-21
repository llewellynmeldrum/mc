#pragma once 

#include "Types.h"
#include <memory>

struct Window;

struct App{
public:
    u64 frameCount = 0;
    std::unique_ptr<Window> win;
    void setup();
    void loop();
    void exit(i32 exit_code);

    bool shouldClose();
private:
};
