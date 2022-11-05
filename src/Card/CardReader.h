#pragma once

#include "UID.h"

namespace CardReader {

    enum ReadingState {
        READING,
        ENDED,
        ERROR
    };

    void Init(int rtsPin, int sdaPin);

    void SetTimeout(long millisseconds);

    bool HasCard();

    ReadingState ReadCard();

    UID GetCardUID();
}