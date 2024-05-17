#! /usr/bin/env python3

import rospy
from std_msgs.msg import String
from time import sleep
import ast

rospy.init_node("parser_test")

rover = rospy.Publisher("/keys", String, queue_size=10)
rover.publish("3")
rate = rospy.Rate(20)
img_res = (640, 480)

x_mallet, y_mallet, x_bottle, y_bottle, x_aruco, y_aruco = 0, 0, 0, 0, 0, 0

def aruco_callback(msg):
    global x_aruco, y_aruco
    aruco_data = msg.data.split()

    aruco_state = 1
    x_aruco = aruco_data[1]
    y_aruco = aruco_data[2]

    print(x_aruco, y_aruco)

def mallet_callback(msg):
    global x_mallet, y_mallet
    mallet_data = msg.data.split()

    aruco_state = 1
    x_mallet = mallet_data[0]
    y_mallet = mallet_data[1]

    print(x_mallet, y_mallet)

def bottle_callback(msg):
    global x_bottle, y_bottle
    bottle_data = msg.data.split()

    aruco_state = 1
    x_bottle = bottle_data[0]
    y_bottle = bottle_data[1]

    print(x_bottle, y_bottle)


aruco_sub = rospy.Subscriber("/aruco_tag_info", String, aruco_callback)
bottle_sub = rospy.Subscriber("/bottle_info", String, bottle_callback)
mallet_sub = rospy.Subscriber("/mallet_info", String, mallet_callback)

rospy.spin()
