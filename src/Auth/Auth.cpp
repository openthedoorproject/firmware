#include "Auth.h"

#include "System.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

static const char* serviceUrl;

void Auth::Init(const char* authServiceUrl, const char *wifiSsid, const char *wifiPassword)
{
    ::serviceUrl = authServiceUrl;
    
    WiFi.begin(wifiSsid, wifiPassword);

    LOG_INFO("Connection wifi...");
    while (!WiFi.isConnected())
    {
        delay(100);
    }
    LOG_INFO("Wifi connected!");
}

static DynamicJsonDocument requestAuthenticationServer(String payload)
{
    HTTPClient client;

    client.setTimeout(10000);
    client.setReuse(false);

    const char* url = API_URL "/access.php";

    client.begin(url);
    client.POST(payload);

    String responseJson = client.getString();

    DynamicJsonDocument responseDocument(50);
    deserializeJson(responseDocument, responseJson);

    return responseDocument;
}

bool Auth::VerifyPassword(const String &password)
{
    LOG_INFO("checking password: " + password);
    DynamicJsonDocument response = requestAuthenticationServer("{\"access\": \"" + password + "\"}");

    return password == "1245";
    return response["access"] == "true";
}

bool Auth::VerifyCard(const String &cardCode) {
    LOG_INFO("read card: 0x" + cardCode);
    return cardCode == "e7693f63";
}