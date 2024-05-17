#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
from pynput import keyboard

def on_key_press(key):
    try:
        key_str = key.char #key to string
    except :
        key_str = str(key) #for specila keys

    msg = String()
    msg.data = key_str
    pub.publish(msg)

def on_key_release(key):
    pass 

if __name__ == '__main__':
    rospy.init_node('base_node')
    rospy.loginfo('Node has started')
    pub = rospy.Publisher('/rover_control', String, queue_size=1)

    # Builtin function of pynput listener
    listener = keyboard.Listener(on_press=on_key_press, on_release=on_key_release)
    listener.start()

    rospy.spin()

    # Stop the listener when the node is stopped
    listener.stop()
    listener.join()

rospy.loginfo('Node was stopped')
