#include <ros.h>
#include <IBT.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>

IBT base(0, 1);
IBT shoulder(2, 3);
IBT elbow(4, 5);
IBT lateral_wrist(7, 6);
IBT wrist_rot(8, 9);
IBT claw(10, 11);
IBT solenoid(12, 13);

int max_vel;

ros::NodeHandle nh;


void arm_callback(const sensor_msgs::Joy& arm_msg){

  float joint_vel_pwm = map(arm_msg.axes[3], -1, 1, 0, max_vel);
  max_vel = map(joint_vel_pwm, -1, 1, 0, 255);
  
  float shoulder_pwm = map(arm_msg.axes[1], -1, 1, 0, max_vel);
  float lateral_wrist_pwm = map(arm_msg.axes[0], -1, 1, 0, max_vel);
  float wrist_rot_pwm = map(arm_msg.axes[2], -1, 1, 0, max_vel);
  float elbow_pwm = map(arm_msg.axes[5], -1, 1, 0, max_vel);

  int base_up_pwm = map(arm_msg.buttons[2], 0, 1, 0, max_vel);
  int base_down_pwm = map(arm_msg.buttons[3], 0, 1, 0, -max_vel);

  int claw_open_pwm = map(arm_msg.buttons[4], 0, 1, 0, max_vel);
  int claw_close_pwm = map(arm_msg.buttons[5], 0, 1, 0, -max_vel);

  int solenoid_open = map(arm_msg.buttons[0], 0, 1, 0, 255);

  shoulder.setRawSpeed(shoulder_pwm); nh.loginfo("Shoulder up");
  lateral_wrist.setRawSpeed(lateral_wrist_pwm); nh.loginfo("lateral wrist left");
  wrist_rot.setRawSpeed(wrist_rot_pwm); nh.loginfo("wrist rotating ccw");
  elbow.setRawSpeed(elbow_pwm); nh.loginfo("elbow up");

  if (base_up_pwm){
    base.setRawSpeed(base_up_pwm); nh.loginfo("base up");
  }
  if (base_down_pwm){
    base.setRawSpeed(base_down_pwm); nh.loginfo("base down");
  }
  if (claw_open_pwm){
    claw.setRawSpeed(claw_open_pwm); nh.loginfo("claw open");
  }
  if (claw_close_pwm){
    claw.setRawSpeed(claw_close_pwm); nh.loginfo("claw close");
  }
  
  solenoid.setRawSpeed(max_vel); nh.loginfo("solenoid pushed");
  
//  nh.loginfo(arm_msg);

  
}

ros::Subscriber<sensor_msgs::Joy> arm("joy", &arm_callback);


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
