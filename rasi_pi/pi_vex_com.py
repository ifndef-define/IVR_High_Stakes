### Original Source Code for Communication ###
# import serial as ser
# import time

# v5 = ser.Serial('/dev/ttyACM1', 115200)

# while True:
#     if v5.in_waiting > 0:
#         line = v5.readline().decode('utf-8').rstrip()
#         print(f"{line}")

#     v5.write("Hello from Raspberry Pi\n".encode('utf-8'))
#     time.sleep(1)

# ======================================================= #

"""
Rasi-Pi Communication with VEX V5 Brain

The goal of this script to establish a communication interface between the RasPi and
the VEX V5 Brain. This script will be used to send and receive data only. Any one who
uses the PI, can create their own script to process the data recieved, and use this 
script to send and receive data. This script only handles the communication part. 
The actual data processing will be done by the user in their own program.

List things for this script to do:
- When started, the pi should wait for a ping from the robot to signal its ready
    The brain will always initiate the communication, pi will be waiting.
- If another start signal is recieved, the pi script will reset communications
- A message mode should be set, either binary or string (still to test)
- If the brain wants something, the brain should send related data, and wait for pi.
    The pi should not send data unless requested.
- A recieve function should be created to handle the data recieved from the brain
- A send function should be created to send data to the brain
    - If the start signal isnt recieved, the recieve/send functions should return an error
"""

import serial as ser
import time

# Baud: 115200 b/s ~ 14.4 KB/s
v5 = ser.Serial(115200)
v5.port = '/dev/ttyACM1'

connected = False

# Try to open the port, if it fails, loop for up to 30 seconds before giving up
def openPort():
    for i in range(30):
        try:
            v5.open()
        except ser.SerialException:
            time.sleep(1)

    if v5.is_open:
        connected = True
        return True
    return False

