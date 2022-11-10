#pragma once

#include <Arduino.h>

namespace Display {

    void Init(int address, int rows, int columns);

    void Write(const String& text);

    void Write(char character);

    void Cursor(int row, int column);

    void Clear();

    void Off();

    void On();
}