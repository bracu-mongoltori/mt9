#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <sensor_msgs/JointState.h>

ros::NodeHandle nh;

Servo joint_0;
Servo joint_1;
Servo joint_2;
Servo joint_3;
Servo joint_4;
Servo joint_5;

#define VELOCITY 0.1

String name[6] = {joint_0, joint_1, joint_2, joint_3, joint_4, joint_5};
int pins[6] = {};
int position[6] = {90, 90, 90, 90, 90, 90};

void joint_callback(const sensor_msgs::JointState& joint_msg){
    position = joint_msg.position;

}

ros::Subscriber<sensor_msgs::JointState> joint_sub("/joint_states", joint_callback)

void setup(){
    for (i = 0; i < 7; i++){
        name[i].attach(pins[i]);
        name[i].write(position[i]);

        nh.initNode();
        nh.subscribe(joint_sub)
    }

}

void loop(){
    for (i = 0; i < 7; i++){
        name[i].write(position[i]);
    } 
}