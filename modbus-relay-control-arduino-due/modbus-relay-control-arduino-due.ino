// Program: modbus relay board controller
// Author: Yan Naing Aye
// From serial port send two chars 
//   first char = o: open, c: close, t: toggle
//   second char = 1 -9, a - g from relay 1 to 16
#include "ceModbus.h"

#define RELAY_OPEN 1
#define RELAY_CLOSE 2
#define RELAY_TOGGLE 3

byte _slaveID = 1;
byte buf[128];
int _n = 0;  // whether the string is complete
byte cmd[16];
byte _action = 0;
byte _relay = 0;

//--------------------------------------------------------
//define timer
unsigned long cm=0; //current time in milliseconds
unsigned long lm=0; //store last updated time
//--------------------------------------------------------
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN,OUTPUT);
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

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    byte b = Serial.read();

    if(b >= '1' && b <= '9') {
      _relay  = b - 0x30;
    }
    else if ( b>='A' && b <= 'G') {
      _relay = b - 0x41 + 10;
    }
     else if ( b>='a' && b <= 'g') {
      _relay = b - 0x61 + 10;
    }
    else {
      _relay = 0;
    }
    
    if( _relay!=0 && _action >=1 && _action<=3) {
      ControlRelay(1,_relay,_action);
    }
    _relay = 0;
    _action = 0;
    if(b == 'o' || b == 'O') _action = 1;
    else if (b == 'c' || b == 'C') _action = 2;
    else if (b == 't' || b == 'T') _action =3;
  }
}

void ControlRelay(byte id, byte relay, byte action) {
    cmd[0] = id;
    cmd[1] = 6;
    cmd[2] = 0;
    cmd[3] = relay;
    cmd[4] = action;
    cmd[5] =0;
    int n = 6;
    n = ceFraSet((char*)cmd,n);
    byte *p=(byte*)ceFraGetTxBuf();
    for(int i=0;i<n;i++){
     Serial1.print(char(p[i]));
    }
}

void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    byte b = Serial1.read();
    OnRx(char(b));
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
    Serial.print("RX :");
    for(int i=0;i<n;i++){
     //Serial.print("0x");
     if(cmd[i]<16) Serial.print('0');
     Serial.print(cmd[i],HEX);
     Serial.print(' ');
    }
    Serial.print("\r\n");
}

