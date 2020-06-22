import socket                               #Import socket module
import sys
import select
import threading
import time

# TODO:
#   -> Handle losing socket connection


# create a dictionary of all vehicle sensor variables
sensors = {
    "steering" : 2,
    "DayTime" : 1,
    "Distance" : 100,
    "speed" : 45
}
possible_commands = "headlights"
# Keeps track of current command from phone
cmd = ""

HOST  = '10.145.20.100'
RECV_BUFFER = 4096
PORT = 20003

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  #Create a socket object
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((HOST, PORT))  #Bind to the port
server.listen(5)

class ClientThread(threading.Thread):
    connectiontype = ""
    connected = 1 # Keep track of when Thread should run - terminates on socket close
    def __init__(self, client, addr):
        threading.Thread.__init__(self)
        self.csocket = client
        print("New connection added: ", addr)
    # Find out if vehicle or mobile connection
    def run(self):
        undefined = 1
        connectiontype = ""
        while undefined:
            try:
                ready = select.select([self.csocket], [], [], 5)
                while not ready[0]:
                    ready = select.select([self.csocket], [], [], 5)

                if ready[0]:
                    # get length of message
                    length_of_message = int.from_bytes(self.csocket.recv(2), byteorder='big')
                    # get message
                    message_queue = self.csocket.recv(length_of_message).decode("UTF-8")
                    if len(message_queue) == 0:
                        continue
                    msgs = str.split(message_queue)
                    first_message_array = msgs[0].split("/")
                    # message_array = str.split(message)
                    # Message must at least contain the device name and the message data
                    if len(first_message_array) >= 2:
                        device = first_message_array[0]
                        if device == 'vehicle':
                            undefined = 0                      
                            self.vehicleInit(message_queue)
                        elif device == 'mobile':
                            data = first_message_array[1:]
                            undefined = 0
                            self.mobileInit(message_queue)

            except select.error:
                print("Client disconnected")
                self.csocket.close()
                # Close the thread
                self.connected = 0
                break

    def vehicleInit(self, messages):
        self.connectiontype = "vehicle"
        print("Connection is a vehicle")
        self.vehicleParseMessages(messages)
        self.receiveLoop()

    def vehicleSend(self, message):
        print("Sending message ", message, " to vehicle")
        #encode the message to UTF-8 format
        message_to_send = message.encode("UTF-8")
        #send the length of the data first
        self.csocket.send(len(message_to_send).to_bytes(2, byteorder='big'))
        #send the data to the app
        self.csocket.send(message_to_send)

    def vehicleParseMessages(self, messages):
        global vThread
        vThread = self
        # Message may be pretty long at this point
        # Each new message should be separated by type
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

    def receiveLoop(self):
        while self.connected:
            try:
                ready = select.select([self.csocket], [], [], 5)
                message = ""
                if ready[0]:
                    length_of_message = int.from_bytes(self.csocket.recv(2), byteorder='big')
                    message = self.csocket.recv(length_of_message).decode("UTF-8")
                    if len(message) == 0:
                        continue
                    print("Message: ", message)
                    splitMsg = message.split("/")
                    if self.connectiontype == "vehicle":
                        self.vehicleParseMessages(message)
                    elif self.connectiontype == "mobile":
                        self.mobileParseMessages(message)
            except select.error:
                print(self.connectiontype.title(), " client disconnected");
                self.csocket.close()
                # Close the thread
                self.connected = 0
                break

    def mobileSend(self, message):
        print("Sending message ", message, " to mobile")
        #encode the message to UTF-8 format
        message_to_send = message.encode("UTF-8")
        #send the length of the data first
        self.csocket.send(len(message_to_send).to_bytes(2, byteorder='big'))
        #send the data to the app
        self.csocket.send(message_to_send)

    def mobileParseMessages(self, messages):
        global vThread
        global mThread
        mThread = self
        for msg in messages.split():
            print("Received message ", msg, " from Android device")
            message_array = msg.split("/")
            datatype = message_array[0]
            cmd = message_array[1]
            # Update command does not require sending to vehicle
            if cmd == "update": 
                message = "";
                for key in sensors:
                    current_msg ="vehicle/" + key + "/" \
                                    + str(sensors[key]) + " "
                    message += current_msg
                self.mobileSend(message)
            elif cmd == "close":
                print("Closing socket")
                self.csocket.shutdown(socket.SHUT_RDWR)
                self.csocket.close()
                # Close the thread
                self.connected = 0
            elif 'vcleThread' in globals():
                vThread.vehicleSend(msg)

    def mobileInit(self, messages):
        print("Connection is a phone")
        self.connectiontype = "mobile"
        self.mobileParseMessages(messages)
        self.receiveLoop()

while True:
    client, addr = server.accept()
    # client.settimeout(30)
    newthread = ClientThread(client, addr)
    newthread.start()