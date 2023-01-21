#include "CardReader.h"

#include <Arduino.h>
#include <SPI.h>

#include <MFRC522.h>

static MFRC522* mfrc522;

static long lastReadTime = -1;
static long timeout;

void CardReader::Init(int rtsPin, int sdaPin)
{
    SPI.begin();

    mfrc522 = new MFRC522(sdaPin, rtsPin);
    mfrc522->PCD_Init();

    SetTimeout(2000);
}

void CardReader::SetTimeout(long millisseconds)
{
    ::timeout = millisseconds;
}

bool CardReader::HasCard()
{
    if (millis() - lastReadTime < timeout)
        return false;

    lastReadTime = millis();

    return mfrc522->PICC_IsNewCardPresent();
}

CardReader::ReadingState CardReader::ReadCard()
{
    if (mfrc522->PICC_ReadCardSerial())
        return CardReader::ENDED;
    else
        return CardReader::ERROR;
}

CardReader::UID CardReader::GetCardUID()
{
    MFRC522::Uid uid = mfrc522->uid;

    return UID(uid.size, uid.uidByte);
}