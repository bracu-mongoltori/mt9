#include <ros.h>
#include <IBT.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>


IBT r_wheel(10, 9);
IBT l_wheel(6, 5);

const int relay1 = 4;
const int relay2 = 7;
const int relay3 = 2;
const int relay4 = 3;

const double b = 15;
const double r = 16.5;
const double  max_rpm = 7.2;

ros::NodeHandle nh;

void omnidir_callback(const std_msgs::String &arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'm':
    
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      delay(10);
      break;
    
    case 'n':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      delay(10);
      break;

    case 'b':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
      delay(10);
      break;
      
    case 'v':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
      delay(10);
      break;

     default:
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      break;
  }

  nh.loginfo(arm_msg.data);

  
}


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


void holo_callback(const std_msgs::String &arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'o':
    
      analogWrite(6, 200);
      analogWrite(10, 200);
      analogWrite(5, 0);
      analogWrite(9, 0);
      nh.loginfo("right");
      delay(10);
      break;
    
    case 'p':
      analogWrite(6, 0);
      analogWrite(10, 0);
      analogWrite(5, 200);
      analogWrite(9, 200);
      nh.loginfo("left");
      delay(10);
      break;

     default:
      analogWrite(6, 0);
      analogWrite(10, 0);
      analogWrite(5, 0);
      analogWrite(9, 0);
      nh.loginfo("No ting");
      break;
  }

  nh.loginfo(arm_msg.data);

  
}

ros::Subscriber<geometry_msgs::Twist> rover("cmd_vel", &cmd_vel_callback);
ros::Subscriber<std_msgs::String> omnidir("keys", &omnidir_callback);
ros::Subscriber<std_msgs::String> holo("holonomic", &holo_callback);

void setup() {
  // put your setup code here, to run once:
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  nh.initNode();
  nh.subscribe(rover);
  nh.subscribe(omnidir);
  nh.subscribe(holo);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);
}
