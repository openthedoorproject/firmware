#pragma once

#include<stdint.h>

class CharStream {

public:

    const static char NULL_CHAR = 0;

    virtual char nextChar() = 0;

    virtual char waitNextChar(uint32_t millis) = 0;

};