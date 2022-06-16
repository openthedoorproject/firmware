#include "PasswordParser.h"

template <uint16_t lenght>
bool PasswordParser<lenght>::validCharacter(char input)
{
    return input != CharStream::NULL_CHAR;
}

template <uint16_t lenght>
PasswordParser<lenght>::PasswordParser(CharStream &input, uint32_t waitTime) : charStream(input), waitTime(waitTime)
{
}

template <uint16_t lenght>
bool PasswordParser<lenght>::isFilled()
{
    return PasswordParser::size < lenght;
}

template <uint16_t lenght>
void PasswordParser<lenght>::setValid(bool valid)
{
    PasswordParser::valid = valid;
}

template <uint16_t lenght>
bool PasswordParser<lenght>::isValid()
{
    return valid;
}

template <uint16_t lenght>
void PasswordParser<lenght>::clearBuffer()
{
    PasswordParser::bufferSize = 0;
}

template <uint16_t lenght>
void PasswordParser<lenght>::append(char newChar)
{
    password[bufferSize] = newChar;
    bufferSize++;
}

template <uint16_t lenght>
void PasswordParser<lenght>::parse()
{
    setValid(false);
    while (!isFilled())
    {
        char c = charStream.waitNextChar(PasswordParser::waitTime);
        if (validCharacter(c))
        {
            append(c);
        }
        else
        {
            setValid(false);
            return;
        }
    }

    setValid(true);
}

template <uint16_t lenght>
Password<lenght> PasswordParser<lenght>::getPassword()
{
    return password;
}
