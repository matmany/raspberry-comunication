import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

def getData(radio):
    string  = None
    message = list("GETSTRING")
    while len(message) < 32:
        message.append(0)
    try:
        #start = time.time()
        while 
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
                return string
    except KeyboardInterrupt:
        print("\n")
    except:
        print("erros??")
    finally:
        GPIO.cleanup()
