// Modified for SPFD5408 Library by Joao Lopes

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library
//#include "TouchScreen.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMonoBoldOblique9pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/Org_01.h"
#include "Fonts/FreeSansBold12pt7b.h"
// Pin assignments for the TFT touch screen
#define YP    A1
#define XM    A2
#define YM    7
#define XP    6

// Calibrated values for the TFT touch screen  
//#define TS_MINX   178
//#define TS_MINY   75
//#define TS_MAXX   931
//#define TS_MAXY   895

// Instantiate the ts object
//TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_RD    A0
#define LCD_WR    A1
#define LCD_CD    A2
#define LCD_CS    A3
#define LCD_RESET A4

// Instantiate the tft object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void) {
  Serial.begin(9600);
  //Serial.println(F("Paint!"));
  
  tft.reset();
  tft.begin(0x9341);      // SPFD5408 compatible
  tft.setRotation(3);     // This settings works for me with correct orientation
                          // tft.setRotation(2);
  tft.fillScreen(BLACK);  //Set Background Color with BLACK
  tft.setTextColor(YELLOW);
  tft.setTextSize(1.5);
  tft.setCursor(0, 10);
  tft.setFont(&FreeMono9pt7b);
  tft.println("FreeMono9pt7b");
  
  tft.setFont(&FreeMonoBoldOblique9pt7b);
  tft.println("FreeMonoBoldOblique9pt7b");
  
  tft.setFont(&FreeSans9pt7b);
  tft.println("FreeSans9pt7b");
  
  tft.setFont(&FreeSerif9pt7b);
  tft.println("FreeSerif9pt7b");
  
  tft.setFont(&Org_01);
  tft.println("Org_01");

  tft.setFont(&FreeSansBold12pt7b);
  tft.println("FreeSansBold12pt7b");

}

void loop() {}
