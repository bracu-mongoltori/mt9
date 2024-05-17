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

// Define ROS node handle
ros::NodeHandle nh;

const int st = 175;

// Define joint max velocities
const float MAX_VEL = 255;   // Example value, adjust as needed
const int max_vel = 200;

// Define joint speeds
float shoulder_speed = 0.0;
float lateral_wrist_speed = 0.0;
float wrist_rot_speed = 0.0;
float elbow_speed = 0.0;

// Define button indices
const int BASE_UP_BUTTON = 2;
const int BASE_DOWN_BUTTON = 3;
const int CLAW_OPEN_BUTTON = 4;
const int CLAW_CLOSE_BUTTON = 5;
const int SOLENOID_BUTTON = 1;

// Callback function for joy messages
void joyCallback(const sensor_msgs::Joy& joy_msg) {
  // Handle axes inputs
  shoulder_speed = (joy_msg.axes[1] * MAX_VEL) * -1;
  lateral_wrist_speed = (joy_msg.axes[0] * MAX_VEL) * -1;
  wrist_rot_speed = (joy_msg.axes[2] * MAX_VEL) * -1;
  elbow_speed = (joy_msg.axes[5] * MAX_VEL) * -1;


  // Handle buttons inputs
  if (joy_msg.buttons[BASE_UP_BUTTON] == 1) {
    // Move base up at max velocity
    // Example code: base_up_joint.setRawSpeed(MAX_SPEED)
    base.setRawSpeed(MAX_VEL);

  } else if (joy_msg.buttons[BASE_DOWN_BUTTON] == 1) {
    // Move base down at max velocity
    // Example code: base_down_joint.setRawSpeed(-MAX_SPEED);
    base.setRawSpeed(-MAX_VEL);

  }
  else{
    base.setRawSpeed(0);  
  }

  if (joy_msg.buttons[CLAW_OPEN_BUTTON] == 1) {
    // Open claw at max velocity
    // Example code: claw_joint.setRawSpeed(MAX_SPEED);
    finger.setRawSpeed(MAX_VEL);
    nh.loginfo("opening claw");

  } else if (joy_msg.buttons[CLAW_CLOSE_BUTTON] == 1) {
    // Close claw at max velocity
    // Example code: claw_joint.setRawSpeed(-MAX_SPEED);
    finger.setRawSpeed(-MAX_VEL);
    nh.loginfo("closing claw");
 
  }
  else{
    finger.setRawSpeed(0);  
  }
//
//  if (joy_msg.buttons[SOLENOID_BUTTON] == 1) {
//    // Activate solenoid
//    // Example code: activateSolenoid();
//    solenoid.setRawSpeed(255);  
//
//  }
//  else{
//    solenoid.setRawSpeed(0);  
//  }

  shoulder.setRawSpeed(shoulder_speed);
  lateral_wrist.setRawSpeed(lateral_wrist_speed);
  wrist_rot.setRawSpeed(wrist_rot_speed);
  elbow.setRawSpeed(elbow_speed);
  
}


void arm_callback(const std_msgs::String& arm_msg){

  char arm_char = arm_msg.data[0];

  switch (arm_char){

    case 'u':
      base.setRawSpeed(max_vel);
      break;
    
    case 'j':
      base.setRawSpeed(-max_vel);
      break;

    case 'i':
      shoulder.setRawSpeed(max_vel);
      break;
      
    case 'k':
      shoulder.setRawSpeed(-max_vel);
      break;
    
    case ',':
      elbow.setRawSpeed(max_vel);
      break;
    
    case '.':
      elbow.setRawSpeed(-max_vel);
      break;
      
    case 'r':
      lateral_wrist.setRawSpeed(max_vel);
      break;
    
    case 'f':
      lateral_wrist.setRawSpeed(-max_vel);
      break;

    case 't':
      wrist_rot.setRawSpeed(max_vel);
      break;
      
    case 'g':
      wrist_rot.setRawSpeed(-max_vel);
      break;
    
    case 'y':
      finger.setRawSpeed(max_vel);
      break;
    
    case 'h':
      finger.setRawSpeed(-max_vel);
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

  nh.loginfo(arm_msg.data);

  
}

ros::Subscriber<sensor_msgs::Joy> joy_sub("joy", joyCallback);
ros::Subscriber<std_msgs::String> arm("keys", &arm_callback );

void setup() {
  // Initialize ROS node
  nh.initNode();

  // Subscribe to joy topic
  nh.subscribe(joy_sub);
  nh.subscribe(arm);


  // Initialize your hardware and other setup code here
}

void loop() {
  // Handle ROS communication
  nh.spinOnce();

  // Move joints according to the calculated speeds
  // Example code: shoulder_joint.setRawSpeed(shoulder_speed);
  // Similar code for other joints
}
