                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    /* 
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
#define BUTTON 0

#define BLACK 0
#define WHITE 1

#define BIG_NUM_SIZE 7
#define UNIT_SIZE 2

#define SAMPLE_INTERVAL 2000
#define PULSES_PER_S_TO_MPH 4.85
#define PULSES_PER_S_TO_KPH 7.80
#define PULSES_TO_MI 0.00135 
#define PULSES_TO_KM 0.00217
#define PREV_LENGTH 5

#define MI 0
#define KM 1

#define CLEAR 0
#define HOLD 1

Adafruit_SharpMem display(SCK, MOSI, SS);

volatile int count = 0;
volatile int total_count = 0;

int prevCount = 0;
int currCount;

unsigned long start = 0;
unsigned long now;
unsigned long tempStart;

float distance = 0;
float spd = 0;
int spd_round = 0;

int unit = 0;
float pulses_to_unit;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  display.setTextColor(BLACK);
  display.setTextSize(BIG_NUM_SIZE);
  display.setCursor(30,4);
  
  attachInterrupt(digitalPinToInterrupt(HALL), incr, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON), switchUnit, RISING);

}

void loop(void) 
{
  // Speed stuff
  now = millis();
  if (start == 0) {
    start = millis();
  } else if (now - start >= SAMPLE_INTERVAL) {
    currCount = count;
    tempStart = start;
    start = millis();
    pulses_to_unit = (unit == MI) ? PULSES_PER_S_TO_MPH : PULSES_PER_S_TO_KPH;
    spd = ((prevCount * 0.5 + currCount) * 1000 * pulses_to_unit / (now - tempStart)) * 0.5;
    prevCount = count;
    count = 0;
    //Serial.print(" spd: "); Serial.println(spd);
    //Serial.print("count: "); Serial.print(currCount);
  }
    
  spd_round = round(spd);
  if (unit == MI) distance = total_count * PULSES_TO_MI;
  else if (unit == KM) distance = total_count * PULSES_TO_KM;
  
  // Display stuff
  display.clearDisplay();
  // Draw big number
  display.setTextSize(BIG_NUM_SIZE);
  if (spd_round < 10) display.setCursor(30, 4);
  else display.setCursor(10, 4);
  display.println(spd_round, DEC);
  
  // Draw little label
  display.setTextSize(UNIT_SIZE);
  if (unit == MI) display.println("    MPH");
  else if (unit == KM) display.println("    KPH"); 
  
  // Draw distance
  display.setTextSize(UNIT_SIZE);
  display.print(" "); display.print(distance, 2); 
  if (unit == MI) display.print("MI");
  else if (unit == KM) display.println("KM");
  display.refresh();

  delay(SAMPLE_INTERVAL);
}

void incr()
{
  count++;
  total_count++;
}

void switchUnit()
{
  Serial.println("Button!");
  unit = !unit;
}
