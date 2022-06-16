#pragma once
#include<stdint.h>
#include <Arduino.h>
#include "CharStream.h"

template <uint16_t rowCount, uint16_t columnCount>
class Keyboard : public CharStream
{

private:
    uint32_t rowPins[rowCount];
    uint32_t columnPins[columnCount];
    char keys[rowCount][columnCount];

public:

    Keyboard(const uint32_t (&rowPins)[rowCount], const uint32_t (&columnPins)[columnCount]);

    char getKey(uint16_t row, uint16_t column);

    void setKey(uint16_t row, uint16_t column, char key);

    void setRow(uint16_t index, const char (&keys)[columnCount]);

    void setKeyMap(const char (&keys)[rowCount][columnCount]);

    char nextChar();

    bool hasNext();

    char waitNextChar(const uint32_t millis);

};

#include "Keyboard.cpp"