#!/usr/bin/env python3

import usbrelay_py
from time import sleep
import rospy
from std_msgs.msg import String

rospy.init_node("usbrelay_driver")
sub_topic = rospy.get_param('~topic', "/keys")

count = usbrelay_py.board_count()
print("Count: ",count)

board = usbrelay_py.board_details()[0][0]
print(board)

key_relay = {
    "r": 1,
    "f": 2,
    "t": 3,
    "g": 4,
    "y": 5,
    "h": 6,
    "o": 7,
    "p": 8
}
prev_key = "-"

def relay_callback(msg):
    global prev_key
    key = msg.data    
    if key in key_relay:
        if prev_key == "-":
            pass
        else:
            if key_relay[key] % 2 == 0 and key_relay[prev_key] == key_relay[key] - 1:
                usbrelay_py.board_control(board, key_relay[prev_key], 0)
            elif key_relay[key] % 2 != 0 and key_relay[prev_key] == key_relay[key] + 1:
                usbrelay_py.board_control(board, key_relay[prev_key], 0)
            
        usbrelay_py.board_control(board, key_relay[key], 1) 
        prev_key = key  

    if key != prev_key:
        usbrelay_py.board_control(board, key_relay[prev_key], 0)

sub = rospy.Subscriber(sub_topic, String, relay_callback)

rospy.spin()