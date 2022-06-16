#pragma once

#include "Keyboard.h"
#include "system/Clock.h"

template <uint16_t r, uint16_t c>
Keyboard<r, c>::Keyboard(const uint32_t (&rowPins)[r], const uint32_t (&columnPins)[c])
{

	for (uint16_t i = 0; i < r; i++)
	{
		Keyboard::rowPins[i] = rowPins[i];
		pinMode(rowPins[i], OUTPUT);
	}

	for (uint16_t i = 0; i < c; i++)
	{
		Keyboard::columnPins[i] = columnPins[i];
		pinMode(columnPins[i], INPUT);
	}
}

template <uint16_t r, uint16_t c>
char Keyboard<r, c>::getKey(uint16_t row, uint16_t column)
{
	return Keyboard::keys[row][column];
}

template <uint16_t r, uint16_t c>
void Keyboard<r, c>::setKey(uint16_t row, uint16_t column, char key)
{
	Keyboard::keys[row][column] = key;
}

template <uint16_t r, uint16_t c>
void Keyboard<r, c>::setRow(uint16_t row, const char (&keys)[c])
{
	for (int i = 0; i < c; i++)
	{
		Keyboard::keys[row][i] = keys[i];
	}
}

template <uint16_t r, uint16_t c>
void Keyboard<r, c>::setKeyMap(const char (&keys)[r][c])
{
	for (int y = 0; y < r; y++)
	{
		for (int x = 0; x < c; x++)
		{
			Keyboard::keys[y][x] = keys[y][x];
		}
	}
}

template <uint16_t r, uint16_t c>
char Keyboard<r, c>::nextChar()
{
	for(uint8_t rowPin : rowPins) {
		digitalWrite(rowPin, LOW);
	}

	for (uint8_t row = 0; row < r; row++)
	{
		uint8_t rowPin = rowPins[row];
		digitalWrite(rowPin, HIGH);

		for (uint8_t column = 0; column < c; column++)
		{
			uint8_t columnPin = columnPins[column];

			if (digitalRead(columnPin) == HIGH)
			{
				while (digitalRead(columnPin) == HIGH)
					;
				return Keyboard::getKey(row, column);
			}
		}

		digitalWrite(rowPin, LOW);
	}

	return Keyboard::NULL_CHAR;
}

template <uint16_t r, uint16_t c>
bool Keyboard<r, c>::hasNext()
{

	for(uint16_t y = 0; y < r; y++) {
		uint8_t rowPin = rowPins[y];
		digitalWrite(rowPin, HIGH);
	}

	bool next = digitalRead(columnPins[0]) == HIGH;

	for(uint16_t y = 0; y < r; y++) {
		uint8_t rowPin = rowPins[y];
		digitalWrite(rowPin, LOW);
	}

	return next;
}

template <uint16_t r, uint16_t c>
char Keyboard<r, c>::waitNextChar(uint32_t millis)
{
	uint32_t time = Clock::millis();
	uint32_t now = time;

	char inputKey;

	do
	{
		inputKey = Keyboard::nextChar();
		now = Clock::millis();

	} while (now - time <= millis && inputKey == Keyboard::NULL_CHAR);

	return inputKey;
}