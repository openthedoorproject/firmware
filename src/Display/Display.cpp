#include "Display.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>


static LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void Display::Init(int address, int rows, int colums) {
    Wire.begin();
    
    lcd.init();

    On();
    lcd.noCursor();
}

void Display::Write(const String& text) {
    lcd.print(text);
}

void Display::Cursor(int row, int column) {
    lcd.setCursor(row, column);
}

void Display::Clear() {
    lcd.clear();
}

void Display::Off() {
    lcd.noBacklight();
}

void Display::On() {
    lcd.backlight();
}