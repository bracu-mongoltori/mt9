#! /usr/bin/env python3

import rospy
from std_msgs.msg import String
from time import sleep
import ast

rospy.init_node("all_mover")

rover = rospy.Publisher("/keys", String, queue_size=10)
status = rospy.Publisher("/status", String, queue_size=10)
status_led = rospy.Publisher("/rgb_toggle", String, queue_size=10)
rover.publish("3")
rate = rospy.Rate(20)
img_res = (640, 480)

x_mallet, y_mallet, x_bottle, y_bottle, x_aruco, y_aruco = 0, 0, 0, 0, 0, 0
aruco_state, mallet_state, bottle_state = 0, 0, 0


def bottle_callback(msg):
    global x_bottle, y_bottle, bottle_state
    bottle_data = msg.data.split()

    if int(bottle_data[0]) != 0:    
        bottle_state = 1
        x_bottle = float(bottle_data[1])
        y_bottle = float(bottle_data[2])


    else:
        bottle_state = 0


# aruco_sub = rospy.Subscriber("/aruco_tag_info", String, aruco_callback)
bottle_sub = rospy.Subscriber("/bottle_info", String, bottle_callback)
# mallet_sub = rospy.Subscriber("/mallet_info", String, mallet_callback)

def status_stuff(status_string):
    print(status_string)
    status.publish(status_string)

key = ""

if __name__ == "__main__":
    try:
        while not rospy.is_shutdown():
            if bottle_state == 0:
                print("looking for tag")
                rate.sleep()
            else:
                if x_bottle > ((img_res[0]/2) + 50):
                    key = "d"
                    status_stuff("going right")
                elif x_bottle < ((img_res[0]/2) - 50):
                    key = "a"
                    status_stuff("going left")
                elif x_bottle < ((img_res[0]/2) + 50) and x_bottle > ((img_res[0]/2) - 50):
                    key = "w"
                    status_stuff("going straight")
            rover.publish(key)
            print(key)
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
#except TypeError:   
#pass
