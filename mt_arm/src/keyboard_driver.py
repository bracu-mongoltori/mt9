#!/usr/bin/env python3

import rospy
from std_msgs.msg import String
from time import sleep
from pynput import keyboard

def talker():
    global publisher
    rospy.init_node('keyboard_driver')
    key_topic = rospy.get_param('~topic', "/keys")
    publisher = rospy.Publisher(key_topic, String, queue_size=10)
    print(key_topic)

    rate = rospy.Rate(100)

    while not rospy.is_shutdown():
        with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
            listener.join()
        rate.sleep()


def pub(instruction_key):
    rospy.loginfo(instruction_key)
    publisher.publish(instruction_key)


def on_press(key):
    try:
        msg = str(key.char)
        pub(msg if msg.isnumeric() else (msg))
    except AttributeError:
        pass

def on_release(key):
    if key == keyboard.Key.esc:
        return False
    pub('-')


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass