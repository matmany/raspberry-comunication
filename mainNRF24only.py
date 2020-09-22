import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
from bombeiroDataBaseService import BombeiroDataBaseService
import time
import datetime
import spidev
import radio_config as config
import breakString

GPIO.setmode(GPIO.BCM)

raspId = "ABC"

#Configuração da conexão soket com banco
dataBase = BombeiroDataBaseService(raspId,
    endereco='mongodb://192.168.0.14:27017',
    username='pi42',
    password='12345')
#-----------------------

#Confinguração do radio:
radioPin = 19
pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]
radio = NRF24(GPIO, spidev.SpiDev())
config.configRadio(radio, radioPin, pipes)
radio.startListening()
radio.powerUp()
#----------------------
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
            trama = format(string)
            print(trama)
            print("Our received message decodes to: {}".format(string))
            time.sleep(1)
            # pegar dados da string dos Arduinos string[2:5]
            #print("almost started")
            data = breakString.tramaValues(trama)
            
            #print(data["id"])
            #print(data["s1"])
            #print(data["s2"])
            currenTtime = datetime.datetime.utcnow()
            dataBase.insert(data["s1"], data["s2"], 0.0, currenTtime) 

            #time.sleep(1)

        print("end of message")
except KeyboardInterrupt:
    print("\n")
except:
    print("erros??")
finally:
    GPIO.cleanup()
#>

