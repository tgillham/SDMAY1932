#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <Wire.h>

// constants////////////
// I2C
#define SCL 4
#define SDA 5

char ssid[] = "CenturyLink9713";
char pass[] = "4efda8d46777v3";

WiFiUDP Udp;
//const IPAddress destIpad(192,168,0,4); //iPad
//const IPAddress destIphone(192,168,0,3); //iPhone
const unsigned int destPort = 9000;
const unsigned int localPort = 8000;

//int preset = 0;     //preset initializes to patch/preset 
int btn_mute = 0;
OSCErrorCode error;

// variables to hold old values assigned to digipots from mixer algorithim
 byte mix0_old=0;
 byte mix1_old=0;
 byte mix01_old=0;
 byte mix2_old=0;
 byte mix3_old=0;
 byte mix23_old=0;
 byte mix4_old=0;
 byte mix5_old=0;
 byte mix45_old=0;
 byte filter0_old=0;
 byte filter1_old=0;
 byte filter01_old=0;
 byte filter2_old=0;
 byte filter3_old=0;
 byte filter23_old=0;
 byte master_old=0;
 bool MUTE = false;

 byte mix0_new=0;
 byte mix1_new=0;
 byte mix01_new=0;
 byte mix2_new=0;
 byte mix3_new=0;
 byte mix23_new=0;
 byte mix4_new=0;
 byte mix5_new=0;
 byte mix45_new=0;
 byte filter0_new=0;
 byte filter1_new=0;
 byte filter01_new=0;
 byte filter2_new=0;
 byte filter3_new=0;
 byte filter23_new=0;
 byte master_new=0;

 byte noteIn_1, wave_1, PWM_1, oct_1;
 byte noteIn_2, wave_2, PWM_2, oct_2;

void setup() {
 //I2C
 Wire.begin();
 Serial.begin(115200);
 delay(100);
//WIFI
// Specify a static IP address for NodeMCU - only needeed for receiving messages)
    // If you erase this line, your ESP8266 will get a dynamic IP address
    WiFi.config(IPAddress(192,168,0,123),IPAddress(192,168,0,1), IPAddress(255,255,255,0)); 
  
    // Connect to WiFi network255
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
  GUI_OSC_IN();
 // writeToOSC1(noteIn_1, wave_1, PWM_1, oct_1);
 // writeToOSC2(noteIn_2, wave_2, PWM_2, oct_2);
  writeToMixer(mix0_new, mix1_new, mix01_new, mix2_new, mix3_new, mix23_new, mix4_new, mix5_new, mix45_new);
 // writeToFilter(filter0_new, filter1_new, filter01_new, filter2_new, filter3_new, filter23_new);
 // writeToMasterOut(master_new, MUTE);
}

///////////////////////////////////////
void GUI_OSC_IN()
{
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.route("/TBGUI/Mixer_fader_dP_0", mix0);
      msg.route("/TBGUI/Mixer_fader_dP_1", mix1);
      msg.route("/TBGUI/Mixer_fader_dP_01", mix01);
      msg.route("/TBGUI/Mixer_fader_dP_2", mix2);
      msg.route("/TBGUI/Mixer_fader_dP_3", mix3);
      msg.route("/TBGUI/Mixer_fader_dP_23", mix23);
      msg.route("/TBGUI/Mixer_fader_dP_4", mix4);
      msg.route("/TBGUI/Mixer_fader_dP_5", mix5);
      msg.route("/TBGUI/Mixer_fader_dP_45", mix45);
      msg.route("/TBGUI/Master_fader_dP_0", master);
      msg.route("/TBGUI/MUTE", mute);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  } 
}

//////////////////////////////////////
void mix0(OSCMessage &msg, int addrOffset){
  byte mix0_new = (int) res_con_10to8(long(msg.getInt(0)));
  Serial.print(mix0_new);
}

