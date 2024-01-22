#include <IBT.h>
#include <ros.h>
#include <std_msgs/String.h>

#define joint1_LPWM 3
#define joint1_RPWM 5
#define joint2_LPWM 6
#define joint2_RPWM 9
#define joint3_LPWM 10
#define joint3_RPWM 11

#define vel 100

ros::NodeHandle nh;

IBT joint1(3, 5);
IBT joint2(6, 9);
IBT joint3(10, 11);

// IBT joint3(10, 11);


void keysCallback( const std_msgs::String& keys){
    char state = keys.data[0];

    switch (state){
      case 'r':
        joint1.setRawSpeed(vel);
        delay(100);
        joint1.stop_();
        nh.loginfo("r");
        break;
      case 'f':
        joint1.setRawSpeed(-vel);
        delay(vel);
        joint1.stop_();
        nh.loginfo("f");
        break;
      case 't':
        joint2.setRawSpeed(vel);
        delay(vel);
        joint2.stop_();
        nh.loginfo("t");
        break;
      case 'g':
        joint2.setRawSpeed(-vel);
        delay(vel);
        joint2.stop_();
        nh.loginfo("g");
        break;
      case 'y':
        joint3.setRawSpeed(vel);
        delay(vel);
        joint3.stop_();
        nh.loginfo("y");
        break;
      case 'h':
        joint3.setRawSpeed(-vel);
        delay(vel);
        joint3.stop_();
        nh.loginfo("h");
        break;
      default:
        joint1.stop_();
        joint2.stop_();
        joint3.stop_();
        break;
    }
  }


ros::Subscriber<std_msgs::String> arm("keys", &keysCallback );

void setup()
{ 
  nh.initNode();
  nh.subscribe(arm);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}
