#pragma once
#include <Arduino.h>

template<uint8_t size>
struct Password {

    char content[size];

    inline uint8_t getSize() {
        return size;
    }

    char& operator[] (uint16_t index) {
        return content[index];
    }

    operator String() {
        return content;
    }

};
