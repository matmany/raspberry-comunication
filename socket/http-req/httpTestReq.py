import requests
payload = {'num':'1'}
r = requests.post("http://localhost/phpSillyPageTestHttpReq/phpHttpReqt.php", data=payload)
print(r.text)