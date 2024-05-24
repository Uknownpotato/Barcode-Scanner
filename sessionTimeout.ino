#include <avr/sleep.h>
#include <avr/interrupt.h>

#define LED_PIN 8 
#define BUTTON_PIN 7

const int buttonPin = 2;
const byte interruptPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), wakeUp, FALLING); 
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {

 if (digitalRead(BUTTON_PIN) == HIGH) {
   digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
sessionTimeOut();

}

void sessionTimeOut() {
  unsigned long startTime = millis();
  unsigned long currentTime = millis();

  while (true) {
    currentTime = millis();

    if (digitalRead(BUTTON_PIN) == HIGH) {    // add on || Serial.available() > 0
      // Add/remove new object or register mode change
      // Exit loop if new input is found
    return;
    }
    if (currentTime - startTime >= 5000 || digitalRead(buttonPin) == HIGH) {
      enterSleepMode();
      return;
    }
  }
 }

void controlLED() {
  
if (digitalRead(buttonPin) == HIGH) {
     digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void enterSleepMode() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  Serial.println("Entering sleep mode...");
  sleep_enable();
  sleep_mode(); // Enter sleep mode
  // The program will continue from here after waking up

  // Disable sleep mode
  sleep_disable(); 
  digitalWrite(LED_PIN, HIGH);
  delay(2000);

  // Reattach interrupt for next sleep cycle
  attachInterrupt(digitalPinToInterrupt(interruptPin), wakeUp, FALLING); 
}

void wakeUp() {
  Serial.println("Waking up...");
  return;
}

