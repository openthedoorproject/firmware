
#include <Keypad.h>

const byte linhas = 4;
const byte colunas = 4;
const int ledVermelho = 12;
const int ledVerde = 2;
const int ledAmarelo = 3;
const String senhaV = "123#";

byte pinosLinhas[linhas] = {11, 10, 9, 8};
byte pinosColunas[colunas] = {7, 6, 5, 4};
String senhaDigitada;
int situacao = 0;
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
    situacao = 1;
    mostrarSituacao(situacao);
    senhaDigitada+=teclaApertada;

     if(teclaApertada=='#'){
      if(validarSenha(senhaDigitada, senhaV)){
        situacao = 2;
        mostrarSituacao(situacao);
        Serial.println("Acesso concedido");
        delay(2000);
      }else{
         situacao = 3;
         mostrarSituacao(situacao);
         Serial.println("Acesso negado" + senhaDigitada);
         delay(2000);
         
      }
      
      senhaDigitada="";
      situacao = 0;
    
     }
  }
 
 }

 bool validarSenha(String senha, String senhaV){
  if(senha == senhaV)
    return true;
  else
    return false;
 }

 void  mostrarSituacao(int situacao){
    if(situacao == 0){
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, LOW);
    }else if(situacao == 1){
      digitalWrite(ledAmarelo, HIGH);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, LOW);
    }else if(situacao == 2){
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledVermelho, LOW);
    }else if(situacao == 3){
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledVermelho, HIGH);
    }

 }
 
