#Name: Rachel Brundage

import socket                               #Import socket module
import sys
import select
import threading
import time

# create a dictionary of all vehicle sensor variables
sensors = {
    "name" : 0,
    "steering" : 0,
    "DayTime" : 1,
    "Distance" : 0,
    "speed" : 0
}
possible_commands = "headlights"
# Keeps track of current command from phone
cmd = ""

HOST  = ''
RECV_BUFFER = 4096
PORT = 20003

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  #Create a socket object
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((HOST, PORT))  #Bind to the port
server.listen(5)

class ClientThread(threading.Thread):
    type = ""
    def __init__(self, client, addr):
        threading.Thread.__init__(self)
        self.csocket = client
        print("New connection added: ", addr)
    # Find out if vehicle or mobile connection
    def run(self):
        undefined = 1
        connectiontype = ""
        while undefined:
            ready = select.select([self.csocket], [], [], 5)
            while not ready[0]:
                ready = select.select([self.csocket], [], [], 5)
            message = ""
            if ready[0]:
                message = self.csocket.recv(4096)
                if not message:
                    print("disconnected")
                    break
                message_queue = message.decode('utf-8')
                print("received message: ", message_queue)
                msgs = str.split(message_queue)
                first_message_array = msgs[0].split("/")
                # message_array = str.split(message)
                # Message must at least contain the device name and the message data
                if len(first_message_array) >= 2:
                    device = first_message_array[0]
                    if device == 'vehicle':
                        undefined = 0
                        connectiontype = 'vehicle'                        
                        self.vehicleInit(message_queue)
                    elif device == 'mobile':
                        data = first_message_array[1:]
                        undefined = 0
                        self.mobileInit(message_queue)

    def vehicleInit(self, messages):
        type = "vehicle"
        print("Connection is a vehicle")
        self.vehicleParseMessages(messages)
        self.receiveLoop()

    def vehicleSend(self, message):
        print("Sending message ", message, " to vehicle")
        self.csocket.send(message.encode('ascii'))

    def vehicleParseMessages(self, messages):
        global vThread
        vThread = self
        # Message may be pretty long at this point
        # Each new message should be separated by declaration of the sender of the data
        for msg in messages.split():
            print("Received message ", msg, " from vehicle")
            message_array = msg.split("/")
            datatype = message_array[1]
            data = message_array[2]
            # Receive input from vehicle and update sensor values
            if datatype in sensors.keys():
                print("Updating sensor ", datatype, "to ", data)
                if datatype == "headlights":
                    sensors[datatype] = int(data)
                else:
                    sensors[datatype] = float(data)

                if 'mThread' in globals():
                    mThread.mobileSend(msg)

    def receiveLoop(self):
        while True:
            ready = select.select([self.csocket], [], [], 5)
            message = ""
            if ready[0]:
                message = self.csocket.recv(4096)
                message_queue = message.decode('ascii')
                if type == "vehicle":
                    self.vehicleParseMessages(message)
                elif type == "mobile":
                    self.mobileParseMessages(message)

    def mobileSend(self, message):
        print("Sending message ", message, " to mobile")
        self.csocket.send(message.encode('ascii'))

    def mobileParseMessages(self, messages):
        global vThread
        global mThread
        mThread = self
        for msg in messages.split():
            print("Received message ", msg, " from Android device")
            message_array = msg.split("/")
            datatype = message_array[0]
            cmd = message_array[1]
            if cmd == "update": # Update command does not require sending to vehicle
                for key in sensors:
                    message = "vehicle/" + key + "/" + str(sensors[key]) + " "
                    self.mobileSend(message)
            elif 'vThread' in globals():
                vThread.vehicleSend(msg)

    def mobileInit(self, messages):
        print("Connection is a phone")
        type = "mobile"
        self.mobileParseMessages(messages)
        self.receiveLoop()


while True:
    client, addr = server.accept()
    # client.settimeout(30)
    newthread = ClientThread(client, addr)
    newthread.start()
