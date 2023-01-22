#include "Auth.h"

#include "Logger.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

static String serviceUrl;
static int labNumber;

void Auth::Init(const char* authServiceUrl, int labNumber, const char *wifiSsid, const char *wifiPassword)
{
    ::serviceUrl = String(authServiceUrl) + "/access.php";
    ::labNumber = labNumber;
    
    WiFi.begin(wifiSsid, wifiPassword);

    LOG_INFO("Connecting wifi...");
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

    const char* url = serviceUrl.c_str();

    LOG_INFO("sending: " + payload);
    LOG_INFO("to: " + url);

    client.begin(url);
    client.POST(payload);

    String responseJson = client.getString();

    LOG_INFO("response: " + responseJson);

    DynamicJsonDocument responseDocument(50);
    deserializeJson(responseDocument, responseJson);

    return responseDocument;
}

bool Auth::VerifyPassword(const String &password)
{
    LOG_INFO("checking password: " + password);
    
    DynamicJsonDocument response = requestAuthenticationServer(String("{\"lab\":\"") + labNumber + "\",\"password\": \"" + password + "\"}");
    return response["access"].as<String>() == "true";
}

bool Auth::VerifyCard(const String &cardCode) {
    LOG_INFO("read card: 0x" + cardCode);

    DynamicJsonDocument response = requestAuthenticationServer(String("{\"lab\":\"") + labNumber + "\",\"card\": \"" + cardCode + "\"}");
    return response["access"].as<String>() == "true";
}