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

void arm_callback(const std_msgs::String& arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'm':
    
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      nh.loginfo("SW1");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;
    
    case 'n':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      nh.loginfo("SW2");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;

    case 'b':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
      nh.loginfo("SW3");
      break;
      
    case 'v':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
      nh.loginfo("SW4");
      break;

     default:
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      nh.loginfo("No switch");
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


void holo_callback(const std_msgs::String& arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'o':
    
      digitalWrite(6, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(9, LOW);
      nh.loginfo("right");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;
    
    case 'p':
      digitalWrite(6, LOW);
      digitalWrite(10, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(9, HIGH);
      nh.loginfo("left");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;

     default:
      digitalWrite(6, LOW);
      digitalWrite(10, LOW);
      digitalWrite(5, LOW);
      digitalWrite(9, LOW);
      nh.loginfo("No ting");
      break;
  }

  nh.loginfo(arm_msg.data);

  
}

ros::Subscriber<geometry_msgs::Twist> rover("cmd_vel", cmd_vel_callback);
ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );
ros::Subscriber<std_msgs::String> holo("keys", &holo_callback );



void setup() {
  // put your setup code here, to run once:
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  nh.initNode();
  nh.subscribe(rover);
  nh.subscribe(arm);
  nh.subscribe(holo);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(10);
}
