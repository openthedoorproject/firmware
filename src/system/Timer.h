#pragma once
#include <inttypes.h>

class Timer
{

private:
    typedef void (*CallFunction)();

    bool started;
    uint32_t startTime;
    uint32_t waitTime;
    CallFunction function;

public:
    Timer(uint32_t waitTime, CallFunction function, bool startOnCreation);

    Timer(uint32_t time, CallFunction function);

    void start();

    void tick();
};