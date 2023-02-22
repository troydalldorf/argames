
int pinArray[] = {2, 3, 4, 5, 6, 7, 8};
int p1Pin = 9;
int p2Pin = 10;
int resetPin = 11;
int count = 0;
int timer = 175;
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
  }
  else if (digitalRead(p1Pin) == HIGH) {
    delta = 0;
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