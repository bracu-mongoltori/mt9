#include <ros.h>
#include <IBT.h>
#include <std_msgs/String.h>

IBT base(2, 3);
IBT shoulder(4, 5);
IBT elbow(6, 7);
IBT lateral_wrist(8, 9);
IBT wrist_rot(10, 11);
IBT finger(12, 13);

const int max_vel = 200;

ros::NodeHandle nh;


void arm_callback(const std_msgs::String& arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'u':
      base.setRawSpeed(max_vel);
      break;
    
    case 'j':
      base.setRawSpeed(-max_vel);
      break;

    case 'i':
      shoulder.setRawSpeed(max_vel);
      break;
      
    case 'k':
      shoulder.setRawSpeed(-max_vel);
      break;
    
    case 'o':
      elbow.setRawSpeed(max_vel);
      break;
    
    case 'l':
      elbow.setRawSpeed(-max_vel);
      break;
      
    case 'r':
      lateral_wrist.setRawSpeed(max_vel);
      break;
    
    case 'f':
      lateral_wrist.setRawSpeed(-max_vel);
      break;

    case 't':
      wrist_rot.setRawSpeed(max_vel);
      break;
      
    case 'g':
      wrist_rot.setRawSpeed(-max_vel);
      break;
    
    case 'y':
      finger.setRawSpeed(max_vel);
      break;
    
    case 'h':
      finger.setRawSpeed(-max_vel);
      break;
    
      
    default:
      base.setRawSpeed(0);
      shoulder.setRawSpeed(0);
      elbow.setRawSpeed(0);
      lateral_wrist.setRawSpeed(0);
      wrist_rot.setRawSpeed(0);
      finger.setRawSpeed(0);
      break;
  }

  nh.loginfo(arm_msg.data);

  
}

ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );


void setup() {
  // put your setup code here, to run once:

  nh.initNode();
  nh.subscribe(arm);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);

}
