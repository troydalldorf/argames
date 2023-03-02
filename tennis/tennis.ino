#include "HT16K33.h"

HT16K33 seg(0x70);

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
  seg.begin();
  Wire.setClock(100000);
  seg.displayOn();
  seg.setDigits(4);
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
    seg.displayTime(p1Score, p2Score, true, false);  // do not display leading zero.
    seg.displayColon(1);
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
