import socket
HOST = '192.168.0.23'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
orig = (HOST, PORT)
tcp.bind(orig)
tcp.listen()
#tcp.listen(1)
while True:
    print("first while")
    con, client = tcp.accept()
    print ('Conectado a', client)
    while True:
        print("waiting msg")
        msg = con.recv(1024)
        if not msg:
            print("empty msg")
            con.close() 
            break
        print ('Recebendo:', client, msg)
        con.sendall(b'ok')
        #print ('Finalizando conexao do cliente', client)
        #con.close()
        #tcp.listen()

