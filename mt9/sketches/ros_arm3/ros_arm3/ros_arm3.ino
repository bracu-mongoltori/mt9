#include <ros.h>
#include <std_msgs/String.h>

const int relay1 = 2;
const int relay2 = 3;
const int relay3 = 4;
const int relay4 = 5;
const int relay5 = 6;
const int relay6 = 7;
const int relay7 = 8;
const int relay8 = 9;
const int relay9 = 10;
const int relay10 = 11;
const int relay11 = 12;
const int relay12 = 13;

ros::NodeHandle nh;


void arm_callback(const std_msgs::String& arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'r':
    
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
    
    case 'f':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;

    case 't':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
      
    case 'g':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
    
    case 'y':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
    
    case 'h':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, HIGH);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
    
    case 'o':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, HIGH);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
    
    case 'p':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, HIGH);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;

    case 'u':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, HIGH);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;

    case 'j':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, HIGH);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;

    case 'i':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, HIGH);
      digitalWrite(relay12, LOW);
      break;

    case 'k':
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, HIGH);
      break;
      
    default:
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      digitalWrite(relay9, LOW);
      digitalWrite(relay10, LOW);
      digitalWrite(relay11, LOW);
      digitalWrite(relay12, LOW);
      break;
  }

  nh.loginfo(arm_msg.data);

  
}

ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );

void setup() {
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  pinMode(relay9, OUTPUT);
  pinMode(relay10, OUTPUT);
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);

  nh.initNode();
  nh.subscribe(arm);

}

void loop() {  
  
  nh.spinOnce();
  delay(50);
  

}
