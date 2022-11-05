#pragma once

namespace Keyboard {

    const char NULL_CHAR = '\0';

    void Init(int rowCount, int columnCount, const int *rowPins, const int* columnPins);

    void SetRow(int row, char* characters);

    bool HasInput();

    char GetPressed(bool blocking = false);
}