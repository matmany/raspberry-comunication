import socket
import time

HOST = '192.168.0.23'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
msg1 = '283457BABYTESTE'
msg = msg1.encode()
print ('Enviando:', msg)
while True:
  print ('sending..')
  tcp.sendall(msg)
  print('waiting to recevi')
  data = tcp.recv(1024)
  print ('Received', repr(data))
  time.sleep(1)
  print ('connecting...')
