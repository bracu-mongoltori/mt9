#!/usr/bin/env python3

import cv2
import pyrealsense2 as rl_cam
from ultralytics import YOLO
import numpy as np
import rospy
from std_msgs.msg import String

rospy.init_node("detectron_mt")

mallet_model = YOLO('/home/mahir/catkin_ws/src/mt9/src/autonomous/yolov8n-11-2-24-40e.pt')  

cap = cv2.VideoCapture(1)

trg_label = "Mallet"
conf_threshold = 0.7

mallet_pub = rospy.Publisher("/mallet_info", String, queue_size=10)

while not rospy.is_shutdown():
    ret, color_frame = cap.read()

    mallet_info = "0"

    # frames = pipeline.wait_for_frames()
    # color_frame = frames.get_color_frame()
    # depth_frame = frames.get_depth_frame()

    # color_frame = np.asanyarray(color_frame.get_data())
    # depth_frame = np.asanyarray(depth_frame.get_data())

    #Detecting mallets

    mallet_results = mallet_model.predict(color_frame)
    mallet_results = mallet_results[0]
    #im_array = result.plot()
    id_name_mapping = {value:key for key, value in mallet_results.names.items()}

    mallet_x_centre, mallet_y_centre = None, None

    for obj in mallet_results.boxes.data:
        obj = obj.tolist()
        if obj[4] > conf_threshold and obj[5] == float(id_name_mapping["Mallet"]): 
            mallet_x_centre = int((obj[0] + obj[2]) / 2)
            mallet_y_centre = int((obj[1] + obj[3]) / 2)

            cv2.circle(color_frame, (mallet_x_centre, mallet_y_centre), 5, (255, 0, 0), -1)
            cv2.rectangle(color_frame, (int(obj[0]), int(obj[1])), (int(obj[2]), int(obj[3])), (0, 0, 255), 1)

            mallet_info += f"1 {mallet_x_centre} {mallet_y_centre}"
            mallet_pub.publish(mallet_info)
            break
        mallet_pub.publish(mallet_info)

    cv2.imshow('frame', color_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()


