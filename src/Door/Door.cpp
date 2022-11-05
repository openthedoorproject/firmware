#include "Door.h"

#include "Logger.h"
#include <Arduino.h>

static int doorLockPort;
static int openButtonPort;
static bool highOnClose;

static bool isOpened;
static bool openButtonBeingPressed;

static long openTimeout = 0;
static long openTime = 0;

static void open()
{
    LOG_INFO("opening door");
    digitalWrite(doorLockPort, highOnClose ? LOW : HIGH);
    isOpened = true;
}

static void close()
{
    LOG_INFO("closing door");
    digitalWrite(doorLockPort, highOnClose ? HIGH : LOW);
    isOpened = false;
}

void Door::Init(int doorLockPort, int openButtonPort)
{
    ::doorLockPort = doorLockPort;
    ::openButtonPort = openButtonPort;

    openButtonBeingPressed = false;

    SetHighOnClose(false);

    pinMode(doorLockPort, OUTPUT);
    pinMode(openButtonPort, INPUT);

    close();
}

bool Door::OpenButtonPressed()
{
    bool pressed = digitalRead(openButtonPort) == HIGH? true : false;

    if(pressed) {
        openButtonBeingPressed = true;
    } else {
        if(openButtonBeingPressed) {
            openButtonBeingPressed = false;
            return true;
        }
    }

    return false;
}

void Door::SetOpenTimeout(long millisseconds)
{
    openTimeout = millisseconds;
}

bool Door::IsOpened()
{
    return isOpened;
}

void Door::SetHighOnClose(bool state)
{
    highOnClose = state;
}

bool Door::GrantAccess()
{
    if (!IsOpened())
    {
        open();
        openTime = millis();

        return true;
    }

    if (millis() - openTime >= openTimeout)
    {
        close();
        return false;
    }

    return true;
}