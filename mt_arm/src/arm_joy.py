#!/usr/bin/env python3

import usbrelay_py
from time import sleep
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Joy


#  D - Joystick
# axes index 1 for base angular movement
# button 2 and 3 for wrist movement
# axes index 5 for elbow movement

rospy.init_node("joy_test")

keys_pub = rospy.Publisher("/keys", String, queue_size=10)

def joy_callback(msg: Joy):
    base = msg.axes[1]
    wrist_up = msg.buttons[2]
    wrist_down = msg.buttons[3]
    elbow = msg.axes[5]
    en = msg.buttons[10]

    ENABLE = 1

    key_joy = {
    "r": wrist_down,
    "f": -wrist_up,
    "y": elbow,
    "h": -elbow,
    "o": base,
    "p": -base,
    }
    

    for keys in key_joy:
        if en:
            ENABLE += 1     
        if ENABLE % 2 == 0:
            pass
        else: 
            if key_joy[keys] > 0.1:
                keys_pub.publish(keys)

sub = rospy.Subscriber("/joy", Joy, joy_callback)

rospy.spin()
    

