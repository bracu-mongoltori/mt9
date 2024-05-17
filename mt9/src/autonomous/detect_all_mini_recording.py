#!/usr/bin/env python3

import cv2
import pyrealsense2 as rl_cam
from ultralytics import YOLO
import numpy as np
import rospy
from std_msgs.msg import String

rospy.init_node("detectron_mt")

mallet_model = YOLO('./yolov8n-11-2-24-40e.pt')
bottle_model = YOLO('./yolov8n-25-2-24-40e.pt')  

cap = cv2.VideoCapture(1)

trg_label = "Mallet" if 1 else "Bottle"
conf_threshold = 0.7

fps = int(cap.get(cv2.CAP_PROP_FPS))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

output_video_path = 'recording.avi' 
fourcc = cv2.VideoWriter_fourcc(*'XVID')  
video_writer = cv2.VideoWriter(output_video_path, fourcc, fps, (width, height))

# pipeline = rl_cam.pipeline()
# config = rl_cam.config()
# config.enable_stream(rl_cam.stream.color, 640, 480, rl_cam.format.bgr8, 30)
# config.enable_stream(rl_cam.stream.depth, 640, 480, rl_cam.format.z16, 30)

# pipeline.start(config)

detector_params = cv2.aruco.DetectorParameters()
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
aruco_detector = cv2.aruco.ArucoDetector(aruco_dict, detector_params)

aruco_pub = rospy.Publisher("/aruco_tag_info", String, queue_size=10)
bottle_pub = rospy.Publisher("/bottle_info", String, queue_size=10)
mallet_pub = rospy.Publisher("/mallet_info", String, queue_size=10)

mallet_distance = 0
bottle_distance = 0
aruco_distance = 0


try:
    while True:
        ret, color_frame = cap.read()

        ar_tag_info = "0"
        mallet_info = "0"
        bottle_info = "0"

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

        # Detecting Aruco 
            
        corners, ids, rejected = aruco_detector.detectMarkers(color_frame)

        if ids is not None:
            corners = corners[0][0]
            cv2.rectangle(color_frame, (int(corners[0][0]), int(corners[0][1])), (int(corners[2][0]), int(corners[2][1])), (0, 255, 0), 2)
            center = ( int( (corners[0][0] + corners[2][0]) // 2), int( (corners[0][1] + corners[2][1]) // 2) )
            cv2.circle(color_frame, center, 5, (0, 0, 255), -1)

            ar_tag_info += str("1" + " " + str(center[0]) + " " + str(center[1]) + " " + str(ids[0][0]))
            # aruco_pub.publish(ar_tag_info)

        #Video Recoding 
            

        video_writer.write(color_frame)


        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
        aruco_pub.publish(ar_tag_info)

finally:    
    video_writer.release()
    cv2.destroyAllWindows()
    print("Completed")

video_writer.release()
cv2.destroyAllWindows()
print("Completed")
