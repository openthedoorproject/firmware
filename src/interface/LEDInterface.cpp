#include "LEDInterface.h"

#include <Arduino.h>

LEDInterface::LEDInterface(uint8_t okPin, uint8_t alertPin, uint8_t errorPin): okPin(okPin), alertPin(alertPin), errorPin(errorPin) {

}

void LEDInterface::setPin(uint8_t pin, bool state) {
    digitalWrite(pin, state? HIGH : LOW);
}