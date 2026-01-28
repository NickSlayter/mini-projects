#include <Servo.h>

Servo steering;

const int SERVO_PIN = 9;
const int LEFT_BTTN = 3;
const int RIGHT_BTTN = 2;


const int LEFT_LIM = 20;
const int RIGHT_LIM = 160; 
const int CENTER_POS = 90;
int position = 90;



unsigned long lastButtonTime = 0;
const unsigned long debounceMs = 150;

void setup() {
  // put your setup code here, to run once:
  steering.attach(SERVO_PIN);  
  steering.write(CENTER_POS);

  pinMode(LEFT_BTTN, INPUT_PULLUP);
  pinMode(RIGHT_BTTN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long now = millis();

  bool leftPressed = (digitalRead(LEFT_BTTN) == LOW);
  bool rightPressed = (digitalRead(RIGHT_BTTN) == LOW);

  if (now - lastButtonTime > debounceMs) {
    if (leftPressed && !rightPressed && ((position - 5) >= LEFT_LIM)) {
      position -= 5;
      steering.write(position);
      lastButtonTime = now;
    }
    else if (rightPressed && !leftPressed && ((position + 5) <= RIGHT_LIM)) {
      position += 5;
      steering.write(position);
      lastButtonTime = now;
    }
    else if (rightPressed && leftPressed) {
      steering.write(CENTER_POS);
      position = CENTER_POS;
      lastButtonTime = now;
    }
  }

  // For debugging
  Serial.print(leftPressed);
  Serial.print(" ");
  Serial.println(rightPressed);
  Serial.print("Position: ");
  Serial.println(position);




  delay(20);
} 



