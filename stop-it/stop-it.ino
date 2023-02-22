

int pinArray[] = {2, 3, 4, 5, 6, 7, 8};
int p1Pin = 9;
int p2Pin = 10;
int resetPin = 11;
int count = 0;
int startTimer = 160;
int startLevel = -1;
int level = startLevel;
int timer = startTimer;
int delta = 1;
int position = 0;
int prevT = millis();
int currentT = millis();
  
void setup(){
  for (count=0;count<=6;count++) {
    pinMode(pinArray[count], OUTPUT);
  }
  pinMode(p1Pin, INPUT);
  pinMode(p2Pin, INPUT);
  pinMode(resetPin, INPUT);
}
  
void loop() {
  digitalWrite(pinArray[position], HIGH);
  currentT = millis();
  if (digitalRead(resetPin) == HIGH) {
    delta = 1;
    position = 0;
    timer = startTimer;
    level = 0;
  }
  if (digitalRead(p1Pin) == HIGH) {
    if (position == 3) {
      timer = timer - 20;
      level = level + 1;
    }
    for (int i=0;i<7;i++) {
      digitalWrite(pinArray[position], LOW);
      if (level >= 0) digitalWrite(pinArray[level], HIGH);
      delay(50);
      digitalWrite(pinArray[position], HIGH);
      if (level >= 0) digitalWrite(pinArray[level], LOW);
      delay(50);
    }
    delay(250);
  }
  if (currentT - prevT > timer) {
    prevT = currentT;
    digitalWrite(pinArray[position], LOW);
    position += delta;
    if (position > 6) {
      delta = -1;
      position = 5;
    }
    else if (position < 0) {
      delta = +1;
      position = 1;
    }
  }
}