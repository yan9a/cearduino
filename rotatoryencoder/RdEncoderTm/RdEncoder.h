//File: RdEncoder.h
//Author: Yan Naing Aye
#ifndef RdEncoder_h
  #define RdEncoder_h
  #include "Arduino.h"
  int En_TruthTable[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; //encoder truth table
  class RdEncoder {
    private:
      int pinA, pinB; //encoder inputs
      int PS=0,NS=0; //present state and next state
      int c=0; //current encoder position count (angular position in degree in this case)
      const int SPR=360; // number of states per revolution of the encoder 4*90
      
    public:
     RdEncoder(int A, int B){Setup(A,B);};
     void Setup(int A, int B){pinA=A; pinB=B; pinMode(pinA,INPUT); pinMode(pinB,INPUT);};
     int Get(){return c;};
     void Update(){
      NS = (digitalRead(pinA)<<1) | digitalRead(pinB);
      if(NS!=PS){ c=(c+En_TruthTable[(NS<<2)|PS]+SPR)%SPR; PS = NS;}
     };
     void Reset(int r) {c=0;};
  };
#endif

