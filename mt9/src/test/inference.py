import cv2 
from ultralytics import YOLO

model = YOLO('yolov8n-11-2-24-40e.pt')  
cap = cv2.VideoCapture(0)

trg_label = "Mallet" if 1 else "Bottle"
conf_threshold = 0.5

while True:
    ret, frame = cap.read()

    results = model.predict(frame)
    result = results[0]
    #im_array = result.plot()
    id_name_mapping = {value:key for key, value in result.names.items()}

    bottle_x_centre, bottle_y_centre = None, None

    for obj in result.boxes.data:
        obj = obj.tolist()
        if obj[4] > conf_threshold and obj[5] == float(id_name_mapping[trg_label]): 
            bottle_x_centre = int((obj[0] + obj[2]) / 2)
            bottle_y_centre = int((obj[1] + obj[3]) / 2)
            break

    cv2.circle(frame, (bottle_x_centre, bottle_y_centre), 5, (255, 0, 0), -1)

    cv2.imshow('frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()
