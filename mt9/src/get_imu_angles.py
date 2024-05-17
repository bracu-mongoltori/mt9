#! /usr/bin/env python3

import rospy
from std_msgs.msg import String
from sensor_msgs.msg import Imu, NavSatFix
from tf import transformations
from geometry_msgs.msg import Twist
from tf.transformations import euler_from_quaternion, quaternion_from_euler
from math import radians, cos, sin, asin, sqrt, atan2, degrees
from time import sleep
from sbg_driver.msg import SbgGpsPos, SbgEkfEuler
import numpy as np

rospy.init_node("imu_angles")

rate = rospy.Rate(4)

yaw = 0
lat = 0
lon = 0

# def imu_callback(msg: Imu):
#     global yaw
#     orientation = msg.orientation
#     (_, _, yaw) = euler_from_quaternion([orientation.x, orientation.y, orientation.z, orientation.w])
#     print(yaw)

def bearing(curr_lat, curr_lon, target_lat, target_lon): #Bearing to waypoint (degrees)
    target_lat, target_lon, curr_lat, curr_lon = map(radians, [target_lat, target_lon, curr_lat, curr_lon])
    d_lon = target_lon - curr_lon
    return degrees(atan2(sin(d_lon) * cos(target_lat), cos(curr_lat) * sin(target_lat) - (sin(curr_lat) * cos(target_lat) * cos(d_lon))))

def sbg_euler(msg: SbgEkfEuler):
    global yaw
    yaw = degrees(msg.angle.z) 
    

def sbg_gps(msg: SbgGpsPos):
    global lat, lon
    lat = msg.latitude
    lon = msg.longitude

def check_shit(targ_lat, targ_lon):
    global lat, lon
    target_bearing = bearing(lat, lon, targ_lat, targ_lon)
    print(f"current yaw {yaw}")
    print(f"target yaw {target_bearing}")
    print(f"left {yaw - target_bearing}")

# imu_sub = rospy.Subscriber("/imu/data", Imu, imu_callback)
yaw_sub = rospy.Subscriber("/sbg/ekf_euler", SbgEkfEuler, sbg_euler)
gps_sub = rospy.Subscriber("/sbg/gps_pos", SbgGpsPos, sbg_gps)

# coords = {"longitude": 90.451727444, "latitude": 23.768288209999998}
# coords = {"longitude": 90.451634, "latitude": 23.768023}
coords = {"longitude": 90.452106, "latitude": 23.767987}

while not rospy.is_shutdown():
    check_shit(coords["latitude"], coords["longitude"])
    rate.sleep()