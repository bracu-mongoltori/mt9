#include <ros.h>
#include <IBT.h>
#include <geometry_msgs/Twist.h>

IBT r_wheel(3, 5);
IBT l_wheel(6, 9);

const double b = 15;
const double r = 16.5;
const double  max_wheel_speed = 7.2;

ros::NodeHandle nh;

double pwm_l = 0;
double pwm_r = 0;

void cmd_vel_callback(const geometry_msgs::Twist &vel_msg){
  double linear_vel = vel_msg.linear.x;
  double angular_vel = vel_msg.angular.z;

  char r_vel[10];
  char l_vel[10];
  char r_vel_cons[10];
  char l_vel_cons[10];
  char linear_vel_char[10];
  char ang_vel[10];
  char ang_vel_comp[10];
  char b_comp[10];

  double r_wheel_vel = constrain(((linear_vel + (angular_vel * (b/2)))/r), -max_wheel_speed, max_wheel_speed);
  double l_wheel_vel = constrain(((linear_vel - (angular_vel * (b/2)))/r), -max_wheel_speed, max_wheel_speed);

  dtostrf(((linear_vel - (angular_vel * (b/2)))/r), 4, 2, l_vel);
  dtostrf(((linear_vel + (angular_vel * (b/2)))/r), 4, 2, r_vel);
  dtostrf(r_wheel_vel, 4, 2, r_vel_cons);
  dtostrf(l_wheel_vel, 4, 2, l_vel_cons);
  dtostrf(linear_vel, 4, 2, linear_vel_char);
  dtostrf(angular_vel, 4, 2, ang_vel);
  dtostrf((angular_vel * (b/2)), 4, 2, ang_vel_comp);
  dtostrf(b/2, 4, 7, b_comp);


  pwm_r = map(r_wheel_vel,-max_wheel_speed, max_wheel_speed, -200, 200);
  pwm_l = map(l_wheel_vel,-max_wheel_speed, max_wheel_speed, -200, 200);

  char pwm_r_char[9];
  char pwm_l_char[9];
  
  dtostrf(pwm_r, 4, 2, pwm_r_char);
  dtostrf(pwm_l, 4, 2, pwm_l_char);
  
  nh.loginfo("linear vel");
  nh.loginfo(linear_vel_char);  
  nh.loginfo("ang vel");
  nh.loginfo(ang_vel);  
  nh.loginfo("b comp");
  nh.loginfo(b_comp);
  nh.loginfo("ang vel comp");
  nh.loginfo(ang_vel_comp);
  nh.loginfo("raw r vel");
  nh.loginfo(r_vel);
  nh.loginfo("cons r vel");
  nh.loginfo(r_vel_cons);
  nh.loginfo("raw l vel");
  nh.loginfo(l_vel);
  nh.loginfo("cons l vel");
  nh.loginfo(l_vel_cons);
  nh.loginfo("mapped r pwm");
  nh.loginfo(pwm_r_char);
  nh.loginfo("mapped l pwm");
  nh.loginfo(pwm_l_char);
  nh.loginfo("\n");

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
  delay(1);
}
