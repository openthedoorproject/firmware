#include <Arduino.h>
#include <stdint.h>
#include <Wifi.h>

#include "Password.h"
#include "PasswordParser.h"
#include "PasswordValidator.h"

#include "interface/card/CardData.h"
#include "interface/card/CardReader.h"
#include "interface/LCDDisplay.h"
#include "interface/LEDInterface.h"
#include "interface/Keyboard.h"

#include "CardValidator.h"

#include "Door.h"

#include "system/System.h"
#include "system/Clock.h"

enum State
{
    CARD_DETECTED,
    KEYBOARD_INPUT_DETECTED,
    CHECK_UPDATE,
    START_WAITING,
    WAITING,
    INVALID_CARD,
    INVALID_PASSWORD
};

const char* ssid = "nomeDoWifi";
const char* passwordWifi= "senhaDoWifi";

const uint32_t USER_INPUT_WAIT_TIME = 3000;                     // 3 seconds
const uint32_t UPDATE_CHECK_TIME_INTERVAL = 3 * 60 * 60 * 1000; // 3 hours interval

uint32_t lastUpdateCheckTime;
State state;

Door door(12, false);
LEDInterface leds(4, 5, 6);
LCDDisplay display({7, 8, 9, 10});
Keyboard<4, 3> key({1, 2, 3, 4}, {6, 7, 8});

PasswordParser<5> passwordParser(key, USER_INPUT_WAIT_TIME);

CardReader cardReader;

void setState(State newState)
{
    state = newState;
}

void toDefaultState()
{
    setState(START_WAITING);
    leds.turnOff();
}

void setup()
{
    Wifi.begin(ssid, passwordWifi);
    Serial.begin(9600);

    while(Wifi.status() != WL_CONNECTED)){
        delay(1000);
        Serial.println("Conectando wifi");
    }

    Serial.println("Wifi conectado");
    toDefaultState();
}

void updateCheck()
{
    display.clear();
    display.write("updating system");

    System::update();
}

void readCard()
{
    CardData card = cardReader.readCard();

}

uint32_t getTime()
{
    return Clock::millis();
}

void readPassword()
{
    display.write("Insira a senha:");

    passwordParser.parse();

    if (passwordParser.isValid())
    {
        // TODO: check password
    }
    else
    {
        setState(State::INVALID_PASSWORD);
    }
}

void openDoor()
{
    leds.turnOff();
    leds.setOk(true);
    door.open();
    System::wait(1000);
    door.close();
    toDefaultState();
}

void evalState()
{

    switch (state)
    {

    case State::START_WAITING:

        display.clear();
        display.write("Open The Door");

        setState(WAITING);
        break;

    case WAITING:

        if (cardReader.hasCardOnRange())
        {
            setState(State::CARD_DETECTED);
        }
        else if (key.hasNext())
        {
            setState(State::KEYBOARD_INPUT_DETECTED);
        }
        else if (getTime() - lastUpdateCheckTime >= UPDATE_CHECK_TIME_INTERVAL)
        {
            setState(CHECK_UPDATE);
        }

        break;

    case State::CARD_DETECTED:
        readCard();
        break;

    case State::KEYBOARD_INPUT_DETECTED:
        readPassword();
        break;

    case CHECK_UPDATE:
        updateCheck();

        break;

    case INVALID_CARD:
        leds.turnOff();
        leds.setError(true);

        display.clear();
        display.write("cartão invalido");
        System::wait(1000);
        break;

    case INVALID_PASSWORD:
        leds.turnOff();
        leds.setError(true);

        display.clear();
        display.write("senha inválida");

        System::wait(1000);
        break;
    }
}

void loop()
{

    evalState();
}
