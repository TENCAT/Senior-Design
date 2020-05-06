#Name: Rachel Brundage

import socket                               #Import socket module
import sys
import select
import threading

# create a dictionary of all vehicle sensor variables
sensors = {
    "speed" : 0.0,
    "obstacles" : 0.0,
    "distance" : 0.0,
    "break" : 0.0,
    "headlights" : 0
}
# Keeps track of current command from phone
command = ""

HOST  = ''
RECV_BUFFER = 4096
PORT = 20003

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  #Create a socket object
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((HOST, PORT))  #Bind to the port
server.listen(5)

class ClientThread(threading.Thread):
    def __init__(self, client, addr):
        threading.Thread.__init__(self)
        self.csocket = client
        print("New connection added: ", addr)
    # Find out if vehicle or mobile connection
    def run(self):
        print("Connection from : " , addr)
        undefined = 1
        connectiontype = ""
        while undefined:
            ready = select.select([self.csocket], [], [], 5)
            message = ""
            if ready[0]:
                message = self.csocket.recv(4096)
                message_queue = message.decode('ascii')
                msgs = str.split(message_queue)
                first_message_array = msgs[0].split("/")
                # message_array = str.split(message)
                # Message must at least contain the device name and the message data
                if len(first_message_array) >= 2:
                    device = first_message_array[0]
                    if device == 'vehicle':
                        undefined = 0
                        connectiontype = 'vehicle'                        
                        self.vehicle(message_queue)
                    elif device == 'mobile':
                        data = first_message_array[1:]
                        undefined = 0
                        self.mobile(message_queue)

    def vehicle(self, messages):
        print("Connection is a vehicle")
        print("messages are: ", messages)
        # Message may be pretty long at this point
        # Each new message should be separated by declaration of the sender of the data
        for msg in messages.split():
            message_array = msg.split("/")
            print(message_array)
            datatype = message_array[1]
            data = message_array[2]
            # Receive input from vehicle and update sensor values
            if datatype in sensors.keys():
                if datatype == "headlights":
                    sensors[datatype] = int(data)
                else:
                    sensors[datatype] = float(data)
        print (sensors)

    def mobile(self, messages):
        print("Connection is a phone")


while True:
    client, addr = server.accept()
    # client.settimeout(30)
    newthread = ClientThread(client, addr)
    newthread.start()

