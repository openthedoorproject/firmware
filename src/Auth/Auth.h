#pragma once

#include "Password.h"

namespace Auth {

    void Init(const char* authServiceUrl, int labNumber, const char* wifiSsid, const char* wifiPassword);

    bool VerifyPassword(const String& password);

    bool VerifyCard(const String& cardCode);
}