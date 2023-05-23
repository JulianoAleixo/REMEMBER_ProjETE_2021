/*=========================================================================================================================
 =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= PROJETE 2021 - EQUIPE 2301 - REMEMBER :) =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 =-=-=-=-=-=-=-=-=-= ALUNOS: JULIANO, LINDSEY, MARINA, MIGUEL      ORIENTADORA: ANA LETÌCIA =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 ========================================================================================================================== 
*/

//////////////////////////////////////BIBLIOTECAS///////////////////////////////////////////////////////////////////////////

#include <Wire.h>                     // Inclui biblioteca Wire (I2C)
#include <LiquidCrystal_I2C.h>        // Inclui a biblioteca do LCD
#include "RTClib.h"                   // Inclui a biblioteca do RTC
#include <ESP8266Firebase.h>          // Inclui biblioteca do Firebase
#include <ESP8266WiFi.h>              // Inclui biblioteca do módulo WiFi

//////////////////////////////////////DEFINIÇÕES////////////////////////////////////////////////////////////////////////////

#define LDR A0                        // Define o pino do LDR
#define LED D3                        // Define o pino do LED
#define buzz D0                       // Define o pino do Buzzer 

#define TESTE

#define A D5                          // Pinos do Motor de Passo
#define B D6                          // Pinos do Motor de Passo
#define C D7                          // Pinos do Motor de Passo
#define D D8                          // Pinos do Motor de Passo 
#define quantidadeDePassos 35         // Quantidade de Passos que o Motor precisa dar para girar um compartimento do disco
#define _SSID "**********"            // Nome da rede
#define _PASSWORD "*********"         // Senha da Rede
#define PROJECT_ID "remember-a74a5-default-rtdb"  // ID do projeto no firebase
#define FINGERPRINT "6E F3 55 47 6D F1 0E 5B 2B 04 F2 F8 C0 8B E8 05 65 0F 1A C1"

///////////////////////////////DEFINIÇÕES DAS BIBLIOTECAS///////////////////////////////////////////////////////////////////
   
LiquidCrystal_I2C lcd(0x27,16,2);     // Define o LCD como um 16x2 no endereço 0x27
RTC_DS1307 rtc;                       // Define o RTC na biblioteca
Firebase firebase(PROJECT_ID, FINGERPRINT); // Define o Firebase
DateTime now;                         // Define a hora atual

/////////////////////////////////VARIAVEIS GLOBAIS//////////////////////////////////////////////////////////////////////////

int contador = 1;                     // Variaveis para a contagem de remédios
int modo = 0;                         // Variaveis para a contagem de remédios
int espera = 0;                       // Variaveis para a contagem de remédios
int inicio = 1;                       // Variaveis para a contagem de remédios

int hora1 = 0;                        // Variavel para hora
int minuto1 = 0;                      // Variavel para minuto
int dia1 = 0;                         // Variavel para dia
int mes1 = 0;                         // Variavel para mes
String nome1;                         // Variavel para nome do remédio

int hora2 = 0;                        // Variavel para hora
int minuto2 = 0;                      // Variavel para minuto
int dia2 = 0;                         // Variavel para dia
int mes2 = 0;                         // Variavel para mes
String nome2;                         // Variavel para nome do remédio

int taEscuro = 0;                     // Variável para valor digital do LDR
int girou = 0;                        // Variável que verifica se o motor girou
int intervalo = 0;                    // Variável de intervalo
int minuto_anterior;                  // Variável para o minuto anterior (para o motor não girar em loop)

String comp1_nome;                    // Informações vindas do firebase
String comp1_hora;                    // Informações vindas do firebase
String comp1_min;                     // Informações vindas do firebase
String comp1_dia;                     // Informações vindas do firebase
String comp1_mes;                     // Informações vindas do firebase
 
String comp2_nome;                    // Informações vindas do firebase
String comp2_hora;                    // Informações vindas do firebase
String comp2_min;                     // Informações vindas do firebase
String comp2_dia;                     // Informações vindas do firebase
String comp2_mes;                     // Informações vindas do firebase


////////////////////////////////INFORMAÇÕES DO FIREBASE////////////////////////////////////////////////////////////////////

void le_firebase() 
{
//========================= COMP 1 ========================================================================================
     
   comp1_nome = firebase.getString("Informacoes_Comp1/0"); // Nome Comp. 1
  
   comp1_hora = firebase.getString("Informacoes_Comp1/1"); // Hora Comp. 1
  
   comp1_min = firebase.getString("Informacoes_Comp1/2");  // Minuto Comp. 1
  
   comp1_dia = firebase.getString("Informacoes_Comp1/3");  // Dia Comp. 1
 
   comp1_mes = firebase.getString("Informacoes_Comp1/4");  // Mês Comp. 1

   mostra_relogio();

//========================= COMP 2 =======================================================================================
  
   comp2_nome = firebase.getString("Informacoes_Comp2/0"); // Nome Comp. 2

   comp2_hora = firebase.getString("Informacoes_Comp2/1"); // Hora Comp. 2

   comp2_min = firebase.getString("Informacoes_Comp2/2");  // Minuto Comp. 2
  
   comp2_dia = firebase.getString("Informacoes_Comp2/3");  // Dia Comp. 2

   comp2_mes = firebase.getString("Informacoes_Comp2/4");  // Mês Comp. 2

   mostra_relogio();
}

  
//////////////////////////////////////////SETUP////////////////////////////////////////////////////////////////////////////

