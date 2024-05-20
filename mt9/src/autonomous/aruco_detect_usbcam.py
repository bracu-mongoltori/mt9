#!/usr/bin/env python3
import rospy
from std_msgs.msg import String
import numpy as np
import cv2
import sys

## This portion is for the generic USB webcam
class Webcam:
    def __init__(self):
        # Start the video capture
        self.cap = cv2.VideoCapture(1)
        if not self.cap.isOpened():
            raise ValueError("Unable to open the webcam")

    def get_frame(self):
        ret, frame = self.cap.read()
        if not ret:
            return False, None
        return True, frame

    def release(self):
        self.cap.release()
##

def calc_midpoint(x1, x2, y1, y2):
    return int((x1 + x2) / 2), int((y1 + y2) / 2)

ARUCO_DICT = {
    "DICT_4X4_50": cv2.aruco.DICT_4X4_50,
    "DICT_4X4_100": cv2.aruco.DICT_4X4_100,
    "DICT_4X4_250": cv2.aruco.DICT_4X4_250,
    "DICT_4X4_1000": cv2.aruco.DICT_4X4_1000,
    "DICT_5X5_50": cv2.aruco.DICT_5X5_50,
    "DICT_5X5_100": cv2.aruco.DICT_5X5_100,
    "DICT_5X5_250": cv2.aruco.DICT_5X5_250,
    "DICT_5X5_1000": cv2.aruco.DICT_5X5_1000,
    "DICT_6X6_50": cv2.aruco.DICT_6X6_50,
    "DICT_6X6_100": cv2.aruco.DICT_6X6_100,
    "DICT_6X6_250": cv2.aruco.DICT_6X6_250,
    "DICT_6X6_1000": cv2.aruco.DICT_6X6_1000,
    "DICT_7X7_50": cv2.aruco.DICT_7X7_50,
    "DICT_7X7_100": cv2.aruco.DICT_7X7_100,
    "DICT_7X7_250": cv2.aruco.DICT_7X7_250,
    "DICT_7X7_1000": cv2.aruco.DICT_7X7_1000,
    "DICT_ARUCO_ORIGINAL": cv2.aruco.DICT_ARUCO_ORIGINAL,
    "DICT_APRILTAG_16h5": cv2.aruco.DICT_APRILTAG_16h5,
    "DICT_APRILTAG_25h9": cv2.aruco.DICT_APRILTAG_25h9,
    "DICT_APRILTAG_36h10": cv2.aruco.DICT_APRILTAG_36h10,
    "DICT_APRILTAG_36h11": cv2.aruco.DICT_APRILTAG_36h11
}

rospy.init_node("aruco_detect")
tags = rospy.Publisher("/aruco_tag_info", String, queue_size=10)

detector_params = cv2.aruco.DetectorParameters()
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
aruco_detector = cv2.aruco.ArucoDetector(aruco_dict, detector_params)

webcam = Webcam()

if __name__ == "__main__":
    try:
        data_string = None
        while not rospy.is_shutdown():
            ar_tag_info = "0"
            state, img = webcam.get_frame()
            # cv2.imshow("stmh", img)
            if not state:
                continue
            corners, ids, rejected = aruco_detector.detectMarkers(img)

            if ids is not None:
                corners = corners[0][0]
                cv2.rectangle(img, (int(corners[0][0]), int(corners[0][1])), (int(corners[2][0]), int(corners[2][1])), (0, 255, 0), 2)
                center = ( int( (corners[0][0] + corners[2][0]) // 2), int( (corners[0][1] + corners[2][1]) // 2) )
                cv2.circle(img, center, 5, (0, 0, 255), -1)
                ar_tag_info += str("1" + " " + str(center[0]) + " " + str(center[1]) + " " + str(ids[0][0]))
                tags.publish(ar_tag_info)
            
            tags.publish(ar_tag_info)  
    finally:
        print("shutting down")
        webcam.release()
        sys.exit(0)
