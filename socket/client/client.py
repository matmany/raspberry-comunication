from lib_nrf24 import NRF24
from readArduino.dadosBombeiro import getData
import socket
import RPi.GPIO as GPIO
import time
import spidev

##########################radio Config################
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
##################################################

#############sockect config #####################
HOST = '192.168.0.23'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
#################################################


#msg1 = '283457BABYTESTE'
#
msg1 = getData(radio)
msg = msg1.encode()
while True:
    print ('Enviando:', msg)
    tcp.sendall(msg)
    data = tcp.recv(1024)
    print ('Received', repr(data))
    time.sleep(1)
