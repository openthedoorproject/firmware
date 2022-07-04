#pragma once

#include "Password.h"
#include "<httpClient.h>"

class PasswordValidator {

public:

    template<uint32_t T>
    bool isValid(Password<T>& password);

    if(Wifi.status == WL_CONNECTED){

        HTTPClient http;

        http.begin(URL);
        http.addHeader("content-type", "application/json"); 
        
        int httpRespostaDoServidor = http.POST("posting from esp32");

        if(httpRespostaDoServidor>0){ 
            String response = http.getString();

            Serial.println(httpRespostaDoServidor);
            Serial.println(response);
        }else{
            Serial.println("Erro ao enviar dados para o form php");
            Serial.println(httpRespostaDoServidor);
        }

        http.end();
    }else{
        Serial.println("Falha ao tentar conectar o Wifi");
    }

    delay(10000);

};