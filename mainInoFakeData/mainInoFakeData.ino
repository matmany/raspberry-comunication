#include <SPI.h>  //Serial Peripheral Interface (SPI)
#include "RF24.h" //Biblioteca para NRF24L01 
#include "printf.h"

#define ID "BB02"

const byte interruptPin = 2;
RF24 radio(9,10); //Cria o objeto radio com portas CE e CS
//RF24 radio(8,7);
int radioNumber = 2; //Número do Rádio Transmissor 1 ou 2
String s1s;
String s2s;
String trama;
char receivedMessage[32] = {0};
bool val;

void setup (){
    Serial.begin(9600); //Inicia comunicação com Monitor Serial
    //while(!Serial);
    printf_begin();
    delay(1000);
    radio.begin(); //Inicia comunicação do objeto radio
    radio.setPALevel(RF24_PA_MIN); //Configura potência do objeto radio
    radio.setDataRate(RF24_1MBPS );
    radio.setChannel(0X76); //canal
    radio.enableDynamicPayloads();
    radio.powerUp();
    radio.openWritingPipe(0xF0F0F0F0E1LL); 
    
    radio.openReadingPipe(1,0xE8E8F0F0E1LL);
    //radio.openWritingPipe(0xE8E8F0F0E1LL); 
    //radio.openReadingPipe(1,0xF0F0F0F0E1LL);
    radio.printDetails();
    radio.startListening();
    //radio.maskIRQ(1,1,0);
    //pinMode(interruptPin, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(interruptPin), radioRecevedMessage, FALLING);
}

void loop (){
radioRecevedMessage();
delay(500); //Aguarda 1 segundo
}

void radioRecevedMessage() {
  Serial.println("Entrando");
  char tramac[13];
  if(radio.available()){
    radio.read(receivedMessage, sizeof(receivedMessage));
    //Serial.println("Receving....");
    val = digitalRead(interruptPin);
    //Serial.println(val);
  }
  generateFakeData().toCharArray(tramac, 13);
  radio.stopListening();
  Serial.print(F("Enviando: "));
  Serial.print(tramac);
  
  if (!radio.write(tramac, 13)){                              //Envia a Trama
       Serial.println(": Falha"); //Imprime "Falha" caso não seja enviada
   }
  else{
      Serial.println("Sucesso");
    }
  radio.startListening();
  
  
}

String generateFakeData() {
    int s1 = 0;
    int s2 = 0;
    String s1s;
    String s2s;
    String trama;
    char tramac[13];

    s1 = random(0, 1500);
    s2 = random(0, 1500);
    s1s = getPadded(s1);
    s2s = getPadded(s2);
    trama = String(ID + s1s + s2s);
    return trama;
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
