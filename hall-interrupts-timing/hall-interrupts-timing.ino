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

#define BLACK 0
#define WHITE 1

#define BIG_NUM_SIZE 7
#define UNIT_SIZE 2

#define SAMPLE_INTERVAL 2000
#define PULSES_PER_S_TO_MPH 4.85
#define PULSES_TO_MILES 0.00135 
#define PREV_LENGTH 5

Adafruit_SharpMem display(SCK, MOSI, SS);

volatile int count = 0;
volatile int total_count = 0;
float distance = 0;
unsigned long start = 0;
unsigned long now;
float spd = 0;
int spd_round = 0;
int prev [PREV_LENGTH]; 

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
  int hallCount = count;
  // Speed stuff
  now = millis();
  if (start == 0) {
    start = millis();
  } else if (now - start >= SAMPLE_INTERVAL) {
    Serial.print("count: "); Serial.print(hallCount);
    spd = ((spd * 0.5) + (hallCount * 1000 * PULSES_PER_S_TO_MPH / SAMPLE_INTERVAL)) / 2;
    Serial.print(" spd: "); Serial.println(spd);
    count = 0;
    start = millis();
  }
  
  spd_round = round(spd);
  distance = total_count * PULSES_TO_MILES;
  
  // Display stuff
  display.clearDisplay();
  // Draw big number
  display.setTextSize(BIG_NUM_SIZE);
  if (spd_round < 10) display.setCursor(30, 4);
  else display.setCursor(10, 4);
  display.println(spd_round, DEC);
  
  // Draw little label
  display.setTextSize(UNIT_SIZE);
  display.println("    MPH");
  
  // Draw distance
  display.setTextSize(UNIT_SIZE);
  display.print(" "); display.print(distance, 2); display.print("MI");
  display.refresh();

  delay(SAMPLE_INTERVAL);
}

void incr()
{
  count++;
  total_count++;
}
