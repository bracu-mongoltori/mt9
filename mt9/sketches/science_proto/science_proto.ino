// gas sensor, UV sensor, barometric sensor, multiple gas sensor

#include <ros.h>
#include <std_msgs/Float32.h>
#include <AltSoftSerial.h>
#include <Wire.h>

#define RE 29
#define DE 30

int mq7 = A0;
int UV_OUT = A1;
int UV_EN = A2;
int pressure_pin = 3;
int BH1750address = 0x23;
byte buff[2];

const byte temp_[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xcf};
const byte mois_[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F};
const byte econ[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};
const byte ph_[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};
const byte nitro[] = {0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte phos[] = {0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};

byte values[7];
AltSoftSerial mod;

// atmospheric quantities
std_msgs::Float32 temp_msg;
std_msgs::Float32 uv_msg;
std_msgs::Float32 pressure_msg;
std_msgs::Float32 monoxide_msg;
std_msgs::Float32 mois_msg;
std_msgs::Float32 light_msg;

// soil quantities
std_msgs::Float32 nitrogen_msg;
std_msgs::Float32 phosphorus_msg;
std_msgs::Float32 potassium_msg;
std_msgs::Float32 ph_msg;
std_msgs::Float32 ec_msg;
std_msgs::Float32 soil_temp_msg;
std_msgs::Float32 soil_mois_msg;

// ros science nodes
// atmospheric 
ros::Publisher pub_temp("temperature", &temp_msg);
ros::Publisher pub_uv("uv", &uv_msg);
ros::Publisher pub_pressure("pressure", &pressure_msg);
ros::Publisher pub_monoxide("monoxide", &monoxide_msg);
ros::Publisher pub_mois("moisture", &mois_msg);
ros::Publisher pub_light("light_intensity", &light_msg);

// soil
ros::Publisher pub_nitrogen("nitrogen", &nitrogen_msg);
ros::Publisher pub_phosphorus("phosphorus", &phosphorus_msg);
ros::Publisher pub_potassium("potassium", &potassium_msg);
ros::Publisher pub_ph("ph", &ph_msg);
ros::Publisher pub_ec("ec", &ec_msg);
ros::Publisher pub_soil_temp("soil_tempurature", &soil_temp_msg);
ros::Publisher pub_soil_mois("soil_moisture", &soil_mois_msg);

ros::NodeHandle nh;

long publisher_timer;

int BH1750_Read(int address)
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();  
  return i;
}

void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

float readSensor(const byte* command) {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < 8; i++) {
    mod.write(command[i]);
  }
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
  }
  return combineBytes();
}

float combineBytes() {
  int16_t combinedValue = (values[3] << 8) | values[4];
  return combinedValue;
}

float mapToRange(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void uv(){
  // some processing
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
  float sensorvalue = analogRead(A0) *1000.0/ 1023;

  // pub the message
  monoxide_msg.data = sensorvalue;
  pub_monoxide.publish(&monoxide_msg);
  
}

void mois(){
  // some processing
  float mois;

  // pub the message
  mois_msg.data = mois;
  pub_mois.publish(&mois_msg);
  
}

void light(){
  // some processing
  uint16_t value = 0;
  BH1750_Init(BH1750address);
  if (2 == BH1750_Read(BH1750address))
    {
      value = ((buff[0] << 8) | buff[1]) / 1.2;
      // pub the message
      light_msg.data = value;
      pub_light.publish(&light_msg);
    
    }
}
void nitrogen(){
  // some processing
  float soil_nitro = readSensor(nitro);

  // pub the message
  nitrogen_msg.data = soil_nitro;
  pub_nitrogen.publish(&nitrogen_msg);
  
}
void phosphorus(){
  // some processing
  float soil_phos = readSensor(phos);

  // pub the message
  phosphorus_msg.data = soil_phos;
  pub_phosphorus.publish(&phosphorus_msg);
  
}
void potassium(){
  // some processing
  float soil_pota = readSensor(pota);

  // pub the message
  potassium_msg.data = soil_pota;
  pub_potassium.publish(&potassium_msg);
}
void ph(){
  // some processing
  float soil_ph = readSensor(ph_);
  soil_ph = mapToRange(soil_ph, 0, 1023, 2, 14); 

  // pub the message
  ph_msg.data = soil_ph;
  pub_ph.publish(&ph_msg);
  
}
void ec(){
  // some processing
  float soil_ec = readSensor(econ);

  // pub the message
  ec_msg.data = soil_ec;
  pub_ec.publish(&ec_msg);
  
}

void soil_temp(){
  // some processing
  float soil_temp = readSensor(temp_) / 10.0;

  // pub the message
  soil_temp_msg.data = soil_temp;
  pub_soil_temp.publish(&soil_temp_msg);
  
}
void soil_mois(){
  // some processing
  float soil_mois_hex = readSensor(mois_); 
  float soil_mois = soil_mois_hex / 1.8; 
  soil_mois = mapToRange(soil_mois, 0, 1023, 0, 100); 

  // pub the message
  soil_mois_msg.data = soil_mois;
  pub_soil_mois.publish(&soil_mois_msg);
  
}


void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

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
  nh.advertise(pub_soil_temp);
  nh.advertise(pub_mois);
  nh.advertise(pub_soil_mois);
  nh.advertise(pub_light);

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
    soil_temp();
    mois();
    soil_mois();
    light();

  publisher_timer = millis() + 1000; //publish once a second
  }

  nh.spinOnce();

}
