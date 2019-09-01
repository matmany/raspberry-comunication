# 1 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino"
# 1 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino"
/*

   ------Projeto Wearables------

   

   NERA e Nu[Tec]² - IFES Serra

   Comunicação Arduino <--> Arduino

   Módulos Transmissores

    

   Autor: Magnu Windell Araújo Santos, Julho 2019

  testado...git

*/
# 11 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino"
// ===============================================================================
// --- Bibliotecas ---
# 14 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino" 2
# 15 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino" 2

// ===============================================================================
// --- ID ---


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
  radio.begin(); //Inicia comunicação do objeto radio
  radio.setPALevel(RF24_PA_HIGH); //Configura potência do objeto radio

  switch(radioNumber){ //Configura os pipes de acordo com o número do radio
    case 1:
    radio.openWritingPipe(addresses[0]); //Radio1: Fala no pipe 1
    radio.openReadingPipe(1,addresses[1]); //Radio1: Escuta no pipe 0
    break;

    case 2:
    radio.openWritingPipe(addresses[0]); //Radio0: Fala no pipe 2
    radio.openReadingPipe(1,addresses[2]); //Radio0: Escuta no pipe 0
    break;
  }
}


// ===============================================================================
// --- Loop ---
void loop() {

  s1 = random(0, 1500);
  s2 = random(0, 1500);

    s1s = getPadded(s1);
    s2s = getPadded(s2);
    trama = String("AA02" + s1s + s2s);
    trama.toCharArray(tramac, 13);

    radio.stopListening(); // Para de escutar

    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 69 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 69 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino"
                "Enviando: "
# 69 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino" 3
                ); &__c[0];}))
# 69 "c:\\Users\\matmany\\Desktop\\Iron_lady\\Projetos\\Andamento\\Fapes\\main\\raspberry-comunication\\arduino-transmitter\\arduino-transmitter.ino"
                ))); //Imprime "Enviando" na Serial
    Serial.print(tramac); //Imprime Trama enviada
    if (!radio.write( tramac, 13)){ //Envia a Trama
      Serial.println(": Falha"); //Imprime "Falha" caso não seja enviada
     }

      radio.startListening(); //Volta a escutar

      unsigned long waiting_time = micros(); //Armazena o tempo atual para TIMEOUT
      boolean timeout = false; //Variável para indicação de TIMEOUT
      unsigned long times = 0;

      while ( !radio.available() ){ //Se nada for recebido
        times = micros() - waiting_time;
        if ((times) >= 200000 ){ //Tempo de espera > 200ms
            timeout = true; //TIMEOUT atingido
            break; //Para o while
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
