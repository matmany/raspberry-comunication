from bombeiroDataBaseService import BombeiroDataBaseService
import datetime

dataBase = BombeiroDataBaseService(raspberryId='TTT',
    endereco='mongodb://192.168.0.14:27017',
    username='pi42',
    password='12345')
time = datetime.datetime.utcnow()

dataBase.insert( 77.7, 77.7, 0.0, time)
