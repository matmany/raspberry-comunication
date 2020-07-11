import datetime
from pymongo import MongoClient
client = MongoClient('mongodb://192.168.0.14:27017', 
    username='pi42', 
    password='12345',
    authSource='bombeiros',
    authMechanism='SCRAM-SHA-1')
db = client.bombeiros
collection  = db.raspberry
reading = {"raspID": "CCC", 
            "monoxido": 77.7, 
            "glp": 22.2, 
            "pulso": 56.2, 
            "horaio": datetime.datetime.utcnow()}
reading_id = collection.insert_one(reading)

class BombeiroDataBaseService:
    def __init__(self, raspberryId, endereco, username, password):
        authSource = 'bombeiros'
        authMechanism = 'SCRAM-SHA-1'
        self.raspberryId = raspberryId
        client = MongoClient(endereco, username=username, password=password, authSource=authSource, authMechanism=authMechanism)
        db = client.bombeiros
        self.collection = db.raspberry

    def connectToMongo(endereco, username, password, authSource, authMechanism):
        client = MongoClient(endereco, 
                    username=username, 
                    password=password,
                    authSource=authSource,
                    authMechanism=authMechanism)
    
    def insert(monoxido, glp, pulso, horaio):
        reading = {"raspID": self.raspberryId, 
            "monoxido": monoxido, 
            "glp": glp, 
            "pulso": pulso, 
            "horaio":horaio}
        reading_id = self.collection.insert_one(reading)
        if(reading_id):
            return True
        return False
        


        

