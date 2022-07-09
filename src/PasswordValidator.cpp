#include "PasswordValidator.h"
#include "./system/System.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void buildJson(uint16_t lab, String &password, String &output)
{
    const uint32_t capacity = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<capacity> document;

    document["password"] = password;
    document["lab"] = lab;

    serializeJson(document, output);
}

JsonObject parseJson(WiFiClient& dataStream) {
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    deserializeJson(doc, dataStream);

    return doc.to<JsonObject>();
}

template <uint32_t lenght>
bool PasswordValidator::isValid(Password<lenght> &password)
{

    HTTPClient client;
    client.useHTTP10(true);

    String requestData = buildJson(LOCATION, password.toString());

    if (WiFi.status() == WL_CONNECTED)
    {
        if (client.begin(String(API_BASE_URL) + "/access.php"))
        {
            client.addHeader("Content-Type", "application/json");
            client.POST(requestData);

            JsonObject obj = parseJson(client.getStream());

            bool hasPermission = obj["access"].as<bool>();

            client.end();

            return hasPermission;
        }
    }

    return false;
}