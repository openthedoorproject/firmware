#pragma once

#include "CardData.h"

class CardReader {

public:

    bool hasCardOnRange();
    
    CardData readCard();

};