#include <ros.h>
#include <std_msgs/String.h>

const int sw1 = A4;
const int sw2 = A5;
const int sw3 = 8; 
const int sw4 = 9; 
const int sw5 = 10;
const int sw6 = 11;
const int sw7 = 12;
const int sw8 = 13;
const int relay1 = 6;
const int relay2 = 7;
const int relay3 = 2;
const int relay4 = 3;
const int relay5 = 4;
const int relay6 = 5;

ros::NodeHandle nh;


void arm_callback(const std_msgs::String& arm_msg){
  
  if (arm_msg.data == "r") {

    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    nh.loginfo("SW1");

  } else if (arm_msg.data == "f") {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    nh.loginfo("SW2");

  } else if (arm_msg.data == "t") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    nh.loginfo("SW3");

  } else if (arm_msg.data == "g") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    nh.loginfo("SW4");

  } else if (arm_msg.data == "y") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, HIGH);
    nh.loginfo("SW5");

  } else if (arm_msg.data == "h") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, LOW);
    nh.loginfo("SW6");

  } else if (arm_msg.data == "o") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, LOW);
    nh.loginfo("SW7");

  } else if (arm_msg.data == "p") {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, HIGH);
    nh.loginfo("SW8");

  } else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    nh.loginfo("No switch");

  }
  
}

ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );

void setup() {

  //start serial connection
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(sw3, INPUT_PULLUP);
  pinMode(sw4, INPUT_PULLUP);
  pinMode(sw5, INPUT_PULLUP);
  pinMode(sw6, INPUT_PULLUP);
  pinMode(sw7, INPUT_PULLUP);
  pinMode(sw8, INPUT_PULLUP);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  nh.initNode();
  nh.subscribe(arm);

}

void loop() {

  nh.spinOnce();
  delay(1);
  
}
