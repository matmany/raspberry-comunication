import datetime
from pymongo import MongoClient
# Class para interação com o banco de dados que armazena dados dos bombeiros
# banco de teste:
#     username='pi42'
#     password='12345',
#     authSource='bombeiros',
#     authMechanism='SCRAM-SHA-1'
#     db = client.bombeiros
#     collection  = db.raspberry


class BombeiroDataBaseService:
    def __init__(self, raspberryId, endereco, username, password):
        authSource = 'bombeiros'
        authMechanism = 'SCRAM-SHA-1'
        self.raspberryId = raspberryId
        client = MongoClient(endereco, username=username, password=password, authSource=authSource, authMechanism=authMechanism)
        db = client.bombeiros
        self.collection = db.raspberry
    
    def insert(self, monoxido, glp, pulso, horaio):
        reading = {"raspID": self.raspberryId, 
            "monoxido": monoxido, 
            "glp": glp, 
            "pulso": pulso, 
            "horaio":horaio}
        reading_id = self.collection.insert_one(reading)
        if(reading_id):
            return reading_id
        return False
