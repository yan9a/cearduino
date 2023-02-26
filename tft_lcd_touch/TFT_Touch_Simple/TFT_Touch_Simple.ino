#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#define XP 7  
#define XM A1 
#define YM 6   
#define YP A2   

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define PENRADIUS 3

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0xFFFF
#define BLUE    0xFFE0
#define RED     0x07FF
#define GREEN   0xF81F
#define CYAN    0xF800
#define MAGENTA 0x07E0
#define YELLOW  0x001F
#define WHITE   0x0000

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
void setup(void) {
  tft.reset();
  tft.begin(0x9341);
  tft.fillScreen(BLACK);

  tft.setCursor(20,20);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.println("Touch me!");
  
  pinMode(13, OUTPUT);
}


void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    
    tft.fillCircle(p.x, p.y, PENRADIUS, RED);
  }
}

