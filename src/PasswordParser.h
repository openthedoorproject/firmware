#pragma once

#include <stdint.h>
#include "Password.h"
#include "CharStream.h"

template<uint16_t passwordLenght>
class PasswordParser {

private:
    
    const uint32_t waitTime;
    CharStream& charStream;
    Password<passwordLenght> password;
    uint16_t bufferSize = 0;
    bool valid;

    static bool validCharacter(char input);

    void append(char newChar);

    bool isFilled();

    void setValid(bool valid);

public:

    PasswordParser(CharStream& input, uint32_t waitTime);

    bool isValid();

    void parse();

    void clearBuffer();

    Password<passwordLenght> getPassword();
};