import requests
num = 1
data = {'data': num}
server = "http://192.168.0.37:8081/phpSillyPageTestHttpReq/phpHttpReqt.php"
r = requests.post(server, data=data)
print(r.text)