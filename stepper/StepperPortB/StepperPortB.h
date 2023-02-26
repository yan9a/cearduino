//File: StepperPortB.h
//Author: Yan Naing Aye
//Website: http://www.cool-emerald.com/

//Driving 28BYJ-48 – 5V Stepper Motor using ULN2003A Transistor Arrays
//----------------------------------------------------------
#ifndef StepperPortB_h
  #define StepperPortB_h
  #include "Arduino.h"
byte* S; //Stepping sequence

//Wave drive
//Step          0 | 1 | 2 | 3 | 0 |...

//Blue   = A :  1 | 0 | 0 | 0 | 1 | ...
//Pink   = B :  0 | 1 | 0 | 0 | 0 | ...
//Yellow = C :  0 | 0 | 1 | 0 | 0 | ...
//Orange = D :  0 | 0 | 0 | 1 | 0 | ...
byte Wave[4] = {0x01,0x02,0x04,0x08};

//Full Step drive
//Step          0 | 1 | 2 | 3 | 0 |...

//Blue   = A :  1 | 1 | 0 | 0 | 1 | ...
//Pink   = B :  0 | 1 | 1 | 0 | 0 | ...
//Yellow = C :  0 | 0 | 1 | 1 | 0 | ...
//Orange = D :  1 | 0 | 0 | 1 | 1 | ...  
byte FullStep[4] = {0x09,0x03,0x06,0x0C};

//Half Step drive
//Step          0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 0 |...

//Blue   = A :  1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 |...
//Pink   = B :  0 | 0 | 1 | 1 | 1 | 0 | 0 | 0 | 0 |...
//Yellow = C :  0 | 0 | 0 | 0 | 1 | 1 | 1 | 0 | 0 |...
//Orange = D :  1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 1 |...
byte HalfStep[8] = {0x09,0x01,0x03,0x02,0x06,0x04,0x0C,0x08};
int CS=0;//Current step
int N;
//----------------------------------------------------------
//Mode definition
#define HALF 0
#define FULL 1
#define WAVE 2
//Define the mode to drive the stepper motor
void StepperMode(int Mode) {  
  if(Mode==WAVE) {S=Wave; N=4;}
  else if(Mode==FULL) {S=FullStep; N=4;}
  else {S=HalfStep; N=8;}
}
//----------------------------------------------------------
//Turn stepper motor n steps
//with t microseconds period for each step
//positive n for forward dir and negative n for backward dir
void Step(int n,int t) {
  int CD=1; 
  if(n<0) {n*=-1; CD=-1;}
  for(int i=0;i<n;i++) {
    CS=(CS+(N+CD))%N;
    PORTB=S[CS];
    delayMicroseconds(t);
  }
}
//----------------------------------------------------------
//Initialize
void StepperInit() {
  //Setup port B (digital pin 8 to 13)
  //The two high bits (6 & 7) map to the crystal pins and are not usable
  //only 4 pins - 8 to 11 are used in this program
  //where Blue - pin 8 (LSB), Pink -pin 9, Yellow -pin 10, Orange -pin 11
  DDRB=0xff;
  PORTB = 0x00;  
}
//----------------------------------------------------------
#endif
