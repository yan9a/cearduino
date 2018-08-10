#include "CE_FGen.h"

CE_FGen fg;

void setup() {  
  fg.Begin(500); // ( sampling_freq )
  fg.Set(CE_SINUSOIDAL,5,1,1); // ( waveform , wave_freq , v_peak2peak , v_average )
}

int i;
void loop() {
  
  i+=fg.Poll(); //increase i each sample
  i%=25000; //to change to other waveforms
  
  if(i==0) fg.Set(CE_SINUSOIDAL,5,1,1);
  else if(i==5000) fg.Set(CE_SAWTOOTH,5,1,1);
  else if(i==10000) fg.Set(CE_SQUARE,5,1,1);
  else if(i==15000) fg.Set(CE_TRIANGULAR,5,1,1);
  else if(i==20000) fg.Set(CE_HARMONIC,5,1,1);
  
}
