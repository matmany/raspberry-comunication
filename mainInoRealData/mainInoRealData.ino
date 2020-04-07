#include <SPI.h>  //Serial Peripheral Interface (SPI)
#include "RF24.h" //Biblioteca para NRF24L01 
#include "printf.h"

#define ID "BB02"
#define APC 50   //Precisão de calibração - Quantidade de amostras
#define TPC 50   //Precisão de calibração - Tempo entre amostras
#define LPC 50   //Quantidade de leituras por ciclo
#define TDL 10   //Tempo entre leituras
#define MQ4RL 20  //Resistência de carga MQ-4, 20Kohm <- Datasheet
#define MQ4A 4   //Pino analogico MQ-4 A
#define MQ4B 5   //Pino analogico MQ-4 B
#define MQ4AS "MQ-4 A"   //Pino analogico MQ-4 A
#define MQ4BS "MQ-4 B"   //Pino analogico MQ-4 B

const byte interruptPin = 2;
RF24 radio(9,10); //Cria o objeto radio com portas CE e CS

int radioNumber = 2; //Número do Rádio Transmissor 1 ou 2
String s1s;
String s2s;
String trama;
char receivedMessage[32] = {0};
bool val;

// --- Variáveis Globais --- 
//Sensor A
float MQ4AR0 = 0;          //Resistência em Ar Limpo
float MQ4ARSpR0 = 0;       //Razão entre Resistência Lida e Resistência em Ar Limpo
float MQ4AResult = 0;      //Resultado final em ppm

//Sensor B
float MQ4BR0 = 0;          //Resistência em Ar Limpo
float MQ4BRSpR0 = 0;       //Razão entre Resistência Lida e Resistência em Ar Limpo
float MQ4BResult = 0;      //Resultado final em ppm

// --- Funções e Procedimentos
float calibrate(int pin, String s);
float leituraA();
float leituraB();

void setup (){
    Serial.begin(9600); //Inicia comunicação com Monitor Serial
    while(!Serial);
    MQ4AR0 = calibrate(MQ4A, MQ4AS);
    MQ4BR0 = calibrate(MQ4B, MQ4BS); 
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
  //Serial.println("Entrando");
  char tramac[13];
  if(radio.available()){
    radio.read(receivedMessage, sizeof(receivedMessage));
    //Serial.println("Receving....");
    val = digitalRead(interruptPin);
    //Serial.println(val);
  }
  generateData().toCharArray(tramac, 13);
  radio.stopListening();
  Serial.print(F("Enviando: "));
  Serial.println(tramac);
  
  if (!radio.write(tramac, 13)){                              //Envia a Trama
      // Serial.println(": Falha"); //Imprime "Falha" caso não seja enviada
   }
  else{
     // Serial.println("Sucesso");
    }
  radio.startListening();
  
}

String generateData() {
    int s1 = 0;
    int s2 = 0;
    String s1s;
    String s2s;
    String trama;
    char tramac[13];

    s1 = leituraA();
    s2 = leituraB();
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

float calibrate(int pin, String s){
  // Calibração de Sensores:
  float ValSensor = 0;   //Variável para calibração
  float VoltSensor = 0;  //Variável para calibração
  float AirSensor = 0;   //Variável para calibração
  float result = 0;
  
  Serial.print("-- Calibrando ");
  Serial.print(s);
  Serial.println(" --");
  Serial.print("-- ");
  Serial.print(APC);
  Serial.print(" leituras de ");
  Serial.print(TPC);
  Serial.println(" ms --");

  for(int i=0; i<=APC;i++){
     float percent = (float(i)/APC)*100;
     Serial.print("-- Calibrando ");
     Serial.print(s);
     Serial.print(" : ");
     Serial.print(percent);
     Serial.println("% --");
     ValSensor = ValSensor + analogRead(pin);
     delay(TPC);
  }

  ValSensor = ValSensor/APC;
  VoltSensor = ValSensor/1024*5.0; 
  AirSensor = (5.0-VoltSensor)/VoltSensor;
  result = AirSensor/10.0;
  Serial.print("-- ");
  Serial.print(s);
  Serial.print(" Calibrado, R0 = ");
  Serial.println(MQ4AR0);
  return result;
}

float leituraA(){
  float MQ4AVal = 0;
  float MQ4ARS = 0;
  float result = 0;

  for(int j=0; j<=LPC; j++){
      MQ4AVal = MQ4AVal + analogRead(MQ4A);
      delay(TDL);
  }
  
  MQ4AVal = MQ4AVal/LPC;
  
// Tratamento dos Sinais  
  MQ4ARS = MQ4RL*(1023-MQ4AVal)/(MQ4AVal);            //Conversão
  MQ4ARSpR0 = MQ4ARS/MQ4AR0;                          //Cálculo da razão 
  result = pow((11.436/MQ4ARSpR0),2.83286119);   //Conversão em PPM
  Serial.print("PPM A: ");
  Serial.println(result);
  return(result);
}

float leituraB(){
  float MQ4BVal = 0;
  float MQ4BRS = 0;
  float result = 0;

  for(int j=0; j<=LPC; j++){
      MQ4BVal = MQ4BVal + analogRead(MQ4B);
      delay(TDL);
  }
  
  MQ4BVal = MQ4BVal/LPC;
  
// Tratamento dos Sinais  
  MQ4BRS = MQ4RL*(1023-MQ4BVal)/(MQ4BVal);            //Conversão
  MQ4BRSpR0 = MQ4BRS/MQ4BR0;                          //Cálculo da razão 
  result = pow((11.436/MQ4BRSpR0),2.83286119);   //Conversão em PPM
  Serial.print("PPM B: ");
  Serial.println(result);
  return(result);
}
