#pragma once

#include <Arduino.h>

namespace PasswordParser
{
    enum ParsingState {
        STARTING = 0,
        ACCEPTED = 1,
        NONE = ACCEPTED << 1,
        READING_INPUT = ACCEPTED | NONE,
        ERROR = NONE << 1,
        ENDED = ERROR | (ERROR << 1)
    };

    void SetPasswordSize(int size);

    void SetTimeout(long timeout);

    ParsingState Parse();

    void Reset();

    String GetParsedPassword();

} // namespace PasswordParser
