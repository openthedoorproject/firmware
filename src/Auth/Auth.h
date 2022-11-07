#pragma once

#include <Arduino.h>

namespace Auth
{
    enum Response {
        ACCEPTED,
        DENIED,
        ERROR
    };

    void Init(const String &authenticationServiceUrl, const String &ssid, const String &password);

    Response VerifyPassword(int laboratory, const String &password);

    Response VerifyCard(int laboratory, const String &cardData);
}