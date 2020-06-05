import socket

HOST = '10.145.20.100'
PORT = 20003

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))

sensors = {
    "steering" : 1,
    "DayTime" : 0,
    "Distance" : 200,
    "speed" : 50
}

# msg = "   vehicle/speed/45 "
# client.send(msg.encode('ascii'))

for sensor in sensors:
    msg = "   vehicle/" + sensor + "/" + str(sensors[sensor]) + " "
    client.send(msg.encode())

while 1:
    print("Received message from server: ", client.recv(4096).decode('ascii'))
