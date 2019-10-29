import socket
HOST = '192.168.0.23'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
msg1 = '283457BABYTESTE'
msg = msg1.encode()
print ('Enviando:', msg)
tcp.sendall(msg)
data = tcp.recv(1024)

print ('Received', repr(data))
tcp.close()
