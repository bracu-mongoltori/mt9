#!/usr/bin/env python3

import usbrelay_py
from time import sleep
import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Joy

"""
base - t g - axes 
shoulder - r f 
elbow - y h 
wrist rotation - n m 
claw open close - o p 
linear translation - z x 
"""
rospy.init_node("joy_test")

rate = rospy.Rate(1)

keys_pub = rospy.Publisher("/keys", String, queue_size=10)

def joy_callback(msg: Joy):

    key_joy_axes = {
    "y": [5, 1],
    "h": [5, -1],
    "r": [1, 1],
    "f": [1, -1],
    "o": [2, 1],
    "p": [2, -1],
    }

    key_joy_buttons = {
    "o": [5, 1],
    "p": [4, 1],
    }

    axes = list(msg.axes)
    buttons = list(msg.buttons)


    if 1 in axes:
        axis = axes.index(1)
        dir = axes[axis]
        print("+ axes", axis)
        print(list(key_joy_axes.keys())[list(key_joy_axes.values()).index([axis, dir])])
    
    elif -1 in axes:
        axis = axes.index(-1)
        dir = axes[axis]
        print("- axes", axis)
        print(list(key_joy_axes.keys())[list(key_joy_axes.values()).index([axis, dir])])
    
    else:
        axis = None
        
    if -1 in buttons:
        button = buttons.index(-1)
        dir = buttons[button]
        print("- buttons", button)
        
    elif 1 in buttons:
        button = buttons.index(1)
        dir = buttons[button]
        print("+ buttons", button)
    
    else:
        button = None

sub = rospy.Subscriber("/joy", Joy, joy_callback)

rospy.spin()
    