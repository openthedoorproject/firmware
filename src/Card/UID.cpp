#include "UID.h"

#include "System.h"

CardReader::UID::UID(byte Size, const byte *data)
{
    if (Size > 10)
    {
        LOG_ERROR("Unsupported UID size");
        Size = 10;
    }
    else if (Size < 0)
    {
        UID::Size = -1;
        return;
    }

    UID::Size = Size;

    for (int i = 0; i < Size; i++)
    {
        Bytes[i] = data[i];
    }
}

CardReader::UID CardReader::UID::invalid()
{
    return CardReader::UID(-1, nullptr);
}

bool CardReader::UID::isValid(const CardReader::UID &uid)
{
    return uid.Size > -1;
}

bool CardReader::UID::operator== (const CardReader::UID &other) const
{
    if (other.Size != Size)
        return false;

    for (int i = 0; i < Size; i++)
    {
        if (other.Bytes[i] != Bytes[i])
            return false;
    }

    return true;
}

String CardReader::UID::HexString() {
    String hex = "";

    for(int i = 0; i < Size; i++) {
        hex.concat(String(Bytes[i], HEX));
    } 

    return hex;
}