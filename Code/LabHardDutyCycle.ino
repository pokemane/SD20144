#include <PWM.h>

// pins where we will recieve the analog input that will be scaled and written to a variable
const int Ch1In = A0;
const int Ch2In = A1;
const int AnalogIn = A3;
// variables where we will store the scaled analog input value to be used in the analogWrite command
int Ch1Value = 0;
int Ch2Value = 0;

int Ch1Pin = 9;
int Ch2Pin = 10;
int LEDOut = 11;

int32_t frequency = 200;

void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 
  
  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(Ch1Pin, frequency);
  bool success1 = SetPinFrequencySafe(Ch2Pin, frequency);
  
  //if the pin frequency was set successfully, turn pins on
  if(success && success1) {
    pinMode(Ch1Pin, OUTPUT);
    pinMode(Ch2Pin, OUTPUT);
    pinMode(LEDOut, OUTPUT);
  }
}


void loop()
{
  
  pwmWrite(Ch1Pin, 204);
  pwmWrite(Ch2Pin, 51);
  if (analogRead(AnalogIn) > 512)
  {
    digitalWrite(LEDOut, HIGH); 
  }
  else
  {
    digitalWrite(LEDOut, LOW);
  }
  
}

