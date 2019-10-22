import socket
HOST = '192.168.0.??'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
msg = '283457BABYTESTE'
print 'Enviando:', msg
tcp.send(msg)
tcp.close()