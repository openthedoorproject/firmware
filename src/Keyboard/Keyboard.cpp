#include "Keyboard.h"

#include "Logger.h"
#include <Arduino.h>

static int rowCount, columnCount;

static int *rowPins;
static int *columnPins;

static char *keys;

void Keyboard::Init(int rowCount, int columnCount, const int *rowPins, const int *columnPins)
{
    ::rowPins = new int[rowCount];
    ::columnPins = new int[columnCount];


    for (int i = 0; i < rowCount; i++)
    {
        ::rowPins[i] = rowPins[i];
        pinMode(rowPins[i], OUTPUT);
    }

    for (int i = 0; i < columnCount; i++)
    {
        ::columnPins[i] = columnPins[i];
        pinMode(columnPins[i], INPUT);
    }

    ::rowCount = rowCount;
    ::columnCount = columnCount;

    keys = new char[rowCount * columnCount];
}

static char getKey(int row, int column)
{
    return keys[row * columnCount + column];
}

void Keyboard::SetRow(int row, char *characters)
{
    for (int i = 0; i < columnCount; i++)
    {
        LOG_INFO("setting key (" + row + ", " + i + ") as " + characters[i]);
        keys[row * columnCount + i] = characters[i];
    }
}

char Keyboard::GetPressed(bool blocking)
{
    for (int row = 0; row < rowCount; row++)
    {
        digitalWrite(rowPins[row], LOW);
    }

    char input = Keyboard::NULL_CHAR;

    for (int row = 0; row < rowCount; row++)
    {
        int rowPin = rowPins[row];
        digitalWrite(rowPin, HIGH);

        for (int column = 0; column < columnCount; column++)
        {
            int columnPin = columnPins[column];

            if (digitalRead(columnPin) == HIGH)
            {
                while (blocking && digitalRead(columnPin) == HIGH)
                    ;
                LOG_INFO("pressed: " + row + ", " + column);
                input = getKey(row, column);
                LOG_INFO("pressed: " + input);
                break;
            }
        }

        digitalWrite(rowPin, LOW);

        if(input != Keyboard::NULL_CHAR)
            return input;
    }

    return Keyboard::NULL_CHAR;
}

bool Keyboard::HasInput()
{

    for (int y = 0; y < rowCount; y++)
    {
        int rowPin = rowPins[y];
        digitalWrite(rowPin, HIGH);
    }

    bool next = false;

    for (int column = 0; column < columnCount; column++)
    {
        int columnPin = columnPins[column];
        if (digitalRead(columnPin) == HIGH)
        {
            next = true;
            break;
        }
    }

    for (int y = 0; y < rowCount; y++)
    {
        int rowPin = rowPins[y];
        digitalWrite(rowPin, LOW);
    }

    return next;
}