import cv2 
import numpy as np
import pyrealsense2 as rs

detector_params = cv2.aruco.DetectorParameters()
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
aruco_detector = cv2.aruco.ArucoDetector(aruco_dict, detector_params)

# cap = cv2.VideoCapture(0)

pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
pipeline.start(config)


while True:
    #ret, frame = cap.read()
    frames = pipeline.wait_for_frames()
    color_frame = frames.get_color_frame()
    depth_frame = frames.get_depth_frame()

    color_frame = np.asanyarray(color_frame.get_data())
    depth_frame = np.asanyarray(depth_frame.get_data())


    corners, ids, rejected = aruco_detector.detectMarkers(color_frame)

    if ids is not None:
        corners = corners[0][0]
        cv2.rectangle(color_frame, (int(corners[0][0]), int(corners[0][1])), (int(corners[2][0]), int(corners[2][1])), (0, 255, 0), 2)
        center = ( int( (corners[0][0] + corners[2][0]) // 2), int( (corners[0][1] + corners[2][1]) // 2) )
        cv2.circle(color_frame, center, 5, (0, 0, 255), -1)



    cv2.imshow('frame', color_frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()






