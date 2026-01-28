#include <Servo.h>

Servo steering;

const int SERVO_PIN = 9;
const int LEFT_BTTN = 3;
const int RIGHT_BTTN = 2;

const int LEFT_POS = 40;
const int RIGHT_POS = 140;
const int CENTER_POS = 90;

unsigned long lastButtonTime = 0;
const unsigned long debounceMs = 150;

void setup() {
  steering.attach(SERVO_PIN);
  steering.write(CENTER_POS);

  pinMode(LEFT_BTTN, INPUT_PULLUP);
  pinMode(RIGHT_BTTN, INPUT_PULLUP);
}

void loop() {
  unsigned long now = millis();

  // With INPUT_PULLUP: pressed == LOW
  bool leftPressed  = (digitalRead(LEFT_BTTN) == LOW);
  bool rightPressed = (digitalRead(RIGHT_BTTN) == LOW);

  if (now - lastButtonTime > debounceMs) {
    if (leftPressed && !rightPressed) {
      steering.write(LEFT_POS);
      lastButtonTime = now;
    }
    else if (rightPressed && !leftPressed) {
      steering.write(RIGHT_POS);
      lastButtonTime = now;
    }
    else if (rightPressed && leftPressed) {
      steering.write(CENTER_POS);
      lastButtonTime = now;
    }
  }

  delay(20);
}
