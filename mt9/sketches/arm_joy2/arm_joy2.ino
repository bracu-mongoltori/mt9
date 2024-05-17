#include <ros.h>
#include <IBT.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>

IBT solenoid(0, 1);
IBT base(2, 3);
IBT shoulder(4, 5);
IBT elbow(6, 7);
IBT lateral_wrist(8, 9);
IBT wrist_rot(10, 11);
IBT finger(12, 13);

int max_vel;
const float st = 0.5; // stick threshold
ros::NodeHandle nh;


void arm_callback(const sensor_msgs::Joy& arm_msg){

  float joint_vel_pwm = map(arm_msg.axes[3], -1, 1, 0, max_vel);
  max_vel = map(joint_vel_pwm, -1, 1, 0, 255);
  
  const float shoulder_val = arm_msg.axes[1];
  float lateral_wrist_val = arm_msg.axes[0];
  float wrist_rot_val = arm_msg.axes[2];
  float elbow_val = arm_msg.axes[5];

  int base_up_val = arm_msg.buttons[2];
  int base_down_val = arm_msg.buttons[3];

  int claw_open_val = arm_msg.buttons[4];
  int claw_close_val = arm_msg.buttons[5];

  int solenoid_val = arm_msg.buttons[0];

  switch (true){
    case (st < shoulder_val):
      shoulder.setRawSpeed(max_vel);
      break;
      
    case (st > shoulder_val ):
      shoulder.setRawSpeed(-max_vel);
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
