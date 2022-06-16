#pragma once

#include<stdint.h>

namespace System {

    const uint32_t VERSION =1;// SYSTEM_VERSION;
    
    void wait(uint32_t millis);

    bool updateCheck();

    void update();
};