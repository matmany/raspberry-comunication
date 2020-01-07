import socket
import time

HOST = '192.168.0.37'
PORT = 9090

while True:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(b'Hello, world')
        data = s.recv(1024)
    print(data)    
    time.sleep(1)
print('Received', repr(data))