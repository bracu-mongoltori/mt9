#include <ros.h>
#include <IBT.h>
#include <geometry_msgs/Twist.h>

IBT r_wheel(6, 5);
IBT l_wheel(10, 9);

const double b = 15;
const double r = 16.5;
const double  max_rpm = 7.2;

ros::NodeHandle nh;

void cmd_vel_callback(const geometry_msgs::Twist &vel_msg){
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
}

ros::Subscriber<geometry_msgs::Twist> rover("cmd_vel", cmd_vel_callback);

void setup() {
  // put your setup code here, to run once:
  nh.initNode();
  nh.subscribe(rover);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);
}
