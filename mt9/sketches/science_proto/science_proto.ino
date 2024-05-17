// gas sensor, UV sensor, barometric sensor, multiple gas sensor

#include <ros.h>
#include <std_msgs/Float32.h>

int mq7 = A0;
int UV_OUT = A2;
int UV_EN = 10;
int pressure_pin = 20;

// atmospheric quantities
std_msgs::Float32 temp_msg;
std_msgs::Float32 uv_msg;
std_msgs::Float32 pressure_msg;
std_msgs::Float32 monoxide_msg;

// soil quantities
std_msgs::Float32 nitrogen_msg;
std_msgs::Float32 phosphorus_msg;
std_msgs::Float32 potassium_msg;
std_msgs::Float32 ph_msg;
std_msgs::Float32 ec_msg;
std_msgs::Float32 humidity_msg;
std_msgs::Float32 soil_temp_msg;

// ros science nodes
// atmospheric 
ros::Publisher pub_temp("temperature", &temp_msg);
ros::Publisher pub_uv("uv", &uv_msg);
ros::Publisher pub_pressure("pressure", &pressure_msg);
ros::Publisher pub_monoxide("monoxide", &monoxide_msg);

// soil
ros::Publisher pub_nitrogen("nitrogen", &nitrogen_msg);
ros::Publisher pub_phosphorus("phosphorus", &phosphorus_msg);
ros::Publisher pub_potassium("potassium", &potassium_msg);
ros::Publisher pub_ph("ph", &ph_msg);
ros::Publisher pub_ec("ec", &ec_msg);
ros::Publisher pub_humidity("humidity", &humidity_msg);
ros::Publisher pub_soil_temp("soil_tempurature", &soil_temp_msg);

ros::NodeHandle nh;
long publisher_timer;

void uv(){
  // some processing
  float temperature;
  int uv_Level = analogRead(UV_OUT);
  int en = analogRead(UV_EN);

  float output_Voltage = 3.3 / en * uv_Level;
  float uvIntensity = map(output_Voltage, 0.99, 2.8, 0.0, 15.0);
  // pub the message
  uv_msg.data = uvIntensity;
  pub_uv.publish(&uv_msg);
  
}
void temp(){
  // some processing
  float temperature;
  // pub the message
  temp_msg.data = temperature;
  pub_temp.publish(&temp_msg);
  
}
void pressure(){
  // some processing
  float pressureValue = analogRead(pressure_pin);
  // pub the message
  pressure_msg.data = pressureValue;
  pub_pressure.publish(&pressure_msg);
  
}
void monoxide(){
  // some processingnitrogen
  float temperature;

  // pub the message
  monoxide_msg.data = temperature;
  pub_monoxide.publish(&monoxide_msg);
  
}
void nitrogen(){
  // some processing
  float temperature;

  // pub the message
  nitrogen_msg.data = temperature;
  pub_nitrogen.publish(&nitrogen_msg);
  
}
void phosphorus(){
  // some processing
  float temperature;

  // pub the message
  phosphorus_msg.data = temperature;
  pub_phosphorus.publish(&phosphorus_msg);
  
}
void potassium(){
  // some processing
  float temperature;

  // pub the message
  potassium_msg.data = temperature;
  pub_potassium.publish(&potassium_msg);
}
void ph(){
  // some processing
  float temperature;

  // pub the message
  ph_msg.data = temperature;
  pub_ph.publish(&ph_msg);
  
}
void ec(){
  // some processing
  float temperature;

  // pub the message
  ec_msg.data = temperature;
  pub_ec.publish(&ec_msg);
  
}
void humidity(){
  // some processing
  float temperature;

  // pub the message
  humidity_msg.data = temperature;
  pub_humidity.publish(&humidity_msg);
  
}
void soil_temp(){
  // some processing
  float temperature;

  // pub the message
  soil_temp_msg.data = temperature;
  pub_soil_temp.publish(&soil_temp_msg);
  
}


void setup() {
  // put your setup code here, to run once:
  
  nh.initNode();
  nh.advertise(pub_temp);
  nh.advertise(pub_uv);
  nh.advertise(pub_pressure);
  nh.advertise(pub_monoxide);
  nh.advertise(pub_nitrogen);
  nh.advertise(pub_phosphorus);
  nh.advertise(pub_potassium);
  nh.advertise(pub_ph);
  nh.advertise(pub_ec);
  nh.advertise(pub_humidity);
  nh.advertise(pub_soil_temp);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() > publisher_timer) {
    // get reading and publiish   
    uv();
    temp();
    pressure();
    nitrogen();
    phosphorus();
    potassium();
    ph();
    ec();
    humidity();
    soil_temp();

  publisher_timer = millis() + 1000; //publish once a second
  }

  nh.spinOnce();

}
