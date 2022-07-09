#pragma once

#include<stdint.h>

#ifdef __RELEASE
    #ifndef SYSTEM_VERSION
        #error a system version should be defined
    #endif

    #ifndef API_BASE_URL
        #error a API base URL must be defined
    #endif

    #ifndef LOCATION
        #error a location should be defined
    #endif

#else 
    #ifndef SYSTEM_VERSION
        #define SYSTEM_VERSION 1
    #endif

    #ifndef API_BASE_URL
        #define API_BASE_URL "elabkey.freetzi.com/api"
    #endif

    #ifndef LOCATION 
        #define LOCATION 500
    #endif
#endif

namespace System {

    void wait(uint32_t millis);

    bool updateCheck();

    void update();
};