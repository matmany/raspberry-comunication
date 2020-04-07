import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import spidev

def configRadio(radio, pin, pipes):
    radio.begin(0, pin)
    radio.setPayloadSize(16)
    radio.setChannel(0x76)
    radio.setDataRate(NRF24.BR_1MBPS)
    radio.setPALevel(NRF24.PA_MIN)
    radio.setAutoAck(True)
    radio.enableDynamicPayloads()
    radio.enableAckPayload()
    radio.setCRCLength(2)
    radio.openWritingPipe(pipes[0])
    radio.openReadingPipe(1, pipes[1])
    radio.printDetails()
