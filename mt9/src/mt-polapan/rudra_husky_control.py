#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from time import sleep

lin_vel_x = 200
ang_vel_z = 400

# Function to translate key press to velocity commands
def key_to_cmd(key):
    if key == 'w':
        rospy.loginfo('Husky: moving forward')
        return lin_vel_x, 0.0, 0.0, 0.0, 0.0, 0.0
    elif key=='a':
        rospy.loginfo('Husky: turning left')
        return 0.0, 0.0, 0.0, 0.0, 0.0, ang_vel_z
    elif key== 's':
        rospy.loginfo('Husky: moving backward')
        return -lin_vel_x, 0.0, 0.0, 0.0, 0.0, 0.0
    elif key=='d':
        rospy.loginfo('Husky: turning right')
        return 0.0, 0.0, 0.0, 0.0, 0.0, -ang_vel_z
    else:
        rospy.loginfo('Husky: not a navigation command, stopping movement!!')
        return 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
#    sleep(1)
def husky_callback(msg):
    key = msg.data.lower()
    linear_x, linear_y, linear_z, angular_x, angular_y, angular_z = key_to_cmd(key)
    vel_cmd = Twist()
    vel_cmd.linear.x = linear_x
    vel_cmd.linear.y = linear_y
    vel_cmd.linear.z = linear_z

    vel_cmd.angular.x = angular_x
    vel_cmd.angular.y = angular_y
    vel_cmd.angular.z = angular_z
 #   sleep(1)
    pub.publish(vel_cmd)


if __name__=='__main__':
    rospy.init_node('husky_control')
    rospy.Subscriber('/keys', String, husky_callback)
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size= 10)

    rospy.spin()

