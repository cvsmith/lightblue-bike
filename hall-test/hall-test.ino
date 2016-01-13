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

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");

  // Count
  display.setTextColor(BLACK);
  display.setTextSize(BIG_NUM_SIZE);
  display.setCursor(30,4);
}

void loop(void) 
{
  hallState = digitalRead(HALL);
  display.clearDisplay();

  if (hallState == HIGH) {
    display.println("ON");
    Serial.print("ON");
  } else {
    Serial.print("OFF");
    display.println("OFF");
  }
  
  display.refresh();
  delay(500);
}
