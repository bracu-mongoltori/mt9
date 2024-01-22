#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
import getch
import serial
import time
def talker():
    rospy.init_node('base_node', anonymous=True)
    pub = rospy.Publisher('/rover_control', String, queue_size=10)
    valid = ["w","a","s","d","x"]
    rate = rospy.Rate(10) # 10hz
    # port = '/dev/ttyACM0'
    # baudrate = 9600
    # ser = serial.Serial(port, baudrate, timeout=1)
    while not rospy.is_shutdown():

        key = getch.getch()
        if key in valid:
            pub.publish(key)
            # ser.write(key.encode())
        # time.sleep(.1)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
