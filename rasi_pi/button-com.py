"""
A simple script to demonstrate how to set-up and utilize the Vex-Pi communication
interface. This script will simply create a table of each button on the controller
and display the state and number of times the button was pressed.
"""

import pi_vex_com as vexCom
import matplotlib as mpl

# Data sent from the brain will be a binary number representing the state of the buttons
# 0101 for example will mean that button 1 and 3 are pressed. Button 1 and 3 relate to the table
# below. The raw value of the 4 joystick axis will be appended to the end of the binary number.
# Example: 10001011010.0.-15.127.-127

buttonTable = {
    0, "Up",
    1, "Down",
    2, "Left",
    3, "Right",
    4, "A",
    5, "B",
    6, "X",
    7, "Y",
    8, "L1",
    9, "L2",
    10, "R1",
    11, "R2",
    12, "Left X Axis",
    13, "Left Y Axis",
    14, "Right X Axis",
    15, "Right Y Axis"
}