#pragma once

#include<inttypes.h>

class LEDInterface {

private:
    const uint8_t okPin;
    const uint8_t alertPin;
    const uint8_t errorPin;

    void setPin(uint8_t pin, bool state);

public:

    LEDInterface(uint8_t okPin, uint8_t alertPin, uint8_t errorPin);

    void turnOff();

    inline void setAlert(bool state) {
        setPin(alertPin, state);
    }

    inline void setError(bool state) {
        setPin(errorPin, state);
    }

    inline void setOk(bool state) {
        setPin(okPin, state);
    }

};