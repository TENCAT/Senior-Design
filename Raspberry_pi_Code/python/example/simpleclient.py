#Name: Rachel Brundage

import socket

client=socket.socket()                  #Create a socket object

host = ''
port = 20003

print("Host and port set")

client.connect((host,port))
print("Client connected")

message = input("What message would you like to send?\n")
client.send(message.encode('ascii'))

data = client.recv(1024)
print("Incoming message from server: ")
print(data.decode('ascii'))

client.close                            #Close socket when done

