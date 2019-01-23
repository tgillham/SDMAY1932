// Author: Francisco Alegria
// Project: SDMAY1932 Hardware Synthesizer
// Testbench for learning digital potentiometer usage.

////////////////////////////////////////////////////
// YOU DON'T NEED THE RGB LED pixels FOR THE DIGIPOT TESTING

// libraries
#include <Wire.h> //I2C
#include <FastLED.h> //LED Pixels

// constants////////////
// I2C
#define SCL A5
#define SDA A4
// trim pot wiper read into anlog pins
#define W0 A0 //controls digipot 0
#define W1 A1 //controls digipot 1 
#define W2 A3 //controls both digipots
// Fancy GRB LED pixel 
#define LED_dpin 10
#define Num_LED 21
#define Bright 15
#define LED_type WS2812B
#define Color_order GRB
CRGB leds[Num_LED];

// Variables for saving old values of the digital pots 0 and 1, 2 is for dual control
 byte val0_old=0;
 byte val1_old=0;
 byte val2_old=0;
  
void setup() {
  //I2C
 Wire.begin();
 Serial.begin(115200);
 delay(100);
 //LED pixels
 FastLED.addLeds<LED_type, LED_dpin, Color_order>(leds,Num_LED);
 FastLED.setBrightness(Bright);
//analog pins for trim pot wipers
pinMode(W0, INPUT);
pinMode(W1,INPUT);
pinMode(W2, INPUT);

 //fancy LEDs start sequence with lights to signal device ready for use 
start_seq();
}

void loop() {

  //variables for setting the digipots. Reads the analog pins and sends to "map" function to convert to the correct value range 0-253
  byte val0_new=map(analogRead(W0),0,1023,0,253);
  byte val1_new=map(analogRead(W1),0,1023,0,253);
  byte val2_new=map(analogRead(W2),0,1023,0,253);
 
  // If blocks for setting new values to digipots.
  // compares old value to new value being read from trim pots, assigns new value to digipot if new and old values are different.
  if (val2_new != val2_old )
  {
     Wire.beginTransmission(0x28); //used to start communication by talking to a specific address, look at datasheet for more commands.
  Wire.write(0xAF);                 //command for writing to both pots simultaneously
  Wire.write(val2_new);             //writes new value
  Wire.endTransmission(); 
  val2_old = val2_new;
  }
  
  if (val0_new != val0_old )  //pot 0
  {
     Wire.beginTransmission(0x28); //same address
  Wire.write(0xA9);                //command for pot 0 write
  Wire.write(val0_new);
  Wire.endTransmission(); 
  val0_old = val0_new;
  }
  
if (val1_new != val1_old)  //pot1
{
  Wire.beginTransmission(0x28); //same address
  Wire.write(0xAA);             //command for pot 1 write
  Wire.write(val1_new);
  Wire.endTransmission();
  val1_old = val1_new;
}

//ufor fancy LED pixels, just more visual feedback, one color per trim pot
fill_solid(leds,Num_LED, CRGB(val0_old*0.1,val1_old*0.1,val2_old*0.1));
FastLED.show();

// serial monitor debug feedback
  Serial.println("0: ");
  Serial.println(val0_old);
  Serial.println("\t");
  Serial.println("1: ");
  Serial.println(val1_old);
  Serial.println("\t");
  Serial.println("2: ");
  Serial.println(val2_old);
  Serial.println("\t");
  
  //delay(100);
}

///////////////////////////////////////////////////////
//function for value mapping. Courtesy of Timothy Lindquist
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//////////////////////////////
 //flash light on power up when device ready to use
void start_seq()
{
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::BlueViolet);
    FastLED.show();
    delay(1000);
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::Red);
    FastLED.show();
    delay(1000);
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::Yellow);
    FastLED.show();
    delay(100);
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::Red);
    FastLED.show();
    delay(100);
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::Yellow);
    FastLED.show();
    delay(1000);
    FastLED.clear();
    fill_solid(leds,Num_LED, CRGB::BlueViolet);
    FastLED.show();
    delay(1000);
    FastLED.clear();
    FastLED.show();
}
