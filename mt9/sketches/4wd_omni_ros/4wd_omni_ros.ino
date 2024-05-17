#include <ros.h>
#include <IBT.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>

IBT l_back_wheel(8, 9);
IBT r_wheel(2, 3);
IBT r_back_wheel(4, 5);
IBT l_wheel(6, 7);

IBT omni_one(10, 11);

bool strafe_mode = false;
bool skid_steer = true;
bool omni_mode = false;

bool required_strafe_mode = false;
bool required_skid_mode = false;
bool required_lat_mode = false;

const int max_vel = 200;

const double b = 15;
const double r = 16.5;
const double  max_rpm = 7.2;

ros::NodeHandle nh;

void check_strafe(){
  if (digitalRead(A0) == LOW){
    omni_mode = true;
  }
  else{
    omni_mode = false;
  }
}

void omni_callback(const std_msgs::String& keys){
  char state = keys.data[0];
  if (state == 'w'){
    required_strafe_mode = true;
    required_lat_mode = false;
    required_skid_mode = false;
    strafe_mode = false;
    skid_steer = false;
    omni_mode = true;

  }

  if (state == 'a'){
    required_skid_mode = true;
    required_strafe_mode = false;
    required_lat_mode = false;
    strafe_mode = false;
    skid_steer = true;
    omni_mode = false;
  }
  
  if (state == 's'){
    required_skid_mode = false;
    required_strafe_mode = false;
    required_lat_mode = true;
    strafe_mode = true;
    skid_steer = false;
    omni_mode = false;
  }
  
}

void omni_manual_callback(const std_msgs::String& keys){
  
  char arm_char = keys.data[0];

  switch (arm_char){
      case 'q':
        omni_one.setRawSpeed(max_vel);
        break;
      
      case 'z':
        omni_one.setRawSpeed(-max_vel);
        break;
  }
}

void cmd_vel_callback(const geometry_msgs::Twist &vel_msg){
  if (strafe_mode == true){
    float max_w_val = 50;
    double w = vel_msg.angular.z;
    float pwm = map(w,-max_w_val, max_w_val, -255.0, 255.0);

    r_wheel.setRawSpeed(pwm);
    l_wheel.setRawSpeed(-pwm);
    r_back_wheel.setRawSpeed(-pwm);
    l_back_wheel.setRawSpeed(pwm);
  }
  else{
    double v = vel_msg.linear.x;
    double w = vel_msg.angular.z;

    double r_vel = (v + (w * (b/2)))/r;
    double l_vel = (v - (w * (b/2)))/r;
    
    r_vel = constrain(r_vel, -max_rpm, max_rpm);
    l_vel = constrain(l_vel, -max_rpm, max_rpm);

    float pwm_r = map(r_vel,-max_rpm, max_rpm, -250, 250);
    float pwm_l = map(l_vel,-max_rpm, max_rpm, -250, 250);

    r_wheel.setRawSpeed(pwm_r);
    l_wheel.setRawSpeed(pwm_l);
    r_back_wheel.setRawSpeed(pwm_r);
    l_back_wheel.setRawSpeed(pwm_l);
  }
  
}

ros::Subscriber<geometry_msgs::Twist> rover("cmd_vel", cmd_vel_callback);
ros::Subscriber<std_msgs::String> omni("keys", &omni_callback);
ros::Subscriber<std_msgs::String> omni_manual("keys", &omni_manual_callback);

void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.subscribe(rover);
  nh.subscribe(omni);
  nh.subscribe(omni_manual);
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
