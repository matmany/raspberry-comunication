import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

import socket

def sendToServe(tcp, dest, msg, ):
    print('try connection')
    #tcp.connect(dest)
    print('connected')
    msg = msg.encode()
    print('encoded')
    print('Enviando:', msg)
    tcp.sendall(msg)
    print('sent')
    #data = tcp.recv(1024)
    #print ('Received', repr(data))
    tcp.close()
    print('closed')
    tcp.open()

HOST = '192.168.0.100'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)

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

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, pin)
#0, 17
radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setCRCLength(2)

radio.openWritingPipe(pipes2[0])
#radio.openReadingPipe(1, pipes2[1])
radio.openReadingPipe(1,test)
radio.printDetails()
radio.startListening()
# radio.print_status()
radio.powerUp()
#print("1Node".enconde("hex"))
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
            sendToServe(tcp,dest,string)
            time.sleep(1)

        radio.stopListening()
        radio.write(message)
        radio.startListening()
except KeyboardInterrupt:
    print("\n")
except:
    print("erros??")
finally:
    GPIO.cleanup()

