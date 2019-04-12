#include <Wire.h>

//Can Change Values If We Want
float Max_Time_Attack = 1;         //(ms.)
float Max_Time_Decay = 1;          //(ms.)
float Max_Time_Release = 1;        //(ms.)
float Tap_Positions = 256;

////START OF STUFF THAT WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING////
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
////END OF STUFF THAT WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING////

unsigned long timer = 0;     //Use this for variable for millis(); INSTEAD of delay();

void setup() {

  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  
  ////START OF STUFF THAT WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING////
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  ////END OF STUFF THAT WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING////

}


///////////////////////////////////////FUNCTIONS////////////////////////////////////////

//VOLUME FUNCTION
//Tap_Position is a the value of slider variable  (0-40)

void Volume(unsigned int Tap_Position_Val) {

  //==========================================================================
  Wire.beginTransmission(0x28); // transmit to device #40
  // device address is specified in datasheet
  Wire.write(byte(0xa9)); // sends instruction byte
  Wire.write(Tap_Position_Val); // sends potentiometer value byte
  Wire.endTransmission(); // stop transmitting
  //==========================================================================
}

//ADSR FUNCTION
//Attack is a the value of slider variable      (0-255)
//Decay is a the value of slider variable       (0-255)
//Sustain is a the value of slider variable     (0-255)
//Release is a the value of slider variable     (0-255)

void ADSR(int Attack, int Decay, int Sustain, int Release, int MIDI_Note) {

  //Tells What Note Is/Has Been Pressed ----> FROM GUI
  int MIDI_Note1 = MIDI_Note;  // --->STARTING GUI VALUE
  int MIDI_Note_GUI = 0; // --->GUI VALUE UPDATED   //0 WILL BE REPLACED BY GUI VALUE

  //==========================================================================
  Wire.beginTransmission(0x28); // transmit to device #40
  // device address is specified in datasheet
  Wire.write(byte(0xaa)); // sends instruction byte
  Wire.write(abs(0)); // sends potentiometer value byte
  Wire.endTransmission(); // stop transmitting
  //==========================================================================

  //Position is a the Tap Position of DIGI-POT  (0-255)
  int Position = 0;

  //Value is a Random Variable for Counting
  int Value = 0;

  //Note Tells if a Key Has Been Pressed      (0-1)
  int Note;

  //Periods for Attack, Decay, and Release
  float Period_Attack = (Attack * (Max_Time_Attack / Tap_Positions)) / Tap_Positions;
  float Period_Decay = (Max_Time_Decay) / (Tap_Positions - Sustain);
  float Period_Release = (Max_Time_Release) / (Sustain);

  // Attack Section
  for (Position = 0; Position < Tap_Positions - 1; Position ++ ) {
    timer = millis();

    if (Attack == 0) {
      Position = Tap_Positions - 1;
    }

    //if timer > Period_Attack{
    while (millis() < timer + Period_Attack) {
      //Serial.println(timer);
    }

    //==========================================================================
    Wire.beginTransmission(0x28); // transmit to device #40
    // device address is specified in datasheet
    Wire.write(byte(0xaa)); // sends instruction byte
    Wire.write(abs(Position)); // sends potentiometer value byte
    Wire.endTransmission(); // stop transmitting
    //==========================================================================
    
    //If Note That Was Sent Is Not Same As Note Now Being Shown Start It All Over Again
    if (MIDI_Note1 != MIDI_Note_GUI) {        
      ADSR(Attack, Decay, Sustain, Release, MIDI_Note_GUI);
    }

    Serial.println(abs(Position), DEC);       //Prints Tap Position

  }

  // Decay Section
  for (Position = Tap_Positions - 1; Position >= Sustain + 1; Position --) {
    if (Decay == 0) {
      Position = Sustain + 1;
    }
    timer = millis();

    while (millis() < timer + Period_Decay) {

    }

    //==========================================================================
    Wire.beginTransmission(0x28); // transmit to device #40
    // device address is specified in datasheet
    Wire.write(byte(0xaa)); // sends instruction byte
    Wire.write(abs(Position)); // sends potentiometer value byte
    Wire.endTransmission(); // stop transmitting
    //==========================================================================
    
    //If Note That Was Sent Is Not Same As Note Now Being Shown Start It All Over Again
    if (MIDI_Note1 != MIDI_Note_GUI) {
      ADSR(Attack, Decay, Sustain, Release, MIDI_Note_GUI);
    }

    Serial.println(abs(Position), DEC);       //Prints Tap Position

  }

  // Sustain Section
  for (Value = 0; Value < 2; 1) {

    //Read the State of the Pushbutton Value: --> WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING
    buttonState = digitalRead(buttonPin);

    //Is Button Pressed?
    if (buttonState == HIGH) {
      Note = 1;       //YES
      Value = 0;
      Serial.println("Hi-------------------------");        //Prints Tap Position


    } else {
      Note = 0;       //NO
      Value = 3;
    }

    //If Note That Was Sent Is Not Same As Note Now Being Shown Start It All Over Again
    if (MIDI_Note1 != MIDI_Note_GUI) {
      ADSR(Attack, Decay, Sustain, Release, MIDI_Note_GUI);
    }

    Serial.println(abs(Position), DEC);       //Prints Tap Position

  }

  // Release Section
  for (Position = Sustain; Position >= 0; Position --) {


    if (Release == 0) {
      Position = 0;
    }

    timer = millis();

    while (millis() < timer + Period_Release) {

    }

    //==========================================================================
    Wire.beginTransmission(0x28); // transmit to device #40
    // device address is specified in datasheet
    Wire.write(byte(0xaa)); // sends instruction byte
    Wire.write(abs(Position)); // sends potentiometer value byte
    Wire.endTransmission(); // stop transmitting
    //==========================================================================

    //If Note That Was Sent Is Not Same As Note Now Being Shown Start It All Over Again
    if (MIDI_Note1 != MIDI_Note_GUI) {
      ADSR(Attack, Decay, Sustain, Release, MIDI_Note_GUI);
    }

    Serial.println(abs(Position), DEC);       //Prints Tap Position
  }
}

