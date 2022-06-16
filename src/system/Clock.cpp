#include "Clock.h"

#include<Arduino.h>

uint32_t Clock::millis() {
    return ::millis();
}

uint32_t Clock::micros() {
    return ::micros();
}