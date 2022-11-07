#include "Auth.h"

#include "Logger.h"

#include <WiFi.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>

static String serviceUrl;
static int laboratory;

static Auth::Response requestAccess(const String &payload)
{
    HTTPClient httpClient;

    httpClient.begin(serviceUrl);

    int responseCode = httpClient.POST(payload);

    if (responseCode != 200)
        return Auth::ERROR;

    StaticJsonDocument<40> response;

    String rawResponse = httpClient.getString();
    deserializeJson(response, rawResponse);

    String access = response["access"].as<String>();

    return access == "true"? Auth::ACCEPTED : Auth::DENIED;
}

void Auth::Init(const String &authenticationServiceUrl, const String &ssid, const String &password)
{
    WiFi.begin(ssid.c_str(), password.c_str());

    LOG_INFO("Connecting WiFi...");
    while (!WiFi.isConnected())
        ;
    LOG_INFO("Conneted!");

    serviceUrl = authenticationServiceUrl;
    ::laboratory = laboratory;
}

Auth::Response Auth::VerifyPassword(int laboratory, const String &password)
{
    return requestAccess("{\"lab\":" + String(laboratory) + ",\"password\":\"" + password + "\"}");
}

Auth::Response Auth::VerifyCard(int laboratory, const String &cardData)
{
    return requestAccess("{\"lab\":" + String(laboratory) + ",\"card\":\"" + cardData + "\"}");
}