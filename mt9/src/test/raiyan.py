#! /usr/bin/python3
import rospy
import math
from geometry_msgs.msg import Twist


def move():
    # Starts a new node
    rospy.init_node('follow_path', anonymous=True)
    velocity_publisher = rospy.Publisher('/husky_velocity_controller/cmd_vel', Twist, queue_size=10)

    vel_msg = Twist()

    # Since we are moving just in the x-axis
    # linear
    vel_msg.linear.x = 0.0
    vel_msg.linear.y = 0.0
    vel_msg.linear.z = 0.0

    # angular
    vel_msg.angular.x = 0.0
    vel_msg.angular.y = 0.0
    vel_msg.angular.z = 0.0

    distance = 10.0
    angle = math.radians(0.0)
    speed = 0.5
    angular_speed = math.radians(0.0)
    vel_msg.linear.x = 0.5
    vel_msg.angular.z = math.radians(0.0)

    iteration = 0
    time_prev = 0
    time_prev_loop_1 = 0
    while not rospy.is_shutdown():
        t0 = rospy.Time.now().to_sec()
        # print("time ", t0)

        if iteration == 0.0:
            current_distance = 0.0
            current_angle = math.radians(0.0)

        while(current_distance < distance):
            velocity_publisher.publish(vel_msg)
            
            t1 = rospy.Time.now().to_sec()
            t_final = rospy.Time.now().to_sec()

            current_distance = speed*(abs(t0-t_final))
            # time_prev_loop_1 = t1

            # print(t1-t0)
            print("distance ", current_distance, "time step ", t1 - t_final)

        while(current_angle < angle):
            velocity_publisher.publish(vel_msg)

            t1 = rospy.Time.now().to_sec()
            current_angle = angular_speed*(t1-t0)
            # print(t1-t0)
            # print(current_angle, angular_speed)

        vel_msg.linear.x = 0.0
        vel_msg.angular.z = math.radians(0.0)
        velocity_publisher.publish(vel_msg)

        iteration += 1
        time_prev = t0


if __name__ == '__main__':
    try: 
        move()
    except rospy.ROSInterruptException: 
        pass