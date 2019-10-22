import socket
HOST = ''
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
orig = (HOST, PORT)
tcp.bind(orig)
tcp.listen(1)
while True:
    con, client = tcp.accept()
    print 'Conectado a', client
    while True:
        msg = con.recv(1024)
        if not msg: break
        print 'Recebendo:', client, msg
        print 'Finalizando conexao do cliente', client
        con.close()

