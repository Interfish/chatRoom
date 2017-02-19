from socket import *

HOST = ''
PORT = 1234
BUFSIZ = 1024
ADDR = (HOST,PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)

while True:
    print 'sever address:',HOST,'\nwaiting for connection...'
    tcpCliSock,addr = tcpSerSock.accept()
    print '...connected from: ',addr

    while True:
        data = tcpCliSock.recv(BUFSIZ)
        if not data:
            break
        print "="*20
        print "[From Client]:",data
        message = raw_input("Service>")
        tcpCliSock.send(message)
tcpSerSock.close()