void setup() {
  lcd.init();                         // Começa o LCD
  lcd.backlight();                    // Liga a luz externa do LCD
  lcd.clear();                        // Limpa o LCD
  lcd.print("Remember");              // Escreve a mensagem no LCD 
  delay(1000);                        // Espera 1 segundo 
//#ifndef TESTE 
  lcd.print(".");                     // Escreve a mensagem no LCD 
  delay(1000);                        // Espera 1 segundo 
  lcd.print(".");                     // Escreve a mensagem no LCD 
  delay(1000);                        // Espera 1 segundo 
  lcd.print(".");                     // Escreve a mensagem no LCD 
  delay(2000);                        // Espera 2 segundo 
//#endif
  lcd.clear();                        // Limpa o LCD
  
  Serial.begin(9600);                 // Inicia o Serial Monitor
  
  pinMode(LDR, INPUT);                // Define o LDR como entrada
  pinMode(LED, OUTPUT);               // Define o LED como saída
  pinMode(buzz, OUTPUT);              // Define o Buzzer como saída 

  if (! rtc.begin()) {                // Se o rtc não for iniciado, faz
    while(1);                         // Sempre entre no loop
  }

  pinMode(A,OUTPUT);                  // Define os pinos do motor como saída
  pinMode(B,OUTPUT);                  // Define os pinos do motor como saída
  pinMode(C,OUTPUT);                  // Define os pinos do motor como saída
  pinMode(D,OUTPUT);                  // Define os pinos do motor como saída
   
  WiFi.mode(WIFI_STA);                // Habilita o modo estação
  WiFi.disconnect();                  // Disconecta a placa de qualquer rede WiFi
  delay(1000);                        // Espera 1 segundo
  Serial.println();                   // Pula uma linha no monitor serial
  Serial.println();                   // Pula uma linha no monitor serial
  Serial.print("Connecting to: ");    // Escreve uma mensagem no monitor serial
  Serial.println(_SSID);              // Escreve o nome da rede no monitor serial
  WiFi.begin(_SSID, _PASSWORD);       // Conecta no WiFi com o nome da rede e a senha
  
  while (WiFi.status() != WL_CONNECTED) // Enquanto o status do wifi for desconectado
  { 
    delay(500);                       // Espera 500 milisegundos
    Serial.print("-");                // Escreve um "-" no monitor serial
  }
  
  Serial.println("");                 // Pula uma linha no monitor serial
  Serial.println("WiFi Conectado");   // Escreve que o wifi está conectado no monitor serial
  
  Serial.print("Use this URL to connect: "); // Escreve uma mensagem no monitor serial
  Serial.print("http://");            // Escreve o início de uma URL
  Serial.print(WiFi.localIP());       // Escreve o IP da URL
  Serial.println("/");                // Escreve uma / para finalizar o link

  modo = inicio;                      // Variável modo recebe o valor de início
  le_firebase();                      // Executa a função que lê o firebase

}                                     // Fecha o setup()


//////////////////////////////////////////LOOP//////////////////////////////////////////////////////////////////////////////

