#! /usr/bin/env python3

import rospy
from std_msgs.msg import String
from time import sleep
from geometry_msgs.msg import Twist

rospy.init_node("aruco_move")

rover = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
status = rospy.Publisher("/status", String, queue_size=10)

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

x_mallet, y_mallet, x_bottle, y_bottle, x_aruco, y_aruco = 0, 0, 0, 0, 0, 0
aruco_state, mallet_state, bottle_state = 0, 0, 0

def mallet_callback(msg):
    global x_mallet, y_mallet, mallet_state
    mallet_data = msg.data.split()

    if int(mallet_data[0]) != 0:
        mallet_state = 1
        x_mallet = float(mallet_data[1])
        y_mallet = float(mallet_data[2])

    else:
        mallet_state = 0

# aruco_sub = rospy.Subscriber("/aruco_tag_info", String, aruco_callback)
# bottle_sub = rospy.Subscriber("/bottle_info", String, bottle_callback)
mallet_sub = rospy.Subscriber("/mallet_info", String, mallet_callback)

def status_stuff(status_string):
    print(status_string)
    status.publish(status_string)

key = ""

if __name__ == "__main__":
    try:
        while not rospy.is_shutdown():
            if mallet_state == 0:
                rover.publish(stop)
                rate.sleep()
                print("stopping")

            else:
                if x_mallet > ((img_res[0]/2) + 50):
                    rover.publish(right)
                    status_stuff("going right")
                elif x_mallet < ((img_res[0]/2) - 50):
                    rover.publish(left)
                    status_stuff("going left")
                elif x_mallet < ((img_res[0]/2) + 50) and x_mallet > ((img_res[0]/2) - 50):
                    rover.publish(forward)
                    status_stuff("going straight")
            rate.sleep()
            rover.publish(stop)
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
#except TypeError:   
#pass
