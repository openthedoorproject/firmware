#pragma once

#include <Arduino.h>

namespace PasswordParser
{
    enum ParsingState {
        STARTING,
        READING_INPUT,
        ENDED,
        ERROR
    };

    void SetPasswordSize(int size);

    void SetTimeout(long timeout);

    ParsingState Parse();

    void Reset();

    String GetParsedPassword();

} // namespace PasswordParser
