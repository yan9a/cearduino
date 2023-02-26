#include <Wire.h>
int y,m,d,wd,h,n,s;//year, month, day, week day, hour, minute, second
char* wds[]={"SAT","SUN","MON","TUE","WED","THU","FRI"};
void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output

  if(0){ //To change settings
    Wire.beginTransmission(0x68); // transmit to device #104
    Wire.write(0x00);//Set address to 0
    Wire.endTransmission();// stop transmitting
    
    Wire.requestFrom(0x68, 7);    // request 7 bytes from slave device #104
    s = Wire.read();//second
    n = Wire.read();//minute
    h = Wire.read();//hour
    wd= Wire.read();//week day
    d = Wire.read();//day
    m = Wire.read();//month
    y = Wire.read();//year

    if(0){ //To set date/time
      s=0x00;
      n=0x26;
      h=0x12;
      wd=0x03;
      d=0x17;
      m=0x05;
      y=0x16;
    }
    
    Wire.beginTransmission(0x68); // transmit to device #104
    Wire.write(0x00);//Set address to 0
    Wire.write(s & 0x7F);//Enable oscillator
    Wire.write(n);//minute
    Wire.write(h & 0x3F);//hour to 25hr mode
    Wire.write(wd);//weekday
    Wire.write(d);//day
    Wire.write(m);//month
    Wire.write(y);//year
    Wire.endTransmission();// stop transmitting
  }
}

void loop()
{
  delay(1000);
  
  Wire.beginTransmission(0x68); // transmit to device #104
  Wire.write(0x00);//Set address to 0
  Wire.endTransmission();// stop transmitting
  
  Wire.requestFrom(0x68, 7);    // request 7 bytes from slave device #104
  s = Wire.read();//second
  n = Wire.read();//minute
  h = Wire.read();//hour
  wd= Wire.read();//week day
  d = Wire.read();//day
  m = Wire.read();//month
  y = Wire.read();//year

  Serial.print(0x20,HEX);
  Serial.print(y,HEX);
  Serial.print("/");
  Serial.print(m,HEX);
  Serial.print("/");
  Serial.print(d,HEX);
  Serial.print(" ");
  Serial.print(h,HEX);
  Serial.print(":");
  Serial.print(n,HEX);
  Serial.print(":");
  Serial.print(s,HEX);
  Serial.print(" ");
  Serial.print(wds[wd%7]);
  Serial.print("\n");
  Serial.print("\n");
}
