// Program: modbus relay board emulator
// Author: Yan Naing Aye
#include "ceModbus.h"

#define EMU_OUT1 5
#define EMU_OUT2 4
#define EMU_OUT3 3
#define EMU_OUT4 2

#define EMU_LED1 9
#define EMU_LED2 8
#define EMU_LED3 7
#define EMU_LED4 6

#define EMU_IN1 A0
#define EMU_IN2 A1
#define EMU_IN3 A2
#define EMU_IN4 A3

byte _slaveID = 1;
byte buf[128];
int _n = 0;  // whether the string is complete
byte out[4] = {1,1,1,1};
byte currentOut = 0;
byte currentIn = 0;
byte currentPin = EMU_OUT1;
byte currentVal = 1;

int EMUIn[4] = {A0,A1,A2,A3};
//--------------------------------------------------------
//define timer
unsigned long cm=0; //current time in milliseconds
unsigned long lm=0; //store last updated time
//--------------------------------------------------------
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN,OUTPUT);
  
  pinMode(EMU_OUT1,OUTPUT);
  pinMode(EMU_OUT2,OUTPUT);
  pinMode(EMU_OUT3,OUTPUT);
  pinMode(EMU_OUT4,OUTPUT);

  pinMode(EMU_LED1,OUTPUT);
  pinMode(EMU_LED2,OUTPUT);
  pinMode(EMU_LED3,OUTPUT);
  pinMode(EMU_LED4,OUTPUT);
  
  digitalWrite(EMU_OUT1, HIGH);
  digitalWrite(EMU_OUT2, HIGH);
  digitalWrite(EMU_OUT3, HIGH);
  digitalWrite(EMU_OUT4, HIGH);

  digitalWrite(EMU_LED1, HIGH);
  digitalWrite(EMU_LED2, HIGH);
  digitalWrite(EMU_LED3, HIGH);
  digitalWrite(EMU_LED4, HIGH);
  
  pinMode(EMU_IN1,INPUT);
  pinMode(EMU_IN2,INPUT);
  pinMode(EMU_IN3,INPUT);
  pinMode(EMU_IN4,INPUT);

  ceFrame();
  
}

// the loop function runs over and over again forever
void loop() {
  //----------------------------------------------------
  //Tmr task
  cm=millis(); //get current time
  if((cm-lm)>=(10)) {//if timeout
    lm=cm;   //store the updated time
    //-------------------------------
    //Tasks at each sampling instant
    Tmr10ms();
    //-------------------------------
  }        
  //----------------------------------------------------
  // check rx
  if (_n) {
    if(_n==8) RxCmd(buf,_n);
    _n = 0;
  }
  //----------------------------------------------------
  digitalWrite(EMU_LED1, digitalRead(EMU_IN1));
  digitalWrite(EMU_LED2, digitalRead(EMU_IN2));
  digitalWrite(EMU_LED3, digitalRead(EMU_IN3));
  digitalWrite(EMU_LED4, digitalRead(EMU_IN4));
  //----------------------------------------------------
  
}

void Tmr10ms(){
  static int mcount = 0;
  static int lvl = 0;
  if(mcount++ > 50) {
    mcount = 0; 
    lvl ^= 1;
    digitalWrite(LED_BUILTIN, lvl);
  }
  Tick();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    byte b = Serial.read();
    OnRx(char(b));
    //buf[0] = b;
  }
}

void OnRx(char ch) {
  int rxn=0;
  char *p;
  rxn = ceFraProcess(ch);
  if (rxn > 0) {
    p=ceFraGetRxBuf();
    for(int i=0;i<rxn;i++){
      buf[i] = p[i];
    }
    _n = rxn;
  }
}

void RxCmd(byte* cmd,int n) {
  if(_slaveID==cmd[0]){ // check slave id
    //Serial.print("RX\n");
    if(cmd[1]==6){ // control func
      CtrlCmd(cmd,n);
    }
    else if(cmd[1]==3){ // read func
      ReadCmd(cmd,n);
    }
  }
}

void CtrlCmd(byte* cmd,int n) {
  if(cmd[3]>=1 && cmd[3]<=4){// only for relay 1 - 4
    currentOut = cmd[3] - 1;
    if(cmd[4]==1){
      out[currentOut] = 1;
    }
    else if(cmd[4]==2){
      out[currentOut] = 0;
    }
    else if(cmd[4]==3){
      out[currentOut] ^= 1;
    }
    currentPin = 5 - currentOut;
    digitalWrite(currentPin, out[currentOut]);
    for(int i=0;i<n;i++){
      Serial.print(char(cmd[i]));
    }
  } 
}

void ReadCmd(byte* cmd,int n) {
  if(cmd[3]>=1 && cmd[3]<=4){// only for relay 1 - 4
  // implement to read only one relay (for simplicity)
    currentIn = cmd[3] - 1;
    currentPin = EMUIn[currentIn];
    currentVal = digitalRead(currentPin);
    cmd[2] = 2;
    cmd[3] = 0;
    cmd[4] = currentVal;
    n = 5;
    n = ceFraSet((char*)cmd,n);
    byte *p=ceFraGetTxBuf();
//    byte *p=cmd;
    for(int i=0;i<n;i++){
     Serial.print(char(p[i]));
    }
  }
}
