#! /usr/bin/env python3
import rospy
from std_msgs.msg import String, Bool, Float32
from time import sleep
from geometry_msgs.msg import Twist
import sys

rospy.init_node("aruco_move")

rover = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
status = rospy.Publisher("/status", String, queue_size=10)
rgb_led = rospy.Publisher("/rgb", Float32, queue_size=10)

r = Float32()
g = Float32()
b = Float32()

r.data = 0
g.data = 1
b.data = 2

stop = Twist()
left = Twist()
right = Twist()
forward = Twist()
backward = Twist()

left.angular.z = 50
right.angular.z = -50
forward.linear.x = 100
backward.linear.x = -100

rate = rospy.Rate(5)
img_res = (640, 480)
distance_threshold = 1.5

x_mallet, y_mallet, x_bottle, y_bottle, x_aruco, y_aruco = 0, 0, 0, 0, 0, 0
aruco_state, mallet_state, bottle_state = 0, 0, 0

def aruco_callback(msg):
    global x_aruco, y_aruco, aruco_state
    aruco_data = msg.data.split()
    if int(aruco_data[0]) != 0:
        aruco_state = 1
        x_aruco = float(aruco_data[1])
        y_aruco = float(aruco_data[2])
    else:
        aruco_state = 0

def status_stuff(status_string):
    print(status_string)
    status.publish(status_string)

def aruco_move_callback(msg: Bool):
    if msg.data == False:
        pass
    else:
        status_stuff("beginning mallet detection")
        detected = False
        while not detected:
            if aruco_state == 0:
                rover.publish(stop)
                rate.sleep()
                print("stopping")

            else:
                if x_aruco > ((img_res[0]/2) + 50):
                    rover.publish(right)
                    status_stuff("going right")
                elif x_aruco < ((img_res[0]/2) - 50):
                    rover.publish(left)
                    status_stuff("going left")
                elif x_aruco < ((img_res[0]/2) + 50) and x_aruco > ((img_res[0]/2) - 50):
                    rover.publish(forward)
                    status_stuff("going straight")
                    detected = True
            rate.sleep()
            rover.publish(stop)
            rate.sleep()
        rgb_led.publish(g)

aruco_sub = rospy.Subscriber("/aruco_tag_info", String, aruco_callback)
aruco_move = rospy.Subscriber("/aruco_move", Bool, aruco_move_callback)

if __name__ == "__main__":
    rospy.spin()