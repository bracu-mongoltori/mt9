#! /usr/bin/env python3
import rospy 
from sensor_msgs.msg import JointState

rospy.init_node("test")

def joint_callback(msg: JointState):
    names = msg.name
    position = msg.position
    print(names)
    print(position)

sub = rospy.Subscriber("/joint_states", JointState, joint_callback)

rospy.spin()