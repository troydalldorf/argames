#include "HT16K33.h"

HT16K33 seg(0x70);

int pinArray[] = {2, 3, 4, 5, 6, 7, 8};
int p1Pin = 9;
int p2Pin = 10;
int p1Pulse = 0;
int p2Pulse = 0;
int p1Score = 0;
int p2Score = 0;
int resetPin = 11;
int position = 3;
  
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
}
  
void loop() {
  if (digitalRead(resetPin) == HIGH) {
    position = 0;
    p1Pulse = 0;
    p2Pulse = 0;
    p1Score = 0;
    p2Score = 0;
    clear();
  }
  int p1 = digitalRead(p1Pin);
  int p2 = digitalRead(p2Pin);
  if (((p1 == HIGH) && (p1Pulse %2 == 0)) || ((p1 == LOW) && (p1Pulse %2 == 1))) p1Pulse++;
  if (((p2 == HIGH) && (p2Pulse %2 == 0)) || ((p2 == LOW) && (p2Pulse %2 == 1))) p2Pulse++;
  digitalWrite(pinArray[position], LOW);
  position = 3+(p1Pulse-p2Pulse);
  digitalWrite(pinArray[position], HIGH);
  if (position == 6) lose(2);
  if (position == 0) lose(1);
}

void writeScore()
{
    seg.displayTime(p1Score, p2Score, true, false);  // do not display leading zero.
    seg.displayColon(1);
}

void clear()
{
  for (int i=0;i<=6;i++) {
    digitalWrite(pinArray[i], LOW);
  }
  writeScore();
}

void lose(int player) {
  int losePosition = 0;
  if (player == 1) {
    losePosition = 0;
    p2Score++;
  }
  else {
    losePosition = 3;
    p1Score++;
  }
  p1Pulse = 0;
  p2Pulse = 0;
  clear();
  for (int i=0;i<=5;i++) {
    digitalWrite(pinArray[losePosition], HIGH);
    delay(100);
    digitalWrite(pinArray[losePosition], LOW);
    delay(100);
  }
}