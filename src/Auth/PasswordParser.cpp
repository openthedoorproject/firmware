#include "PasswordParser.h"

#include "Logger.h"
#include "Keyboard/Keyboard.h"

static int passwordSize = 4;
static char *inputBuffer;
static int bufferSize = 0;

static long timeout = 2000; // 2 seconds
static long lastKeyPressedTime;

static PasswordParser::ParsingState CurrentState = PasswordParser::STARTING;

static void changeToState(PasswordParser::ParsingState newState, bool fastSwitch = false)
{
    if ((CurrentState & PasswordParser::ENDED) && (newState & PasswordParser::READING_INPUT))
    {
        LOG_ERROR("illegal state change, from ENDED to READING_INPUT");
        return;
    }

    CurrentState = newState;
    if (fastSwitch)
    {
        LOG_INFO("fast switching state");
        PasswordParser::Parse();
    }
}

static void cleanData()
{
    if (inputBuffer)
        delete[] inputBuffer;

    bufferSize = 0;
}

static void prepareToRead()
{
    inputBuffer = new char[passwordSize + 1];
    inputBuffer[passwordSize] = '\0';

    lastKeyPressedTime = millis();

    changeToState(PasswordParser::READING_INPUT, true);
}

static void readInput()
{

    if (millis() - lastKeyPressedTime >= timeout)
    {
        changeToState(PasswordParser::ERROR);
        return;
    }

    if (!Keyboard::HasInput())
    {
        changeToState(PasswordParser::NONE);
        return;
    }

    char letter = Keyboard::GetPressed(true);
    inputBuffer[bufferSize] = letter;

    bufferSize++;
    lastKeyPressedTime = millis();

    if (bufferSize >= passwordSize)
    {
        changeToState(PasswordParser::ENDED);
    }
    else
    {
        changeToState(PasswordParser::ACCEPTED);
    }
}

static void prepareToRestart()
{
    LOG_INFO("restarting parser");
    cleanData();
    changeToState(PasswordParser::STARTING);
}

void PasswordParser::SetPasswordSize(int size)
{
    passwordSize = size;
}

void PasswordParser::SetTimeout(long timeout)
{
    ::timeout = timeout;
}

PasswordParser::ParsingState PasswordParser::Parse()
{
    switch (CurrentState)
    {
    case STARTING:
        prepareToRead();
        break;

    case READING_INPUT:
    case ACCEPTED:
    case NONE:
        readInput();
        break;

    case ERROR:
        LOG_INFO("recovering from error");

    case ENDED:
        prepareToRestart();
    }

    return CurrentState;
}

String PasswordParser::GetParsedPassword()
{
    if (CurrentState != ENDED)
    {
        LOG_WARN("trying to get password before parsing is finished");
    }

    inputBuffer[bufferSize] = '\0';

    return String(inputBuffer);
}