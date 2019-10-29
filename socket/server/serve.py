import socket
HOST = '192.168.0.100'
PORT = 9090
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
orig = (HOST, PORT)
tcp.bind(orig)
tcp.listen()
#tcp.listen(1)
while True:
    con, client = tcp.accept()
    print ('Conectado a', client)
    while True:
        msg = con.recv(1024)
        if not msg:
            print("empty msg")
            con.close() 
            break
        print ('Recebendo:', client, msg)
        #con.sendall(b'serveToYou998882943')
        #print ('Finalizando conexao do cliente', client)
        #con.close()

