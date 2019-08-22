#include "RF24.h"

RF24 radio(7, 8);
const byte data_pipe[6] = "00001";

void setup() {
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(data_pipe);
}

void loop() {
  char data[] = "Hello world!";
  radio.write(data, strlen(data));
  delay(1000);
}