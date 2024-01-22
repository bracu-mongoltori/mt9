#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
import serial
import time

def callback(data):
    port = '/dev/ttyACM0'
    baudrate = 9600
    ser = serial.Serial(port, baudrate, timeout=1)
    key = data.data
    ser.write(key.encode())
    
    time.sleep(.1)
    print(key)

def listener():
    rospy.init_node('rover_node', anonymous=True)
    rospy.Subscriber("/rover_control", String, callback)
   

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
