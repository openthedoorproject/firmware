#include "Display.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>


static LiquidCrystal_I2C* lcd;

void Display::Init(int address, int rows, int colums) {
    Wire.begin();
    
    lcd = new LiquidCrystal_I2C(address, rows, colums);
    lcd->init();

    On();
    lcd->noCursor();
}

void Display::Write(const String& text) {
    lcd->print(text);
}

void Display::Write(char character) {
    lcd->print(character);
}

void Display::Cursor(int row, int column) {
    lcd->setCursor(column, row);
}

void Display::Clear() {
    lcd->clear();
}

void Display::Off() {
    lcd->noBacklight();
}

void Display::On() {
    lcd->backlight();
}