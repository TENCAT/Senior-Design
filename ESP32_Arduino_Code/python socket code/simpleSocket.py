import socket
s= socket.socket()
port=7021

s.bind(('0.0.0.0',port))

s.listen(5);
while True:
    c,addr = s.accept()
    message=c.recv(4096);
    print(message.decode());
    reply="got your message"
    c.send(reply.encode())
    
