//#include <Inventory.h>
#include <time.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <SPI.h>
#include <SD.h>
//#include <iostream.h>
using namespace std;
const int buttonPin = 2;
const int lightPin = 8;
const int soundPin = 9;
const byte interruptPin = 2;
int debounceTime = 20;
volatile byte state = LOW;
const int c = 262;
const int db = 277;
const int d = 294;
const int eb = 311;
const int e = 330;
const int f = 349;
const int gb = 370;
const int g = 392;
const int ab = 415;
const int a = 440;
const int b = 466;
const int h = 494;
int startMelody1[] = {c, d, e};
int startMelody2[] = {200, 200, 400};
int successMel1[] = {c, g};
int successMel2[] = {200, 400};
int failureMel1[] = {c, a/2};
int failureMel2[] = {200, 400};

File savedObjects;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(lightPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), lightBlink, RISING);
}

bool scanning = false;

bool buttonPressAdd(){
}

bool buttonPressRemove(){
}

String scanEAN(){
  while (Serial.available() == 0);
  return Serial.readString();
}

bool stopScan(){
}

void saveProductsToFile(Inventory scanObj, String name){
  savedObjects = SD.open(name, FILE_WRITE);
  if(savedObjects){
    for (auto i = ){
      savedObjects.println(...);
    }
    savedObjects.close();
  }
}

void lightBlink(){
  digitalWrite(lightPin,HIGH);
  delay(1000);
  digitalWrite(lightPin, LOW);
}

void loop() {
  //startSound();
  //Serial.print(sizeof(startMelody1)/sizeof(startMelody1[0]));
  playMelody(startMelody1, startMelody2, sizeof(startMelody1)/sizeof(startMelody1[0]));
  delay(1000);
  playMelody(successMel1, successMel2, sizeof(successMel1)/sizeof(successMel1[0]));
  delay(1000);
  playMelody(failureMel1, failureMel2, sizeof(failureMel1)/sizeof(failureMel1[0]));

  lightBlink();
  timeInactive();
  digitalWrite(lightPin, HIGH);
  delay(2000);
  /*if(buttonPressAdd()){
    Inventory scannedObjects(day(), month(), year());
    scanning = true;
    while(scanning) {
      scannedObjects.addProduct(scanEAN());
      scanning = !stopScan();
    }
    saveProductsToFile();
    saveFileToDB();
  }*/

}

void timeInactive() {

unsigned long startTime = millis();   // starts counting since function called
unsigned long currentTime = millis();
int buttonState;

  while (currentTime - startTime < 12000) { 
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      delay(debounceTime);
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        break;
      }   
    }
    currentTime = millis();
    //Serial.print(currentTime);
    //Serial.print("\n");
    //delay(500);
}
      if (currentTime - startTime >= 12000) {
        enterSleepMode();       //when inactive for 120 seconds, enters sleepmode
  }
  }

void enterSleepMode() {
set_sleep_mode(SLEEP_MODE_PWR_DOWN);
sleep_mode();
}

void startSound(){
  tone(soundPin, c);
  delay(200);
  tone(soundPin, d);
  delay(200);
  tone(soundPin, e);
  delay(200);
  noTone(soundPin);
}

void approvedSound(){
  tone(soundPin, c, 150);
  delay(200);
  tone(soundPin, c);
  delay(200);
  tone(soundPin, g);
  delay(400);
  noTone(soundPin);
}

void playMelody(int tonePitch[], int toneDur[], int numberTones){
  for (int i = 0; i < numberTones; i++){
    if (tonePitch[i] == 0){
      noTone(soundPin);
      delay(toneDur[i]);
    } else {
      tone(soundPin, tonePitch[i]);
      delay(toneDur[i]);
    }
  }
  noTone(soundPin);
}
