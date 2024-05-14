#include <avr/sleep.h>
#include <avr/interrupt.h>
const int buttonPin = 2;
const byte interruptPin = 2;
volatile byte state = LOW;

void setup() {

Serial.begin(9600);     //initializes serial data communication
pinMode(buttonPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(interruptPin), enterSleepMode, RISING);

}

void loop() {

timeInactive();       // calls the function to start counting after an action

}

void timeInactive() {

unsigned long startTime = millis();   // starts counting since function called
int buttonState;

  while (startTime < 120000) { 
    buttonState = digitalRead(2);
    if (buttonState == HIGH || Serial.available() > 0) {    //breaks loop if new input is found
      // add that the new scan or mode change also registers in loop !
      // addObj(++)
      break;
   
    }
}
      if (startTime >= 120000) {
        enterSleepMode();       //when inactive for 120 seconds, enters sleepmode
  }
  }

void enterSleepMode() {
set_sleep_mode(SLEEP_MODE_PWR_DOWN);
sleep_enable();
sleep_mode();
}

void wakeUp() {
detachInterrupt(digitalPinToInterrupt(interruptPin));
}