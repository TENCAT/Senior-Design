#Name: Rachel Brundage

import socket                               #Import socket module

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)                    #Create a socket object
host = ''				    #Listens to other devices on network
port = 20003                                #Reserve a port, same as client
server.bind((host, port))                   #Bind to the port

server.listen(5)                #Allows for 5 requests before denying access

while True:
    c, addr = server.accept()               #Establish connection with client
    print ("Got connection from", addr)
    data=c.recv(1024).decode()
    print ("Received message: ", data)
    c.send("Message Received".encode('ascii'))
    #c.send(num)
    c.close()                               #Close the connection

