#include "Door.h"
#include <Arduino.h>

Door::Door(uint8_t pin, bool closed): pin(pin), closed(closed) {
}

Door::Door(uint8_t pin): Door(pin, true) {

}

bool Door::isClosed() {
    return Door::closed;
}

void Door::open() {
    if(!isClosed())
        return;
    
    digitalWrite(pin, LOW);
}

void Door::close() {
    if(isClosed())
        return;

    digitalWrite(pin, HIGH);
}

uint8_t Door::getPin() {
    return Door::pin;
}