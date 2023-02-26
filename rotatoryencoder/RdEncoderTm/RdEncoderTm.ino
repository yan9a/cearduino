#include "RdEncoder.h"
#include "Tm_Timer.h"

RdEncoder En(9,10);// initialize Encoder - channel A = pin 9, channel B = pin 10

//define LED indicator
const int ledPin=13;      // the number of the LED pin
int ledState=0;             // ledState used to set the LED

//define timer
unsigned long cm=0; //current time in milliseconds
unsigned long lm=0; // will store last time LED was updated

void ledToggle() //toggle the LED indicator
{
  ledState^=1; 
  digitalWrite(ledPin, ledState);
}

void ReadSample() //tasks to perform at each sampling period
{
  En.Update(); // read encoder and update its position
}

void setup() 
{    
  pinMode(ledPin, OUTPUT);// initialize the LED as an output
  Serial.begin(9600);// initialize serial communication
  Tm_Setup(1000,ReadSample);// initialize timer to read encoder at 1kHz 
  //(Hz,Func) = sampling frequency, function to call to get sample
  // max speed for 1kHz = 1000/360 = 2.8 rps = 167 rpm 
}

void loop()
{
  //En.Update();//constantly update encoder position
  cm=millis(); //get current time
  if(cm-lm >= 1000) {//if timeout
    lm=cm;   //store the updated time
    ledToggle(); // toggle LED
    Serial.println(En.Get()); //write to serial port
  }        
}



