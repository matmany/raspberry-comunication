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
