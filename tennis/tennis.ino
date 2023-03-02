#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int pinArray[] = {2, 3, 4, 5, 6, 7, 8};
int p1Pin = 9;
uint16_t p1Score = 0;
int p2Pin = 10;
uint16_t p2Score = 0;
int resetPin = 11;
int baseTimer = 180;
int timerFactor = 60;
int delta = 0;
int position = 0;
int prevT = millis();
int currentT = millis();
int currentTimer = baseTimer;
  
void setup(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  for (int count=0;count<=6;count++) {
    pinMode(pinArray[count], OUTPUT);
  }
  pinMode(p1Pin, INPUT);
  pinMode(p2Pin, INPUT);
  pinMode(resetPin, INPUT);
  reset();
}
  
void loop() {
  digitalWrite(pinArray[position], HIGH);
  currentT = millis();
  if (digitalRead(resetPin) == HIGH) {
    reset();
  }
  if (digitalRead(p1Pin) == HIGH) {
    if (position > 3 && delta < 0) {
      lose(1);
    }
    else if (position <= 2) {
      delta = 1;
      currentTimer = baseTimer - (position) * timerFactor;
    }
  }
  if (digitalRead(p2Pin) == HIGH) {
    if (position < 4 && delta > 0) {
      lose(2);      
    }
    else if (position >= 4) {
      delta = -1;
      currentTimer = baseTimer - (6-position) * timerFactor;
    }
  }
  if (currentT - prevT > currentTimer) {
    prevT = currentT;
    digitalWrite(pinArray[position], LOW);
    position += delta;
    if (position > 6) {
      lose(2);
    }
    else if (position < 0) {
      lose(1);
    }
  }
}

void reset()
{
  delta = 0;
  p1Score = 0;
  p2Score = 0;
  if (random(1, 3) == 1) position = 0; else position = 6;
  clear();
  writeScore();
}

void clear()
{
  for (int i=0;i<=6;i++) {
    digitalWrite(pinArray[i], LOW);
  }
}

void writeScore()
{
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("  TENNIS");

  display.setTextSize(4);
  display.setCursor(0, 28); 
  display.print(p1Score);
  display.setCursor(72, 28);
  display.println(p2Score);
  display.display();

  display.display();
}

void lose(int player) {
  clear();
  int losePosition = 0;
  delta = 0;
  if (player == 1) {
    losePosition = 0;
    position = 6;
    p2Score++;
  }
  else {
    losePosition = 6;
    position = 0;
    p1Score++;
  }
  writeScore();
  for (int i=0;i<=5;i++) {
    digitalWrite(pinArray[losePosition], HIGH);
    delay(100);
    digitalWrite(pinArray[losePosition], LOW);
    delay(100);
  }
}
