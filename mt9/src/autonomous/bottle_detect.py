#!/usr/bin/env python3

import cv2
import pyrealsense2 as rl_cam
from ultralytics import YOLO
import numpy as np
import rospy
from std_msgs.msg import String

rospy.init_node("detectron_mt")

bottle_model = YOLO('/home/mahir/catkin_ws/src/mt9/src/autonomous/yolov8n-25-2-24-40e.pt')  

cap = cv2.VideoCapture(1)

trg_label = "Bottle"
conf_threshold = 0.7

bottle_pub = rospy.Publisher("/bottle_info", String, queue_size=10)

while not rospy.is_shutdown():
    ret, color_frame = cap.read()

    bottle_info = "0"

    # frames = pipeline.wait_for_frames()
    # color_frame = frames.get_color_frame()
    # depth_frame = frames.get_depth_frame()

    # color_frame = np.asanyarray(color_frame.get_data())
    # depth_frame = np.asanyarray(depth_frame.get_data())

    # Detecting bottles

    bottle_results = bottle_model.predict(color_frame)
    bottle_results = bottle_results[0]
    #im_array = result.plot()
    id_name_mapping = {value:key for key, value in bottle_results.names.items()}

    bottle_x_centre, bottle_y_centre = None, None

    for obj in bottle_results.boxes.data:
        obj = obj.tolist()
        if obj[4] > conf_threshold and obj[5] == float(id_name_mapping["Bottle"]): 
            bottle_x_centre = int((obj[0] + obj[2]) / 2)
            bottle_y_centre = int((obj[1] + obj[3]) / 2)

            cv2.circle(color_frame, (bottle_x_centre, bottle_y_centre), 5, (255, 0, 0), -1)
            cv2.rectangle(color_frame, (int(obj[0]), int(obj[1])), (int(obj[2]), int(obj[3])), (0, 0, 255), 1)

            bottle_info += f"1 {bottle_x_centre} {bottle_y_centre}"
            bottle_pub.publish(bottle_info)
            break
        bottle_pub.publish(bottle_info)

    cv2.imshow('frame', color_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()


