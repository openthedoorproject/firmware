#pragma once

#include<stdint.h>

class Door {

private:
    const uint8_t pin;
    bool closed;

public:
    Door(uint8_t pin, bool closed);

    Door(uint8_t pin);

    bool isClosed();

    uint8_t getPin();

    void close();

    void open();

};