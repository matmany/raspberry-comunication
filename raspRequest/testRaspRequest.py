import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

GPIO.setmode(GPIO.BCM)
pin = 19
message = list("1")
while len(message) <32:
   message.append(0)
#GPIO.setup(pin, GPIO.OUT)
#GPIO.output(pin, False)
#pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
pipes = ["1Node", "2Node", "3Node"]
pipe1 = "1Node"
hexaCode = []
for letter in pipe1:
     hexaCode.append(ord(letter))
test = list(reversed(hexaCode))
#hexaCode.append(0)
pipes2 = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
channels = [0x76,0x78]
radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, pin)
#0, 17
radio.setPayloadSize(32)
radio.setChannel(channels[0])
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setCRCLength(2)

radio.openWritingPipe(pipes2[0])
#radio.openReadingPipe(1, pipes2[1])
radio.openReadingPipe(1,test) #test
radio.printDetails()
radio.startListening()
# radio.print_status()
radio.powerUp()
#print("1Node".enconde("hex"))
channel = 0
try:
    while True:
        # radio.printDetails()
        while not radio.available(0):
            # print(radio.whatHappened())
            # radio.printDetails()
            time.sleep(1/100)
            #print("not online")

        while radio.available(0):
            receivedMessage = []
            radio.read(receivedMessage, radio.getDynamicPayloadSize())
            print("Received: {}".format(receivedMessage))

            print("Translating our received Message into unicode characters...")
            string = ""

            for n in receivedMessage:
               if (n >= 32 and n <= 126):
                   string += chr(n)
            print("Our received message decodes to: {}".format(string))
            time.sleep(1)

        radio.stopListening()
        if channel == 1:
         channel = 0
        else:
         channel+=1
        radio.setChannel(channels[channel])
        radio.write(message)
        print("------Channel:", channel)
        radio.startListening()
except KeyboardInterrupt:
    print("\n")
except:
    print("erros??")
finally:
    GPIO.cleanup()
