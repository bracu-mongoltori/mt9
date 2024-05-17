import cv2
import pyrealsense2 as rl_cam
from ultralytics import YOLO
import numpy as np

mallet_model = YOLO('yolov8n-11-2-24-40e.pt')  
bottle_model = YOLO('yolov8n-25-2-24-40e.pt')  

#cap = cv2.VideoCapture(1)

trg_label = "Mallet" if 1 else "Bottle"
conf_threshold = 0.7

pipeline = rl_cam.pipeline()
config = rl_cam.config()
config.enable_stream(rl_cam.stream.color, 640, 480, rl_cam.format.bgr8, 30)

pipeline.start(config)

detector_params = cv2.aruco.DetectorParameters()
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
aruco_detector = cv2.aruco.ArucoDetector(aruco_dict, detector_params)

while True:
    #ret, frame = cap.read()
    frames = pipeline.wait_for_frames()
    frame = frames.get_color_frame()

    frame = np.asanyarray(frame.get_data())

    #Detecting mallets

    mallet_results = mallet_model.predict(frame)
    mallet_results = mallet_results[0]
    #im_array = result.plot()
    id_name_mapping = {value:key for key, value in mallet_results.names.items()}

    bottle_x_centre, bottle_y_centre = None, None

    for obj in mallet_results.boxes.data:
        obj = obj.tolist()
        if obj[4] > conf_threshold and obj[5] == float(id_name_mapping["Mallet"]): 
            bottle_x_centre = int((obj[0] + obj[2]) / 2)
            bottle_y_centre = int((obj[1] + obj[3]) / 2)

            cv2.circle(frame, (bottle_x_centre, bottle_y_centre), 5, (255, 0, 0), -1)
            cv2.rectangle(frame, (int(obj[0]), int(obj[1])), (int(obj[2]), int(obj[3])), (0, 0, 255), 1)
            break

    # Detecting bottles

    bottle_results = bottle_model.predict(frame)
    bottle_results = bottle_results[0]
    #im_array = result.plot()
    id_name_mapping = {value:key for key, value in bottle_results.names.items()}

    bottle_x_centre, bottle_y_centre = None, None

    for obj in bottle_results.boxes.data:
        obj = obj.tolist()
        if obj[4] > conf_threshold and obj[5] == float(id_name_mapping["Bottle"]): 
            bottle_x_centre = int((obj[0] + obj[2]) / 2)
            bottle_y_centre = int((obj[1] + obj[3]) / 2)

            cv2.circle(frame, (bottle_x_centre, bottle_y_centre), 5, (255, 0, 0), -1)
            cv2.rectangle(frame, (int(obj[0]), int(obj[1])), (int(obj[2]), int(obj[3])), (0, 0, 255), 1)
            break

    # Detecting Aruco 
        
    corners, ids, rejected = aruco_detector.detectMarkers(frame)

    if ids is not None:
        corners = corners[0][0]
        cv2.rectangle(frame, (int(corners[0][0]), int(corners[0][1])), (int(corners[2][0]), int(corners[2][1])), (0, 255, 0), 2)
        center = ( int( (corners[0][0] + corners[2][0]) // 2), int( (corners[0][1] + corners[2][1]) // 2) )
        cv2.circle(frame, center, 5, (0, 0, 255), -1)

    

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()