//////////////////////////////////////
void mix1(OSCMessage &msg, int addrOffset){
  byte mix1_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix01(OSCMessage &msg, int addrOffset){
  byte mix01_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix2(OSCMessage &msg, int addrOffset){
  byte mix2_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix3(OSCMessage &msg, int addrOffset){
  byte mix3_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix23(OSCMessage &msg, int addrOffset){
  byte mix23_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix4(OSCMessage &msg, int addrOffset){
  byte mix4_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix5(OSCMessage &msg, int addrOffset){
  byte mix5_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mix45(OSCMessage &msg, int addrOffset){
  byte mix45_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void master(OSCMessage &msg, int addrOffset){
  byte master_new = (int) floor((msg.getFloat(0))/4);
}

//////////////////////////////////////
void mute(OSCMessage &msg, int addrOffset){
  bool muteIn = (bool) msg.getFloat(0);
}

////////////////////////////////////////
void writeToOSC1(byte a, byte b, byte c, byte d){
  
}

////////////////////////////////////////
void writeToOSC2(byte a, byte b, byte c, byte d){
  
}

////////////////////////////////////////
void writeToMixer(byte mix0_new, byte mix1_new, byte mix01_new, byte mix2_new, byte mix3_new, byte mix23_new, byte mix4_new, byte mix5_new, byte mix45_new){
  
  if (mix0_new != mix0_old) //For: Mixer_fader_dP_0
    {
      Wire.beginTransmission(0x28); //Address = A2 A1 A0 = 0b000
      Wire.write(0xA9);             //Write command for pot 0
      Wire.write(mix0_new);         //write new value
      Wire.endTransmission();
      mix0_old = mix0_new;          //save new value for next comparison
     // Serial.print(mix0_new);
     // Serial.print("/n");
    }
    if (mix1_new != mix1_old) //For: Mixer_fader_dP_1
    {
      Wire.beginTransmission(0x28); //Address = A2 A1 A0 = 0b000
      Wire.write(0xAA);             //Write command for pot 0
      Wire.write(mix1_new);         //write new value
      Wire.endTransmission();
      mix1_old = mix1_new;          //save new value for next comparison
    }
    
  if (mix01_new != mix01_old) //For: Mixer_fader_dP_01
    {
      Wire.beginTransmission(0x28); //Address = A2 A1 A0 = 0b000
      Wire.write(0xAF);             //Write command for pot 0
      Wire.write(mix01_new);         //write new value
      Wire.endTransmission();
      mix01_old = mix01_new;          //save new value for next comparison
    }
    if (mix2_new != mix2_old) //For: Mixer_fader_dP_2
    {
      Wire.beginTransmission(0x29); //Address = A2 A1 A0 = 0b000
      Wire.write(0xA9);             //Write command for pot 0
      Wire.write(mix2_new);         //write new value
      Wire.endTransmission();
      mix2_old = mix2_new;          //save new value for next comparison
    }
    
  if (mix3_new != mix3_old) //For: Mixer_fader_dP_3
    {
      Wire.beginTransmission(0x29); //Address = A2 A1 A0 = 0b000
      Wire.write(0xAA);             //Write command for pot 0
      Wire.write(mix3_new);         //write new value
      Wire.endTransmission();
      mix3_old = mix3_new;          //save new value for next comparison
    }
    if (mix23_new != mix23_old) //For: Mixer_fader_dP_23
    {
      Wire.beginTransmission(0x29); //Address = A2 A1 A0 = 0b000
      Wire.write(0xAF);             //Write command for pot 0
      Wire.write(mix23_new);         //write new value
      Wire.endTransmission();
      mix23_old = mix23_new;          //save new value for next comparison
    }
}

////////////////////////////////////////
void writeToFilter(byte filter0, byte filter1, byte filter2, byte filter3){
  
}

////////////////////////////////////////
void writeToMasterOut(byte masterNewdP, bool muteIn){
  if (muteIn != MUTE){
    byte MUTE = muteIn; 
  }
}
////////////////////////////////////////
// resolution converter, 10 bits to 8 bits
long res_con_10to8(long x) 
{
  long in_max = 1023;
  long in_min = 0;
  long out_max = 255;
  long out_min = 0;
  return (x - in_min)*(out_max - out_min)/(in_max - in_min)+out_min;
}
