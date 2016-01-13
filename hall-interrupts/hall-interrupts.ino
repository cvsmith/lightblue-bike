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
#define HALL 7

Adafruit_SharpMem display(SCK, MOSI, SS);

#define BLACK 0
#define WHITE 1

#define BIG_NUM_SIZE 7
#define LABEL_SIZE 2

int hallState;
volatile int count = 0;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  display.setTextColor(BLACK);
  display.setTextSize(BIG_NUM_SIZE);
  display.setCursor(30,4);
  
  attachInterrupt(digitalPinToInterrupt(HALL), incr, RISING);
}

void loop(void) 
{
  hallState = digitalRead(HALL);
  display.clearDisplay();

  display.clearDisplay();
  // Draw big number
  display.setTextSize(BIG_NUM_SIZE);
  if (count < 10) display.setCursor(30, 4);
  else display.setCursor(10, 4);
  display.println(count, DEC);
  
  // Draw little label
  display.setTextSize(LABEL_SIZE);
  display.println("    MPH");
  display.refresh();
  
  delay(500);
}

void incr()
{
  count++;
}
