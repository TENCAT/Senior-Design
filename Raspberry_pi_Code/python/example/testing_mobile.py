import socket

HOST = '10.145.20.100'
PORT = 20003

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

msg = "mobile/headlights/1 "
client.send(msg.encode('ascii'))
