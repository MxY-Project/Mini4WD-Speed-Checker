#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "LOGO.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char str_cnt[6];

// speed sensor related variable
const int sensorPin = 14;  
volatile int counter = 0;
volatile int rpm = 0;
unsigned long previousMillis = 0;
const double perimeter = 3.14 * 25; // perimeter of speed encoder in milimeter
double maxKmh = 0;
const int numReadings  = 10;
int readIndex  = 0;
long totalRpm  = 0;
int rpm_list[numReadings];

// timer related variable
unsigned long previousTimerMillis = 0;

// battery related variable
int batteryPercentage = 0;
int adcVal = 0;
double batVolt = 0.6;
const unsigned long adcReadPeriod = 5;
unsigned long previousAdcReadMillis;
unsigned int adcCounter = 0;
unsigned int batteryPercentages[15] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 50, 40, 30, 20, 10, 0};
double batteryVoltages[15] = {4.2, 4.15, 4.11, 4.08, 4.02, 3.98, 3.95, 3.91, 3.87, 3.84, 3.8, 3.77, 3.73, 3.69, 3.27};

void setup() {
  Wire.begin(SDA, SCL);
  Wire.setClock(400000);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  
  attachInterrupt(digitalPinToInterrupt(sensorPin), sensor_handler, FALLING);
  
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 128, 64, WHITE);
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  show_rpm(0);
  delay(1000);
  
  pinMode(sensorPin, INPUT_PULLUP);

}

void loop() {
   if (counter >= 1) { 
      unsigned long currentMillis = millis();
      rpm = counter * (60 * 1000 / (currentMillis - previousMillis)); // 60 * 10000 to convert from miliseconds to minutes
      rpm = movingAverage(rpm); // perform smoothing
      previousMillis = currentMillis;
      counter = 0;

      show_rpm(rpm);
      delay(200); // refresh display duration
   }
   else{
      show_rpm(0);
      maxKmh = 0;
      previousTimerMillis = millis();
   }
   setBatteryVolt();
}