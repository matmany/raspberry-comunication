/*
   ------Projeto Wearables------
   
   NERA e Nu[Tec]² - IFES Serra
   Comunicação Arduino <--> Arduino
   Módulos Transmissores
    
   Autor: Magnu Windell Araújo Santos, Julho 2019
  testado...git
*/
// ===============================================================================
// --- Bibliotecas ---
#include <SPI.h>  //Serial Peripheral Interface (SPI)
#include "RF24.h" //Biblioteca para NRF24L01 
#include "printf.h"
// ===============================================================================
// --- ID ---
#define ID "BB02"

// ===============================================================================
// --- Variáveis Globais ---
int radioNumber = 2
; //Número do Rádio Transmissor 1 ou 2
RF24 radio(7,8); //Cria o objeto radio com portas CE e CS
byte addresses[][6] = {"1Node","2Node","3Node"}; //Endereço dos pipes

int s1 = 0;
int s2 = 0;
String s1s;
String s2s;
String trama;
char tramac[13];

// ===============================================================================
// --- Configurações Iniciais ---
void setup() {
  Serial.begin(9600); //Inicia comunicação com Monitor Serial
  printf_begin();
  radio.begin(); //Inicia comunicação do objeto radio
  radio.setPALevel(RF24_PA_MIN); //Configura potência do objeto radio
  radio.setDataRate(RF24_1MBPS );
  radio.setChannel(0X78);
  radio.enableDynamicPayloads();
  radio.powerUp();
  switch(radioNumber){ //Configura os pipes de acordo com o número do radio
    case 1:
      radio.openWritingPipe(addresses[0]); //Radio1: Fala no pipe 1
      radio.openReadingPipe(1,addresses[1]); //Radio1: Escuta no pipe 0
      break;
    
    case 2:
      //radio.openWritingPipe(0xF0F0F0F0E1LL);
      //0x65646f4e31
      //radio.openWritingPipe(addresses[0]);
      radio.openWritingPipe(0x65646f4E31LL); //Radio0: Fala no pipe 2
      //radio.openReadingPipe(1,addresses[2]); //Radio0: Escuta no pipe 0
      radio.openReadingPipe(1,0xE8E8F0F0E1LL);
      break;
  }
  Serial.println(addresses[1][0]);
  Serial.println(addresses[0][1]);
  Serial.println(addresses[0][2]);
  Serial.println(addresses[0][3]);
  Serial.println(addresses[0][4]);
  Serial.println(addresses[0][5]);
  Serial.println(addresses[0][6]);
  Serial.println(addresses[0][7]);
  Serial.println(addresses[0][8]);
  Serial.println(addresses[0][9]);
  radio.printDetails();
}


// ===============================================================================
// --- Loop ---
void loop() {

  s1 = random(0, 1500);
  s2 = random(0, 1500);

    s1s = getPadded(s1);
    s2s = getPadded(s2);
    trama = String(ID + s1s + s2s);
    trama.toCharArray(tramac, 13);
  
    radio.stopListening(); // Para de escutar

    Serial.print(F("Enviando: "));      //Imprime "Enviando" na Serial
    Serial.print(tramac);              //Imprime Trama enviada
    if (!radio.write( tramac, 13)){    //Envia a Trama
      Serial.println(": Falha");          //Imprime "Falha" caso não seja enviada
     }
     else {
      Serial.println("Sucesso");
     }
          
      radio.startListening(); //Volta a escutar
      
      unsigned long waiting_time = micros(); //Armazena o tempo atual para TIMEOUT
      boolean timeout = false; //Variável para indicação de TIMEOUT
      unsigned long times = 0;
      
      while ( !radio.available() ){ //Se nada for recebido
        times = micros() - waiting_time;
        if ((times) >= 200000 ){ //Tempo de espera > 200ms
            timeout = true; //TIMEOUT atingido
            break;  //Para o while
        }     
      }
          
      if ( timeout ){ //Se TIMEOUT for verdadeiro
          Serial.println("\b \b Tempo Esgotado"); //Imprime "Tempo Esgotado"
      }
      else{ //Se TIMEOUT for falso
          int recebido_t = 0; //Variável de resposta
          radio.read( &recebido_t, sizeof(int)); //Escuta a resposta
          if(recebido_t){ //Se resposta for verdadeira
            Serial.println(": Enviado"); //Imprime "Enviado"
          }
      }
    delay(1000); //Aguarda 1 segundo
}

String getPadded(int num) {
  char buff[5];
  char padded[6];
  
  sprintf(buff, "%.4u", num); 

  padded[0] = buff[0];
  padded[1] = buff[1];
  padded[2] = buff[2];
  padded[3] = buff[3];
  padded[4] = buff[4];
  padded[5] = '\0'; // The terminating NULL

  return String(padded);
}
