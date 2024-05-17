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

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'r':
    
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      nh.loginfo("SW1");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;
    
    case 'f':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      nh.loginfo("SW2");
      nh.loginfo(arm_msg.data);
      delay(10);
      break;

    case 't':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      nh.loginfo("SW3");
      break;
      
    case 'g':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      nh.loginfo("SW4");
      break;
    
    case 'y':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, HIGH);
      nh.loginfo("SW5");
      break;
    
    case 'h':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, LOW);
      nh.loginfo("SW6");
      break;
    
    case 'x':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, LOW);
      nh.loginfo("SW7");
      break;
    
    case 'c':
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, HIGH);
      nh.loginfo("SW8");
      break;

    default:
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      nh.loginfo("No switch");
      break;
  }

  nh.loginfo(arm_msg.data);

  
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
  int sw1val = digitalRead(sw1);
  int sw2val = digitalRead(sw2);
  int sw3val = digitalRead(sw3);
  int sw4val = digitalRead(sw4);
  int sw5val = digitalRead(sw5);
  int sw6val = digitalRead(sw6);
  int sw7val = digitalRead(sw7);
  int sw8val = digitalRead(sw8);
  
  if (sw1val == LOW ) {

    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    Serial.println("SW1");

  } else if (sw2val == LOW ) {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("SW2");

  } else if (sw3val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);

    Serial.println("SW3");

  } else if (sw4val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
    Serial.println("SW4");

  } else if (sw5val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, HIGH);
    Serial.println("SW5");

  } else if (sw6val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, LOW);
    Serial.println("SW6");

  } else if (sw7val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, LOW);
    Serial.println("SW7");

  } else if (sw8val == LOW ) {

    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, HIGH);
    Serial.println("SW8");

  } else{
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
    Serial.println("No switch");

  }

  
  nh.spinOnce();
  delay(50);
  

}
