#!/usr/bin/env python3

import cv2 
import numpy as np
import pyrealsense2 as rs
import rospy
from std_msgs.msg import String

detector_params = cv2.aruco.DetectorParameters()
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
aruco_detector = cv2.aruco.ArucoDetector(aruco_dict, detector_params)

rospy.init_node("tag_detect")
aruco_pub = rospy.Publisher("/aruco_tag_info", String, queue_size=10)

cap = cv2.VideoCapture(3)

# pipeline = rs.pipeline()
# config = rs.config()
# config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
# config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
# pipeline.start(config)


while not rospy.is_shutdown():
    ret, frame = cap.read()
    # frame = pipeline.wait_for_frames()
    # frame = frame.get_color_frame()
    # frame = np.asanyarray(frame.get_data())

    tag_info = ""


    corners, ids, rejected = aruco_detector.detectMarkers(frame)

    if ids is not None:
        corners = corners[0][0]
        cv2.rectangle(frame, (int(corners[0][0]), int(corners[0][1])), (int(corners[2][0]), int(corners[2][1])), (0, 255, 0), 2)
        center = ( int( (corners[0][0] + corners[2][0]) // 2), int( (corners[0][1] + corners[2][1]) // 2) )
        cv2.circle(frame, center, 5, (0, 0, 255), -1)

        print(ids[0][0])
        print(center)

        tag_info += str("1" + " " + str(center[0]) + " " + str(center[1]) + " " + str(ids[0][0]))
        aruco_pub.publish(tag_info)


    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()






