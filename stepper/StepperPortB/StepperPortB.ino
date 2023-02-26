
//Driving 28BYJ-48 – 5V Stepper Motor using ULN2003A Transistor Arrays
//Author: Yan Naing Aye
//Website: http://www.cool-emerald.com/

#include "StepperPortB.h"
//----------------------------------------------------------
void setup() {    
  StepperInit();//setup pins
  //StepperMode(WAVE);
  //StepperMode(FULL);
  StepperMode(HALF);
  //Serial.begin(9600);
}
void loop() {
  Step(4096,2500); //turn 4096 steps forward with 2500 us period for each step
  Step(-4096,2500);//turn 4096 steps backward with 2500 us period for each step
  //Serial.println(CS);
}
