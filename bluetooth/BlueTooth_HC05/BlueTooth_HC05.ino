#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3
char recvChar;
SoftwareSerial blueToothSerial(RxD,TxD);
void setup()
{
    Serial.begin(9600);
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    blueToothSerial.begin(9600);

    pinMode(13, OUTPUT);
}
void loop()
{  
  if(blueToothSerial.available())
  {
      recvChar = blueToothSerial.read();
      Serial.print(recvChar);
      if(recvChar == '1') digitalWrite(13, HIGH);
      else if(recvChar == '0') digitalWrite(13, LOW);
  }
  if(Serial.available())
  {
      recvChar  = Serial.read();
      blueToothSerial.print(recvChar);
  }
}

