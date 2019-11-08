#include <SPI.h>  //Serial Peripheral Interface (SPI)
#include "RF24.h" //Biblioteca para NRF24L01 
#include "printf.h"
#include <avr/sleep.h>

#define ID "BB02"

int radioNumber = 2; //Número do Rádio Transmissor 1 ou 2
RF24 radio(7,8); //Cria o objeto radio com portas CE e CS
//int s1 = 0;
//int s2 = 0;
String s1s;
String s2s;
String trama;
char tramac[13];

void setup (){
    Serial.begin(9600); //Inicia comunicação com Monitor Serial 
    printf_begin();
    radio.begin(); //Inicia comunicação do objeto radio
    radio.maskIRQ(1,1,0);
    radio.setPALevel(RF24_PA_MIN); //Configura potência do objeto radio
    radio.setDataRate(RF24_1MBPS );
    radio.setChannel(0X78); //canal
    radio.enableDynamicPayloads();
    radio.powerUp();
    radio.openWritingPipe(0x65646f4E31LL); 
    radio.openReadingPipe(1,0xE8E8F0F0E1LL);
    radio.printDetails();
    
}

void loop() {
    Serial.println("Runing");

}

char *generateFakeData() {
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
    trama.toCharArray(tramac, 13);
    return tramac;
}

void wakedUp(){
    char *tramac;
    tramac =  generateFakeData();
    radio.stopListening(); // Para de escutar
    Serial.print(F("Enviando: "));
    Serial.print(tramac); 
    if (!radio.write( tramac, 13)){    //Envia a Trama
      Serial.println(": Falha");          //Imprime "Falha" caso não seja enviada
     }
     else {
      Serial.println("Sucesso");
     }
     radio.startListening();
     delay(1000);
}

void sleepNow(){
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    attachInterrupt(1, wakedUp, FALLING);
    sleep_cpu(); 
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
