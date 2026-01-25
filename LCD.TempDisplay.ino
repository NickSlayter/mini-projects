#include <LiquidCrystal.h>

  // LCD pins: RS, E, D4, D5, D6, D7
  LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

  const int tempPin = A0;

  // If you power everything from 5V, leave this as 5.0.
  // If you power from 3.3V, change this to 3.3 (or measure it with a multimeter).

  const float VREF = 5.0;

  // variables for LCD Menu
  int page = 0; // 0=tempC, 1=tempF, 2=raw, 3=voltage
  const int NUM_PAGES = 4;

  const int btnNext = 2;
  const int btnPrev = 3;

  // Debounce / event tracking
  bool lastNext = HIGH;
  bool lastPrev = HIGH;
  unsigned long lastButtonTime = 0;
  const unsigned long debounceMs = 150;

  int readAveragedAnalog(int pin, int samples = 20) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(2); // tiny pause helps decorrelate noise
  }
  return (int)(sum / samples);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) { ; }

  Serial.println("time_ms,tempC,tempF,raw,voltage");
  
  lcd.begin(16, 2);
  lcd.clear();

  pinMode(btnNext, INPUT_PULLUP);
  pinMode(btnPrev, INPUT_PULLUP);

  lcd.setCursor(0,0);
  lcd.print("Menu Demo");

  delay(800);

}

void renderPage(float tempC, float tempF, int raw, float voltage) {
  lcd.clear();

  lcd.setCursor(0, 0);

  switch (page) {
    case 0:
      lcd.print("Temp (C)");
      lcd.setCursor(0, 1);
      lcd.print(tempC, 1);
      lcd.print((char)233);
      lcd.print("C");
      break;

    case 1:
      lcd.print("Temp (F)");
      lcd.setCursor(0, 1);
      lcd.print(tempF, 1);
      lcd.print((char)223);
      lcd.print("F");
      break;

    case 2:
      lcd.print("ADC Raw");
      lcd.setCursor(0, 1);
      lcd.print(raw);
      break;

    case 3:
      lcd.print("Voltage");
      lcd.setCursor(0, 1);
      lcd.print(voltage, 3);
      lcd.print(" V");
      break;
  }
  lcd.setCursor(15, 0);
  lcd.print(page); // 0-3
}

void loop() {
  
  // put your main code here, to run repeatedly:

  int raw = readAveragedAnalog(tempPin, 20);
  float voltage = raw * (VREF / 1023.0);
  float tempC = (voltage - 0.5) * 100.0;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  /* This block used when not doing the menu system, outputs all 4 display values to LCD
  // Convert ADC reading to voltage
  float voltage = raw * (VREF / 1023.0);

  // TMP36: 10 mV per °C with 500 mV offset at 0°C
  float tempC = (voltage - 0.5) * 100.0;
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Line 1: show raw + voltage (useful for debugging)
  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(voltage, 3);
  lcd.print(" Raw:");
  lcd.print(raw);

  // Pad the rest of the line (clears leftover chars)
  lcd.print("    ");

  // Line 2: show temperature
  lcd.setCursor(0, 1);
  lcd.print(tempC, 1);
  lcd.print((char)223); // degree symbol on most HD44680 LCDs
  lcd.print("C  ");

  lcd.print(tempF, 1);
  lcd.print((char)223);
  lcd.print("F");

  // Pad end of line if needed
  lcd.print("   "); 
  */

// Read buttons -> generate events ---

  bool nextNow = digitalRead(btnNext); // HIGH = not pressed, LOW = pressed
  bool prevNow = digitalRead(btnPrev);

  unsigned long now = millis();

  // Detect a "press event": last was HIGH, now LOW (falling edge)
  if (now - lastButtonTime > debounceMs) {
    if (lastNext == HIGH && nextNow == LOW) {
      page = (page + 1) % NUM_PAGES;
      lastButtonTime = now;
      renderPage(tempC, tempF, raw, voltage);
    }
    if (lastPrev == HIGH && prevNow == LOW) {
      page = (page - 1 + NUM_PAGES) % NUM_PAGES;
      lastButtonTime = now;
      renderPage(tempC, tempF, raw, voltage);
    }
  }
  
  lastNext = nextNow;
  lastPrev = prevNow;

  // ---- 3) Render periodically even if no button pressed ----
  // (so the value updates while you stay on one page)
  static unsigned long lastRender = 0;
  const unsigned long renderIntervalMs = 500;

  if (now - lastRender >= renderIntervalMs) {
    renderPage(tempC, tempF, raw, voltage);
    lastRender = now;
  }

  //Printing to Serial using Serial.print();

  unsigned long t = millis();

  Serial.print(t);
  Serial.print(",");
  Serial.print(tempC, 2);
  Serial.print(",");
  Serial.print(tempF, 2);
  Serial.print(",");
  Serial.print(raw);
  Serial.print(",");
  Serial.println(voltage, 3);

  delay(800);


}
