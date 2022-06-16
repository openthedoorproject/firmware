#include "Timer.h"
#include "Clock.h"

Timer::Timer(uint32_t waitTime, Timer::CallFunction function, bool startOnCreation) : waitTime(waitTime), function(function)
{
    if(startOnCreation) {
        Timer::start();
    }
}

void Timer::start() {
    if(started)
        return;
    
    startTime = Clock::millis();
    started = true;
}

void Timer::tick()
{
    if(!started)
        return;

    if(Clock::millis() - startTime >= waitTime) {
        function();
        startTime = Clock::millis();
    }
}