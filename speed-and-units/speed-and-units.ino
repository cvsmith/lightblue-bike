                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    /* LightBlue Bike
 * Christopher Smith, Joseph Paetz, Matthew Klein, Samuel Westenberg */

/* Counts number of passes magnet makes by sensor
 * Calculates speed and distance traveled by the bike wheel
 * Writes that data to the display 
 * Changes data displayed when buttons are pressed
 * Change magic numbers like PULSES_TO_MI for different bike wheels */

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

// any pins can be used
#define SCK 5 //10
#define MOSI 4 //11
#define SS 3 //13
#define HALL 2 // Interrupt number. Pin 2 on Bean. 7 on Leo.
#define UNIT_BUTTON 1 //2
#define DISP_BUTTON 0 //3

// Display constants
#define BLACK 0
#define WHITE 1
#define BIG_NUM_SIZE 7  // Big speed size
#define UNIT_SIZE 2     // Unit label size

#define SAMPLE_INTERVAL 2000

// For bike wheel 2.168m in circumference
#define PULSES_PER_S_TO_MPH 4.85
#define PULSES_PER_S_TO_KPH 7.80
#define PULSES_TO_MI 0.00135 
#define PULSES_TO_KM 0.00217

// For unit variable
#define MI 0
#define KM 1

// For display configuration variable
#define SPD_BIG 0
#define DIS_BIG 1

Adafruit_SharpMem display(SCK, MOSI, SS);

// Count for speed, total_count for distance
volatile int count = 0;
volatile int total_count = 0;

// prevCount for moving avg, currCount is temp
int prevCount = 0;
int currCount;

// Timing variables
unsigned long start = 0;
unsigned long now;
unsigned long tempStart;

// Distance and speed variables
float distance = 0;
float spd = 0;
int spd_round = 0;

// Unit variables
int unit = MI;
float pulses_to_unit;

// Display variables
int disp = SPD_BIG;

// Button variables
int dispBtnReading;
int unitBtnReading;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  display.setTextColor(BLACK);
  display.setTextSize(BIG_NUM_SIZE);
  display.setCursor(30,4);
  
  attachInterrupt(HALL, incr, RISING);
  pinMode(UNIT_BUTTON, INPUT);
  pinMode(DISP_BUTTON, INPUT);

  startAnimation();
  display.clearDisplay();
}

void loop(void) 
{
  // Button stuff
  // Read both buttons, then reset, switch units, or switch display mode
  dispBtnReading = digitalRead(DISP_BUTTON);
  unitBtnReading = digitalRead(UNIT_BUTTON);
  if (dispBtnReading && unitBtnReading) {
    Serial.println("Reset button!");
    startAnimation();
    display.clearDisplay();
    prevCount = 0;
    count = 0;
    total_count = 0;
    start = 0;
    spd = 0;
    distance = 0;
  } else if (unitBtnReading) switchUnit();
  else if (dispBtnReading) switchDisp();
  
  // Speed stuff
  // If more than a sample interval has passed, calculate new stats
  now = millis();
  // Default value after reset
  if (start == 0) start = millis();
  else if (now - start >= SAMPLE_INTERVAL) {
    currCount = count;
    tempStart = start;
    start = millis();
    pulses_to_unit = (unit == MI) ? PULSES_PER_S_TO_MPH : PULSES_PER_S_TO_KPH;
    spd = ((prevCount * 0.5 + currCount) * 1000 * pulses_to_unit / (now - tempStart)) * 0.5;
    prevCount = count;
    count = 0;
    Serial.print(digitalRead(2));
    //Serial.print("spd: "); Serial.print(spd);
    //Serial.print(" count: "); Serial.println(currCount);
  }
  
  spd_round = round(spd);
  
  if (unit == MI) distance = total_count * PULSES_TO_MI;
  else if (unit == KM) distance = total_count * PULSES_TO_KM;
  
  // Display stuff
  display.clearDisplay();
  // Draw big number
  display.setTextSize(BIG_NUM_SIZE);
  //if (spd_round < 10) display.setCursor(30, 4);
  display.setCursor(10, 4);
  if (disp == SPD_BIG) {
    if (spd_round < 10) display.println(spd_round, 1);
    else display.println(spd_round, DEC);
  } else {
    display.setCursor(4, 4); 
    display.setTextSize(BIG_NUM_SIZE - 2);
    display.println(distance, 1);
  }
  
  // Draw big number's label
  display.setTextSize(UNIT_SIZE);
  if (disp == SPD_BIG) {
    if (unit == MI) display.println("    MPH");
    else if (unit == KM) display.println("    KPH"); 
  } else {
    if (unit == MI) display.println("     MI\n");
    else display.println("    KPH\n");
  }
  
  // Draw little number and label
  display.setTextSize(UNIT_SIZE);
  display.print(" "); 
  if (disp == SPD_BIG) {
    display.print(distance, 1); 
    if (unit == MI) display.println(" MI");
    else if (unit == KM) display.println(" KM");
  } else {
    display.print(spd_round, 1); display.print(" ");
    if (unit == MI) display.println("MPH");
    else if (unit == KM) display.println("KPH");
  }
  display.refresh();

  delay(SAMPLE_INTERVAL);
}

void incr()
{
  count++;
  total_count++;
  Serial.println("420");
}

void switchUnit()
{
  Serial.println("Unit button!");
  unit = !unit;
}

void switchDisp()
{
  Serial.println("Disp button!");
  disp = !disp;
}

// Adapted from code written by Limor Fried/Ladyada for Adafruit Industries  
void startAnimation() {  
  display.clearDisplay();
  for (uint8_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, BLACK);
    display.refresh();
  }
  for (uint8_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, BLACK); 
    display.refresh();
  }
  delay(250);
}
