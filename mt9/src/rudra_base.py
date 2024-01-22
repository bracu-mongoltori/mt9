#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
from pynput import keyboard

def on_press(key):
    key_charc = key.char
    pub.publish(key_charc)

if __name__=='__main__':


    rospy.init_node('base_node')

    pub = rospy.Publisher('/rover_control', String, queue_size=10)



    rate = rospy.Rate(10)


    with keyboard.Listener(on_press=on_press) as listener:
        while not rospy.is_shutdown():
            rate.sleep()