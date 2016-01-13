/* 
 * Program should count the number of passes a magnet makes by the sensor,
 * and display that count on the display, refreshing every 500
 */

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
