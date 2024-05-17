#include <ros.h>
#include <IBT.h>
#include <std_msgs/String.h>

IBT base(2, 3);
IBT shoulder(4, 5);
IBT elbow(6, 7);
IBT lateral_wrist(8, 9);
IBT finger(10, 11);
//IBT finger(12, 13);
IBT omni_one(12, 13);

bool strafe_mode = false;
bool skid_steer = true;
bool omni_mode = false;

bool required_strafe_mode = false;
bool required_skid_mode = false;
bool required_lat_mode = false;

const int max_vel = 200;

ros::NodeHandle nh;

void check_strafe(){
  if (digitalRead(A0) == LOW){
    strafe_mode = true;
  }
  else{
    strafe_mode = false;
  }
}

void omni_callback(const std_msgs::String& keys){
  char state = keys.data[0];
  if (state == 'w'){
    required_strafe_mode = true;
    required_lat_mode = false;
    required_skid_mode = false;

  }

  if (state == 'a'){
    required_skid_mode = true;
    required_strafe_mode = false;
    required_lat_mode = false;
  }
  
  if (state == 's'){
    required_skid_mode = false;
    required_strafe_mode = false;
    required_lat_mode = true;
  }
  
}

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
//
//    case 't':
//      wrist_rot.setRawSpeed(max_vel);
//      break;
//      
//    case 'g':
//      wrist_rot.setRawSpeed(-max_vel);
//      break;
    
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
//      wrist_rot.setRawSpeed(0);
      finger.setRawSpeed(0);
      break;
  }

  nh.loginfo(arm_msg.data);
}

ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );
ros::Subscriber<std_msgs::String> omni("keys", &omni_callback);

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT_PULLUP);
  nh.initNode();
  nh.subscribe(arm);
  nh.subscribe(omni);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);
  if (required_strafe_mode == true){
    check_strafe();
    if (strafe_mode != true){
      omni_one.setRawSpeed(max_vel);
      nh.loginfo("changing mode strafe");
      delay(10);
    }
    else{
      required_strafe_mode = false;
      omni_one.setRawSpeed(0);
      nh.loginfo("mode changed strafe!");
    }
  }

  if (required_skid_mode == true){
    omni_one.setRawSpeed(max_vel);
    nh.loginfo("changing to skid mode");
  }

  if (required_lat_mode == true){
    omni_one.setRawSpeed(-max_vel);
    nh.loginfo("changing to lateral mode");
  }

}
