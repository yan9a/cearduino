
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

//The control pins for the LCD can be assigned to any digital or analog pins.
#define LCD_CS A3 // Chip Select 
#define LCD_CD A2 // Command/Data 
#define LCD_WR A1 // LCD Write 
#define LCD_RD A0 // LCD Read
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

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
  tft.setRotation(3);
  tft.fillScreen(BLACK);
}

void loop(void) {
    tft.setCursor(20,20);
    tft.setTextColor(RED&BLUE);  
    tft.setTextSize(8);
    tft.println("16:30");

    tft.setCursor(20,100);
    tft.setTextColor(GREEN); 
    tft.setTextSize(4);
    tft.println("2016/Jul/22");

    tft.setCursor(10,150);
    tft.setTextColor(YELLOW); 
    tft.setTextSize(3);
    tft.println("ME 1378 Waso ");

    tft.setCursor(10,180);
    tft.setTextColor(YELLOW); 
    tft.setTextSize(3);
    tft.println("Waning 3");
    delay(1000);
}
