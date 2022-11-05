#pragma once

#include <Arduino.h>

namespace CardReader
{
    struct UID
    {
        byte Bytes[10];
        byte Size;

        UID(byte size, const byte* data);

        static UID invalid();

        static bool isValid(const UID& uid);

        bool operator== (const UID& other) const;

        String HexString();
    };
}