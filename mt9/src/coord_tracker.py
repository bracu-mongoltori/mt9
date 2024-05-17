import rospy
from sensor_msgs.msg import Joy
from sbg_driver.msg import SbgGpsPos
from std_msgs.msg import Float32

rospy.init_node("coordinate_tracker")

coord_list = []

def gps_callback(msg: SbgGpsPos):
    global lat, lon
    lat = msg.latitude
    lon = msg.longitude
    
def joy_callback(msg: Joy):
    global lat, lon, coord_list
    R2 = 0
    X = 1
    B = 2

    coordinate = SbgGpsPos()
    coordinate.latitude = lat
    coordinate.longitude = lon

    if msg.buttons[R2] == 1:
        coord_list.append([coordinate])
    if msg.button[X] == 1:
        print(coord_list)
    if msg.button[B] == 1:
        autonomous_pub.publish(coord_list[0][0])
        coord_list.pop(0)

gps_sub = rospy.Subscriber("/sbg/gps_pos", SbgGpsPos, gps_callback)
joy_sub = rospy.Subscriber("/joy", Joy, joy_callback)
autonomous_pub = rospy.Publisher("/autonomous", SbgGpsPos, queue_size=10)

rospy.spin()