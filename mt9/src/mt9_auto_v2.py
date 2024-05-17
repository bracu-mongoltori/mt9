#! /usr/bin/env python3

#this version subscribes to a topic to get coords. This node pairs with the coord_tracker node.

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

rospy.init_node("autonomous")

yaw = 0.0
lat = 0.0
lon = 0.0

lin_vel = 60
ang_vel = 10

forward = Twist()
left = Twist()
right = Twist()
backward = Twist()
stop = Twist()

forward.linear.x = lin_vel
left.angular.z = ang_vel
right.angular.z = -ang_vel
backward.linear.x = -lin_vel

rate = rospy.Rate(5)

distance_threshold = 1.5

def distance_from_gps(lat1, lat2, lon1, lon2):
    #radius of earth in kilometers
    r = 6371

    lon1 = radians(lon1)
    lon2 = radians(lon2)
    lat1 = radians(lat1)
    lat2 = radians(lat2)
      
    #Haversine formula
    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * asin(sqrt(a))

    return c * r * 1000 #converts to meters
     

def haversine_distance(curr_lat, curr_lon, target_lat, target_lon):
    radius_earth = 6371000.0
    target_lat, target_lon, curr_lat, curr_lon = map(radians, [target_lat, target_lon, curr_lat, curr_lon])
    
    a = (sin((target_lat - curr_lat) / 2) ** 2) + cos(curr_lat) * cos(target_lat) * (sin((target_lon - curr_lon) / 2)) ** 2
    return radius_earth * 2.0 * asin(sqrt(a))  

def bearing(curr_lat, curr_lon, target_lat, target_lon): #Bearing to waypoint (degrees)
    target_lat, target_lon, curr_lat, curr_lon = map(radians, [target_lat, target_lon, curr_lat, curr_lon])
    d_lon = target_lon - curr_lon
    return degrees(atan2(sin(d_lon) * cos(target_lat), cos(curr_lat) * sin(target_lat) - (sin(curr_lat) * cos(target_lat) * cos(d_lon))))

def sbg_euler(msg: SbgEkfEuler):
    global yaw
    yaw = degrees(msg.angle.z)

def gps_callback(msg: SbgGpsPos):
    global lat, lon
    lat = msg.latitude
    lon = msg.longitude

def autonomous_callback(msg: SbgGpsPos):
    target_lat = msg.latitude
    target_lon = msg.longitude

    target_coords = {"latitude": target_lat, "longitude": target_lon} 
    moveTo(coords=target_coords)
    sleep(2)
    print("Enter next coordinate")
    
def status_stuff(status_string):
    print(status_string)
    status.publish(status_string)

def moveTo(coords, yaw_threshold = 10):  # coords = {"longitude": 0, "latitude": 0}
    global yaw, lat, lon, distance_threshold
    target_lat = coords["latitude"]
    target_lon = coords["longitude"]

    status_stuff("GPS: ")
    status_stuff(f"Distance to target: {distance_from_gps(lat, target_lat, lon, target_lon)}")
    while distance_from_gps(lat, target_lat, lon, target_lon) > distance_threshold:
        rover.publish(stop) # Needed for Real Rover

        distance = haversine_distance(lat, lon, target_lat, target_lon)
        target_yaw = bearing(lat, lon, target_lat, target_lon)

        upper_limit = target_yaw + yaw_threshold
        lower_limit = target_yaw - yaw_threshold

        d_yaw = abs(target_yaw - yaw)

        status_stuff("GPS: ")
        status_stuff(f"lat: {lat}, lon: {lon}")
        status_stuff(f"Distance to target: {distance}") #{distance_from_gps(lat, target_lat, lon, target_lon)}")
        status_stuff(f"current yaw: {yaw}, target yaw: {target_yaw}")
        status_stuff(f"angle left: {abs(target_yaw - yaw)}")
        status_stuff("")

        if d_yaw > 180:
            d_yaw = 360 - d_yaw

        if d_yaw > 5:
            if yaw > upper_limit:
                rover.publish(left)
                status_stuff("turning left")
            elif yaw < lower_limit:
                rover.publish(right)
                status_stuff("turning right")
            else:
                rover.publish(forward)
                status_stuff("forward")

        else:

            rover.publish(forward)
            status_stuff("forward")
                
        rate.sleep()

    status_stuff("Reached Target Coords")
    rover.publish(stop)

yaw_sub = rospy.Subscriber("/sbg/ekf_euler", SbgEkfEuler, sbg_euler)
gps_sub = rospy.Subscriber("/sbg/gps_pos", SbgGpsPos, gps_callback)
autonomous_sub = rospy.Subscriber("/autonomous", SbgGpsPos, autonomous_callback)
rover = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
status = rospy.Publisher("/status", String, queue_size=10)

if __name__ == "__main__":
    rospy.spin()