//VOLUME dB FUNCTION

unsigned int dB_to_tap(double dB) {
  // put your main code here, to run repeatedly:
  unsigned int total_taps = 256;
  unsigned int closest_tap;
  double raw_value;

  if (dB >= 0)
  {
    closest_tap = 1;
  }
  else
  {
    raw_value = total_taps - (pow(10, ((dB / 20))) * (total_taps - 1));
    closest_tap = (unsigned int) (raw_value + 0.5);
  }
  return closest_tap;
}


////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////MAIN//////////////////////////////////////////

void loop() {

  //Volume_Value is a Place Holder Value to Compare to the Volume Slider (-40-0) 0 being the loudest
  int Volume_Value = 0;

  //Volume_Level is a the value of slider variable  (0-255)
  int Volume_Level; // "SLIDER VALUE";

  //Tells What Note is/has Been Pressed ----> FROM GUI
  int MIDI_Note = 0;

  //Attack is a the value of slider variable      (0-255)
  float Attack;  // "SLIDER VALUE";

  //Decay is a the value of slider variable     (0-255)
  float Decay;   // "SLIDER VALUE";

  //Sustain is a the value of slider variable     (0-255)
  float Sustain; // "SLIDER VALUE";

  //Release is a the value of slider variable     (0-255)
  float Release; // "SLIDER VALUE";

  //Note Tells if a Key Has Been Pressed        (0-1)
  int Note = 0;  // "Value that Will Go High When Key Is Pressed";

  //Read the State of the Pushbutton Value: --> WILL BE DELETED WHEN WE GET TABLET INTERFACE GOING
  buttonState = digitalRead(buttonPin);

  //VALUES FROM SLIDERS --> WILL BE REPLACED BY THOSE VALUES WHEN WE GET TABLET INTERFACE GOING
  Attack = 1;
  Decay = 1;
  Sustain = 150;
  Release = 1;

  //Is Button Pressed?
  if (buttonState == HIGH) {
    Note = 1;         //YES
  } else {
    Note = 0;         //NO
  }
  
  Volume_Level = 1;
  
  //If Volume Slider Is Changed
  if (Volume_Value != Volume_Level)
  {
    unsigned int a = dB_to_tap(Volume_Level);
    Volume(a);
    Volume_Value = Volume_Level;      //NOW SET IT EQUAL TO VOLUME
  }

  //Key is/was Pressed
  if (Note == 1)
  {
    ADSR(Attack, Decay, Sustain, Release, MIDI_Note);
    Note = 0;
  }

}

////////////////////////////////////////////////////////////////////////////////////////

