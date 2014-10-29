#include <PWM.h>

// input pins for channels strength pots (analog)
int Ch1In = A0;
int Ch2In = A1;
// output pins for LED channels (PWM)
int Ch1Pin = 9;
int Ch2Pin = 10;
// intermediate var's for scaling pots to PWM
int Ch1Value = 0;
int Ch2Value = 0;
// LED PWM Frequency
int32_t frequency = 200;

// fan tach input pin
int TachIn = 2;

// Onboard Debug LED Pin
int DebugLED = 13;

// Fan saftey check
boolean FanRunning = false;
int32_t TachTimestamp = 0;
int32_t TachTimeout = 100;

void setup()
{
  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 
  
  //sets the frequency for the specified pin
  boolean success = SetPinFrequencySafe(Ch1Pin, frequency);
  boolean success1 = SetPinFrequencySafe(Ch2Pin, frequency);
  
  //if the pin frequency was set successfully, turn pins on
  if(success && success1)
  {
    pinMode(Ch1Pin, OUTPUT);
    pinMode(Ch2Pin, OUTPUT);
    pinMode(DebugLED, OUTPUT);
    pinMode(TachIn, INPUT_PULLUP);
  }
  
  //wait 500ms
  delay(500);
  
  //ramp the LEDs from 0% to 100% for 500ms
  for (int i=0; i<100; i++)
  {
    Ch1Value = (i/100)*analogRead(Ch1In)/4;
    Ch2Value = (i/100)*analogRead(Ch2In)/4;
    
    pwmWrite(Ch1Pin, Ch1Value);
    pwmWrite(Ch2Pin, Ch2Value);
    
    delay(5);
  }
  
  //Setup the fan's hall effect input
  attachInterrupt(0, rpmInt, RISING);  //attaches an intrupt to the rising edge on pin 2
  TachTimestamp = millis();
}


void loop()
{
  
  //determine if the fan is running by using a timeout of 100ms
  if (millis()> (TachTimestamp+100) ) //check if the currect time is past our timeout
  {
    FanRunning = false;
    digitalWrite(DebugLED, LOW);
  }
  else
  {
    FanRunning = true;
    digitalWrite(DebugLED, HIGH);
  }
  
  if (FanRunning)
  {
    //reads the voltage from the channel 1 and 2 analog inputs, converts to a 0-255 scale, and stores to a variable
    Ch1Value = analogRead(Ch1In)/4;
    Ch2Value = analogRead(Ch2In)/4;
    
    //use the variable that we saved above to set the pwm duty cycle (0 is 0%, 255 is 100%)
    pwmWrite(Ch1Pin, Ch1Value);
    pwmWrite(Ch2Pin, Ch2Value);
  }
  
}

void rpmInt()
{
  //log the tach input time
  if (millis() > (TachTimestamp+9) ) //if the sensor has been triggered faster than 9ms later, it is a bounce
  {
    //log the tach input time
    TachTimestamp = millis();
    digitalWrite(DebugLED,HIGH);
  }
}
