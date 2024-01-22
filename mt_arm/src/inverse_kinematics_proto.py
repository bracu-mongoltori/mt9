#! /usr/bin/env python3
import rospy 
from sensor_msgs.msg import JointState
from std_msgs.msg import String
from numpy import arctan2, arccos
from math import degrees, sqrt

link1 = 30.5
link2 = 30.5

theta_0 = 0
theta_1 = 0
theta_2 = 0


rospy.init_node("ik_test")

def coord_handler(msg: String):
    global theta_1, theta_2
    print("Received!")
    x, y = int(msg.data.split()[0]), int(msg.data.split()[1])
    theta_2_numer = link1 ** 2 + link2 ** 2 - (x ** 2 - y ** 2)
    theta_2_denom = 2 * link1 * link2

    theta_2_rad = arccos(theta_2_numer/theta_2_denom)

    theta_1a = arctan2(y, x)
    theta_1b_numer = x ** 2 + y ** 2 + link1 ** 2 - link2 ** 2
    theta_1b_denom = 2 * sqrt(x ** 2 + y ** 2) * link1
    theta_1b = arccos(theta_1b_numer/theta_1b_denom)

    theta_1_rad = theta_1a + theta_1b

    theta_1 = degrees(theta_1_rad)
    theta_2 = degrees(theta_2_rad)
    print(theta_1)
    print(theta_2)


joint_publisher = rospy.Publisher("/joint_states", JointState, queue_size=10)
coord_sub = rospy.Subscriber("/input_coords", String, coord_handler)

home_state = JointState()
target_state = JointState()
current_state = JointState()

target_state.name = ["joint_0", "joint_1", "joint_2", "joint_3", "joint_4"]
home_state.name = ["joint_0", "joint_1", "joint_2", "joint_3", "joint_4"]
current_state.name = ["joint_0", "joint_1", "joint_2", "joint_3", "joint_4"]

home_state.position = [0, 0, 0, 0, 0]
target_state.position = [theta_0, theta_1, theta_2, 0, 0]

rate = rospy.Rate(5)

print("Awaiting coordinates")
while not rospy.is_shutdown():
    joint_publisher.publish(target_state)
    rate.sleep()
