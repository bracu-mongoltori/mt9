#! /usr/bin/env python3

import rospy
from std_msgs.msg import String
from time import sleep
import ast

rospy.init_node("aruco_move")

rover = rospy.Publisher("/keys", String, queue_size=10)
status = rospy.Publisher("/status", String, queue_size=10)
status_led = rospy.Publisher("/rgb_toggle", String, queue_size=10)
rover.publish("3")
rate = rospy.Rate(0.25)
img_res = (640, 480)

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

aruco_sub = rospy.Subscriber("/aruco_tag_info", String, aruco_callback)


def status_stuff(status_string):
    print(status_string)
    status.publish(status_string)

key = ""

if __name__ == "__main__":
    try:
        while not rospy.is_shutdown():
            if aruco_state == 0:
                print("looking for tag")
                rover.publish("a")
                rate.sleep()
            else:
                if x_aruco > ((img_res[0]/2) + 50):
                    key = "d"
                    rover.publish(key)
                    status_stuff("going right")
                elif x_aruco < ((img_res[0]/2) - 50):
                    key = "a"
                    rover.publish(key)
                    status_stuff("going left")
                elif x_aruco < ((img_res[0]/2) + 50) and x_aruco > ((img_res[0]/2) - 50):
                    key = "w"
                    rover.publish(key)
                    status_stuff("going straight")
            #rover.publish(key)
            print(key)
            rate.sleep()
            # sleep(1.5)
    except rospy.ROSInterruptException:
        pass
#except TypeError:   
#pass
