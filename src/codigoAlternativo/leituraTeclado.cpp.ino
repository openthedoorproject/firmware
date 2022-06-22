
#include <Keypad.h>

const byte linhas = 4;
const byte colunas = 4;
const int ledVermelho = 12;
const int ledVerde = 2;
const int ledAmarelo = 3;

byte pinosLinhas[linhas] = {11, 10, 9, 8};
byte pinosColunas[colunas] = {7, 6, 5, 4};
String senhaDigitada;
const String senhaV = "123#";
char teclas[linhas][colunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, linhas, colunas);

void setup() {
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  Serial.begin(9600);
  Serial.println("Digite sua senha " + senhaV);

}

void loop() {
  char teclaApertada = teclado.getKey();
  
  if (teclaApertada) {
   
  digitalWrite(ledAmarelo, HIGH);
    //Serial.print("Tecla digitada: ");
    //Serial.println(teclaApertada);
     senhaDigitada+=teclaApertada;
     if(teclaApertada=='#'){
      if(validarSenha(senhaDigitada, senhaV)){
         digitalWrite(ledAmarelo, LOW);
         digitalWrite(ledVerde, HIGH); 
        Serial.println("Acesso concedido");
        delay(2000);
      }else{
         digitalWrite(ledAmarelo, LOW);
         digitalWrite(ledVermelho, HIGH);
         Serial.println("Acesso negado" + senhaDigitada);
         delay(2000);
         
      }
      //digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, LOW);
      senhaDigitada="";
    //Serial.print("Senha digitada: ");
    //Serial.println(senhaDigitada);
     }
  }
 
 }

 bool validarSenha(String senha, String senhaV){
  if(senha == senhaV){
    return true;
  }else{
    return false;
  }
  //digitalWrite(ledAmarelo, LOW);
  //digitalWrite(ledVermelho, LOW);
  //digitalWrite(ledVerde, LOW);
 }
 
