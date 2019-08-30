import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
#ULtimo que deu certo!!!!
GPIO.setmode(GPIO.BCM)
pin = 19
#GPIO.setup(pin, GPIO.OUT) -- corrigido na lib!!!
#GPIO.output(pin, False)
#pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
pipes = ["1Node","2Node","3Node"]
pipe1 = b"1Node"
pipes2 = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0,pin)
#0, 17
radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()
radio.setCRCLength(2)

radio.openReadingPipe(1, pipes2[1])
radio.printDetails()
radio.startListening()
#radio.print_status()
radio.powerUp()
try:
 while True:
     #radio.printDetails()
     while not radio.available(0):
         #print(radio.whatHappened())
         #radio.printDetails()
         time.sleep(1/100)
         #print("not online")

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
except KeyboardInterrupt:
     print("\n")
except:
     print("erros??")
finally:
     GPIO.cleanup()
#>
