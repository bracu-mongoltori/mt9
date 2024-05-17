#include <ros.h>
#include <IBT.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

IBT omni_one(8, 9);
IBT omni_two(10, 11);

bool strafe_mode_left = false;
bool strafe_mode_right = false;
bool skid_steer = true;
bool omni_mode = false;

bool required_strafe_mode_left = false;
bool required_strafe_mode_right = false;

void check_strafe_left(){
  if (digitalRead(A0) == LOW){
    strafe_mode_left = true;
  }
  else{
    strafe_mode_left = false;
  }
}
void check_strafe_right(){
  if (digitalRead(A1) == LOW){
    strafe_mode_right = true;
  }
  else{
    strafe_mode_right = false;
  }
}

ros::NodeHandle nh;

void omni_callback(const std_msgs::String& keys){
  char state = keys.data[0];
  if (state == 'w'){
    required_strafe_mode_left = true;
    required_strafe_mode_right = true;
  }
  
}

ros::Subscriber<std_msgs::String> rover("keys", omni_callback);


void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  nh.initNode();
  nh.subscribe(rover);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);
  if (required_strafe_mode_left == true){
    check_strafe_left();
    if (strafe_mode_left != true){
      omni_one.setRawSpeed(100);
      nh.loginfo("changing mode");
      delay(100);
    }
    else{
      required_strafe_mode_left = false;
      omni_one.setRawSpeed(0);
      nh.loginfo("mode changed!");
    }
  }
  if (required_strafe_mode_right == true){
    check_strafe_right();
    if (strafe_mode_right != true){
      omni_two.setRawSpeed(100);
      nh.loginfo("changing mode");
      delay(100);
    }
    else{
      required_strafe_mode_right = false;
      omni_two.setRawSpeed(0);
      nh.loginfo("mode changed!");
    }
  }
}
