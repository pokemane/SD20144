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
boolean PrevFanState = true;
int32_t TachTimestamp = 0;
int32_t TachTimeout = 100;

boolean FirstScan = true;

int averager[5];
int average;




//remove this later
int Ch3Value = 0;
int Ch3In = A2;





void setup()
{
  //digitalWrite(Ch1Pin, LOW);
  //digitalWrite(Ch2Pin, LOW);
  
  //Serial.begin(9600);
  
  pinMode(TachIn, INPUT_PULLUP);
  
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
  }
  
  //wait 500ms then ramp
  delay(1000);
  ramp();
  
  //Setup the fan's hall effect input
  //digitalWrite(TachIn, HIGH);
  //attachInterrupt(0, rpmInt, RISING);  //attaches an interrupt to the falling edge on pin 2
  //TachTimestamp = millis();
}

void ramp()
{
  //ramp the LEDs from 0% to 100% for 500ms
  int i = 0;
  while (i < 100)
  {
    
    readChannelInputs();
    
    //pwmWrite(Ch1Pin, map(Ch3Value, 0, 1024, 0, Ch1Value));
    //pwmWrite(Ch2Pin, map(Ch3Value, 0, 1024, 0, Ch2Value));
    pwmWrite(Ch1Pin, map(i, 0, 100, 0, map(Ch3Value, 0, 1024, 0, Ch1Value)));
    pwmWrite(Ch2Pin, map(i, 0, 100, 0, map(Ch3Value, 0, 1024, 0, Ch2Value)));
    
    delay(5);
    i = i + 1;
  } 
}

void readChannelInputs()
{
  Ch1Value = analogRead(Ch1In)/4;
  Ch2Value = analogRead(Ch2In)/4;
  Ch3Value = analogRead(Ch3In)/4;
}

void writePWM()
{
  //pwmWrite(Ch1Pin, Ch1Value);
  //pwmWrite(Ch2Pin, Ch2Value);
  
  pwmWrite(Ch1Pin, map(Ch3Value, 0, 1024, 0, Ch1Value));
  pwmWrite(Ch2Pin, map(Ch3Value, 0, 1024, 0, Ch2Value));
}

void loop()
{
  if (millis()%100 <= 5)
  {
    averager[5] = averager[4];
    averager[4] = averager[3];
    averager[3] = averager[2];
    averager[2] = averager[1];
    averager[1] = averager[0];
    averager[0] = analogRead(A4);
    
    average = (averager[0] + averager[1] + averager[2] + averager[3] + averager[4] + averager[5])/6;
  }
  
  //determine if the fan is running by using a timeout of 100ms
  if (average > 25) //check if the currect time is past our timeout
  {
    FanRunning = false;
    PrevFanState = false;
    digitalWrite(DebugLED, LOW);
  }
  else
  {
    FanRunning = true;
    if (!PrevFanState)
    {
      delay(500);
      
      ramp();
    }
    PrevFanState = true;
  }
  
  //Serial.print(analogRead(A4));
  //Serial.print("\n");
  
  if (FanRunning)
  {
    //reads the voltage from the channel 1 and 2 analog inputs, converts to a 0-255 scale, and stores to a variable
    readChannelInputs();
    
    //use the variable that we saved above to set the pwm duty cycle (0 is 0%, 255 is 100%)
    writePWM();
  }
  else
  {
    //turn off the pwm pins
    pwmWrite(Ch1Pin, 0);
    pwmWrite(Ch2Pin, 0);
    digitalWrite(Ch1Pin, LOW);
    digitalWrite(Ch2Pin, LOW);
  }
  
}
/*
void rpmInt()
{
  //log the tach input time
  if (millis() > (TachTimestamp + 2) ) //if the sensor has been triggered faster than 2ms later, it is a bounce
  {
    //log the tach input time
    TachTimestamp = millis();
    digitalWrite(DebugLED,HIGH);
  }
  
}
*/
