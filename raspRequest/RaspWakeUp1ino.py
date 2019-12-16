import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM)
pin = 19
message = list("1")

while len(message) <32:
   message.append(0)

pipes = ["1Node", "2Node", "3Node"]
pipe1 = "1Node"
hexaCode = []

for letter in pipe1:
     hexaCode.append(ord(letter))

test = list(reversed(hexaCode))
pipes2 = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
channels = [0x76,0x78]
radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, pin)
radio.setPayloadSize(32)
radio.setChannel(channels[0])
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)
radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setCRCLength(2)
radio.openWritingPipe(pipes2[0])
radio.openReadingPipe(1,pipes2[1])
radio.printDetails()
radio.powerUp()
channel = 0

message = list("GETSTRING")

while len(message) < 32:
 message.append(0)
try:
    while True:
        #Acorda Arduino
        start = time.time()
        radio.write(message)
        print("Sent the message: {}".format(message))
        radio.startListening()
        
        while not radio.available(0):
            time.sleep(1/100)

        while radio.available(0):
        #Recebe Msg de Arduino
            receivedMessage = []
            radio.read(receivedMessage, radio.getDynamicPayloadSize())
            print("Received: {}".format(receivedMessage))

            print("Translating our received Message into unicode characters...")
            string = ""

            for n in receivedMessage:
               if (n >= 32 and n <= 126):
                   string += chr(n)
            print("Our received message decodes to: {}".format(string))
            radio.stopListening()
            time.sleep(1)
except KeyboardInterrupt:
    print("\n")
except:
    print("erros??")
finally:
    GPIO.cleanup()
