#pragma once

#include<inttypes.h>
#include<Arduino.h>

class LCDDisplay {

public:

    LCDDisplay(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);

    void write(uint8_t row, uint8_t column, const String& message);

    inline void write(const String& message) {
        write(0, 0, message);
    }

    void clear();

};