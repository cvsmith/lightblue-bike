/*********************************************************************
This is an example sketch for our Monochrome SHARP Memory Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1393

These displays use SPI to communicate, 3 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SCK 10
#define MOSI 11
#define SS 13

Adafruit_SharpMem display(SCK, MOSI, SS);

#define BLACK 0
#define WHITE 1

#define BIG_NUM_SIZE 7
#define LABEL_SIZE 2

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");

  // Count
  display.setTextColor(BLACK);
  for (uint8_t i = 0; i < 100; i++) {
    display.clearDisplay();
    
    // Draw big number
    display.setTextSize(BIG_NUM_SIZE);
    if (i < 10) display.setCursor(30,4);
    else display.setCursor(10, 4);
    display.println(i, DEC);
    
    // Draw little label
    display.setTextSize(LABEL_SIZE);
    display.println("    MPH");
    display.refresh();
    delay(1000);
  }
}

void loop(void) 
{
  // Screen must be refreshed at least once per second
  display.refresh();
  delay(500);
}
