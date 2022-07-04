#include <httpClient.h>
#include <Wifi.h>

const char* ssid = "nomeDoWifi";
const char* password = "senhaDoWifi";
const char URL = "http://elabkey.freetzi.com/api{arquivo.php}";

void setup(){

    Wifi.begin(ssid, password);
    Serial.begin(9600);

    while(Wifi.status() != WL_CONNECTED)){
        delay(1000);
        Serial.println("Conectando wifi");
    }

    Serial.println("Wifi conectado");
}

void loop(){
    if(Wifi.status == WL_CONNECTED){

        HTTPClient http;

        http.begin(URL);
        http.addHeader("content-type", "text/plain"); 
        /*não entendi muito bem a finalidade desse metodo acima, 
        ele especifica otipo que vai ser lido no servidor ? sla*/

        int httpRespostaDoServidor = http.POST("posting from esp32");

        if(httpRespostaDoServidor>0){ // se httpRespostaDoServidor>0 então não ocorreu nenhum erro no processo
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
}