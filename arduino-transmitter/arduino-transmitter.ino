#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// ce, csn pins
RF24 radio(7, 8);

void setup(void)
{
  Serial.begin(115200);
  printf_begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.enableDynamicPayloads();
  radio.powerUp();
  //radio.setCRCLength(0);
  radio.printDetails();
}

void loop(void)
{
  const char text[] = "Hello World!";
  radio.write(&text, sizeof(text));
  //delay(1000);
}
