import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
import radio_config as config

GPIO.setmode(GPIO.BCM)
pin = 19
pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
radio = NRF24(GPIO, spidev.SpiDev())
config.configRadio(radio, pin, pipes)
radio.startListening()
radio.powerUp()
try:
    while True:
        print("start")
        while not radio.available(0):
            time.sleep(1)
            print("not online")

        while radio.available(0):
            print("Is available")
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

        print("end of message")
except KeyboardInterrupt:
    print("\n")
except:
    print("erros??")
finally:
    GPIO.cleanup()
#>

