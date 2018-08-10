//File: CE_FGen.h
//Description: Function Generator for Arduino Uno
//WebSite: http://cool-emerald.blogspot.com
//MIT License (https://opensource.org/licenses/MIT)
//Copyright (c) 2018 Yan Naing Aye

#ifndef CE_FGEN_H
#define CE_FGEN_H
enum CE_Waveform {
  CE_HARMONIC,
  CE_SAWTOOTH,
  CE_SINUSOIDAL,
  CE_SQUARE,
  CE_TRIANGULAR
};

class CE_FGen {
private:
	float t; // time for waveform calculation
	float Ts; // sampling period
	float f; // frequency of the waveform to generate
	float vp; // peak voltage of the waveform to generate
	float va; // offset, average voltage level of the waveform to generate
	CE_Waveform wave; // waveform type to generate
public:
	void Begin(float Fs);
  void Set(CE_Waveform Wf, float Freq, float Vpp, float Vavg);//set waveform
	int Poll(); // poll for function generation task
	void SetV(float v); // set output voltage
	float Sample();
    void PrintAi();
};

void CE_FGen::Begin(float Fs) {
	pinMode(3, OUTPUT);//ping 3 as output
	TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);//hardware timer
	TCCR2B = _BV(CS21);//pwm frequency	
	Ts = 1 / Fs;	
	t = 0;
	Serial.begin(115200);
}

void CE_FGen::Set(CE_Waveform Wf, float Freq, float Vpp, float Vavg){
  wave = Wf;
  f = Freq;
  vp = Vpp/2;
  va = Vavg;
}

int CE_FGen::Poll() {
	static float t0=0; // time reference for sampling interval calculation
	float t1; // time value in us
	int r = 0;
	t1 = micros();//read the current time in micro seconds
	if ((t1 - t0) >= (Ts * 1000000)) { //check the time difference
		t0 = t1; // update the time reference
		Sample();
		r=1;
	}//end of checking the time difference
	return r;
}

float CE_FGen::Sample() {
	float vo;
	t = fmod(t + Ts, 1 / f);// update the time in seconds
	switch (wave) {
   case CE_HARMONIC:
      vo = 0.5*sin(2 * PI * f * t)-0.3*sin(6 * PI * f * t)+0.2*sin(10 * PI * f * t);
      break;
		case CE_SAWTOOTH:
			vo = 2*t*f-1;
			break;
		case CE_SINUSOIDAL:
			vo = sin(2 * PI * f * t);
			break;   
    case CE_SQUARE:
      if(t<(0.5/f)){
        vo = -1;
      }
      else {
        vo = 1;
      }      
      break;
    case CE_TRIANGULAR:
      if(t<(0.5/f)){
        vo = 4*t*f-1;
      }
      else {
        vo = 4*(1/f-t)*f-1;
      }      
      break;
    default:
      vo = sin(2 * PI * f * t);
      break;
	}
	vo = vo*vp + va; //scale and offset
	SetV(vo);
    PrintAi();
}

void CE_FGen::SetV(float v) {	
	OCR2B = floor(v * 51 + 0.5);// 255/5= 51 , change from 0 - 5 V range to 0 - 255 pwm duty cycle count
}

// Read and Plot Ain
void CE_FGen::PrintAi() {
	Serial.print(float(analogRead(0)) / 204.6); Serial.print(" ");
	Serial.print(float(analogRead(1)) / 204.6); Serial.print("\n");
}

#endif // CE_FGEN_H
