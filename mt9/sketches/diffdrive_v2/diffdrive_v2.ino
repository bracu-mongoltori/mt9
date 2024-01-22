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

  char vel_l[10];
  char vel_r[10];
  char r_vel_cons[10];
  char l_vel_cons[10];
  char lin_vel[10];
  char ang_vel[10];
  char ang_vel_comp[10];
  char b_comp[10];

  double r_vel = (v + (w * (b/2)))/r;
  double l_vel = (v - (w * (b/2)))/r;
  
  dtostrf(l_vel, 4, 2, vel_l);
  dtostrf(r_vel, 4, 2, vel_r);
  
  r_vel = constrain(r_vel, -max_rpm, max_rpm);
  l_vel = constrain(l_vel, -max_rpm, max_rpm);


  dtostrf(r_vel, 4, 2, r_vel_cons);
  dtostrf(l_vel, 4, 2, l_vel_cons);
  dtostrf(v, 4, 2, lin_vel);
  dtostrf(w, 4, 2, ang_vel);
  dtostrf(w * (b/2), 4, 2, ang_vel_comp);
  dtostrf(b/2, 4, 7, b_comp);


  float pwm_r = map(r_vel,-max_rpm, max_rpm, -250, 250);
  float pwm_l = map(l_vel,-max_rpm, max_rpm, -250, 250);

  char pwm_r_char[9];
  char pwm_l_char[9];
  
  dtostrf(pwm_r, 4, 2, pwm_r_char);
  dtostrf(pwm_l, 4, 2, pwm_l_char);
  
  nh.loginfo("linear vel");
  nh.loginfo(lin_vel);  
  nh.loginfo("ang vel");
  nh.loginfo(ang_vel);  
  nh.loginfo("b comp");
  nh.loginfo(b_comp);
  nh.loginfo("ang vel comp");
  nh.loginfo(ang_vel_comp);
  nh.loginfo("raw r vel");
  nh.loginfo(vel_r);
  nh.loginfo("cons r vel");
  nh.loginfo(r_vel_cons);
  nh.loginfo("raw l vel");
  nh.loginfo(vel_l);
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
  delay(10);
}