void loop() { 

///////////////////////////////////////LEITURA DO FIREBASE NO LOOP////////////////////////////////////////////////////////// 
 
  intervalo++;                        // Intervalo recebe seu valor mais 1
  if(intervalo == 500)                // Se o valor do intervalo for igual a 500
  {
   le_firebase();                     // Executa a função que lê o firebase
   intervalo = 0;                     // Zera a variavel intervalo
  }

//////////////////////////////CONTADOR DE REMÉDIOS E VERIFICA O ESTADO DO LDR PARA O PISCA/////////////////////////////////

    if(analogRead(LDR) >= 700)        // Se o valor do LDR for maior que 700
    {
      taEscuro = 1;                   // Variavel recebe 1
    }
    else                              // Senão
    {
      taEscuro = 0;                   // Variável recebe 0
    }

    if(modo == espera)                // Se a variável modo for igual a espera (estado 0)     
    {   
      pisca();                        // Executa a função pisca
      if(taEscuro == 0)               // Se o valor digital do LDR for 0
      {
        contador++;                   // Soma 1 ao contador
        Serial.println(contador);     // Mostra o valor do contador no monitor serial
        firebase.setInt("Remember/LDR",1); // Manda o valor 1 para o firebase
        modo = inicio;                // Modo recebe o valor de espera (estado 0)
      }
    }

    nome1 = comp1_nome;               // As variaveis do compartimento 1 recebem os valores do firebase
    hora1 = comp1_hora.toInt();       // As variaveis do compartimento 1 recebem os valores do firebase
    minuto1 = comp1_min.toInt();      // As variaveis do compartimento 1 recebem os valores do firebase
    
    nome2 = comp2_nome;               // As variaveis do compartimento 2 recebem os valores do firebase
    hora2 = comp2_hora.toInt();       // As variaveis do compartimento 2 recebem os valores do firebase
    minuto2 = comp2_min.toInt();      // As variaveis do compartimento 2 recebem os valores do firebase
    
    Serial.print(hora1);              // Exibe a hora 1 no monitor serial
    Serial.print(":");                // Separa a hora do minuto com um ":"
    Serial.println(minuto1);          // Exibe o minuto 1 no monitor serial
    Serial.println("");               // Pula uma linha no monitor serial
    Serial.print(hora2);              // Exibe a hora 1 no monitor serial
    Serial.print(":");                // Separa a hora do minuto com um ":"
    Serial.println(minuto2);          // Exibe o minuto 1 no monitor serial
    Serial.println(contador);         // Exibe o valor do contador no monitor serial
   
///////////////////////////////////EXIBE A HORA NO DISPLAY LCD/////////////////////////////////////////////////////////////

     mostra_relogio();                // Função do relógio

/////////////////////////////////////COMPARA OS HORARIOS DOS COMPARTIMENTOS////////////////////////////////////////////////

//========================= COMP 1 ========================================================================================
    if((hora1 == now.hour()) && (minuto1 == now.minute()) && (girou==0)){
      lcd.clear();                    // Limpa o LCD
      int i;                          // Cria a variavel para o for
      Serial.println("horario");      // Mostra que está no horário no monitor serial
      for(i=0; i < quantidadeDePassos; i++) // Entra no loop para a quantidade de passos pré definidos
      { 
        onestep();                    // Executa a função de girar um passo       
      } 
      modo = espera;                  // Variável modo recebe o valor de espera
      girou = 1;                      // Variável girou recebe o valor 1
      minuto_anterior = minuto1;      // Minuto anterior guarda o valor do minuto 1
      lcd.setCursor(0,0);             // Muda a linha do LCD
      lcd.print(nome1);               // Escreve o nome 1 no display
    } 

//========================= COMP 2 =======================================================================================
    if((hora2 == now.hour()) && (minuto2 == now.minute()) && (girou==0)){
      lcd.clear();                    // Limpa o LCD
      int i;                          // Cria a variavel para o for
      Serial.println("horario");      // Mostra que está no horário no monitor serial
      for(i=0; i < quantidadeDePassos; i++) // Entra no loop para a quantidade de passos pré definidos
      {
        onestep();                    // Executa a função de girar um passo 
      }
      modo = espera;                  // Variável modo recebe o valor de espera
      girou = 1;                      // Variável girou recebe o valor 1
      minuto_anterior = minuto2;      // Minuto anterior guarda o valor do minuto 1
      lcd.setCursor(0,0);             // Muda a linha do LCD
      lcd.print(nome2);               // Escreve o nome 1 no display
    }

//========================================================================================================================
    if(minuto_anterior!= now.minute())// Se o minuto anterior for diferente do minuto atual
    {
      girou = 0;                      // Variável girou recebe o valor 0 
    }

  
}                                     // Fecha void loop

///////////////////////////////////////FUNÇÃO PISCA-PISCA//////////////////////////////////////////////////////////////////

void pisca() {
  digitalWrite(LED,1);                 // Liga o LED 
  tone(buzz,950);                      // Toca o Buzzer
  delay(500);                          // Espera 250 milésimos
  digitalWrite(LED,0);                 // Desliga o LED
  noTone(buzz);                        // Para o Buzzer
  delay(250);                          // Espera 250 milésimos
}

////////////////////////////////////////FUNÇÃO PARA O MOTOR///////////////////////////////////////////////////////////////

void write(int a,int b,int c,int d){
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}

/////////////////////////////////////FUNÇÃO DE UM PASSO DO MOTOR//////////////////////////////////////////////////////////

void onestep(){
  write(1,0,0,0);
  delay(5);
  write(1,1,0,0);
  delay(5);
  write(0,1,0,0);
  delay(5);
  write(0,1,1,0);
  delay(5);
  write(0,0,1,0);
  delay(5);
  write(0,0,1,1);
  delay(5);
  write(0,0,0,1);
  delay(5);
  write(1,0,0,1);
  delay(5);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mostra_relogio()
{
    now = rtc.now();                  // Chamada de função
    lcd.setCursor(0,1);               // Define a linha do LCD
    lcd.print("     ");               // Deixa o horário centralizado
    if(now.hour() < 10){              // Se o valor de HORA for menor que 10
      lcd.print("0");                 // Digita um "0" antes do valor 
    }
    
    lcd.print(now.hour(), DEC);       // Digita o valor da HORA
    lcd.print(':');                   // Digita um ":"
    
    if(now.minute() < 10){            // Se o valor de MINUTO for menor que 10
      lcd.print("0");                 // Digita um "0" antes do valor 
    }  
    
    lcd.print(now.minute(), DEC);     // Digita o valor do MINUTO 
}
