//File: Tm_Timer.h
//Author: Yan Naing Aye
#ifndef Tm_Timer_h
  #define Tm_Timer_h
  #include "Arduino.h"
  int timer1_counter;
  typedef void (*FuncPointer) ();
  FuncPointer Tm_f;
  void Tm_Setup(int fs,FuncPointer fa){ 
      Tm_f=fa;
      // initialize timer1 
      noInterrupts();           // disable all interrupts
      TCCR1A = 0;
      TCCR1B = 0;
      timer1_counter = 65536-(long)16000000/((long)256*fs);   // preload timer 65536-16MHz/256/(freq in Hz)
      TCNT1 = timer1_counter;   // preload timer
      TCCR1B |= (1 << CS12);    // 256 prescaler 
      TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
      interrupts();             // enable all interrupts      
  }
  ISR(TIMER1_OVF_vect)        // interrupt service routine 
  {
    TCNT1 = timer1_counter;   // preload timer
    Tm_f();
  }
#endif
