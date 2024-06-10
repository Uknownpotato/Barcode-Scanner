//#include <Inventory.h>
#include <time.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <SPI.h>
#include <SD.h>
//#include <iostream.h>
using namespace std;
const int buttonPin = 2;
const int rRGB = A0;
const int gRGB = A1;
const int bRGB = A2;
const int soundPin = 8;
const byte interruptPin = 2;
int debounceTime = 20;
volatile byte state = LOW;
const byte RED[3] = {255, 0, 0};
const byte GREEN[3] = {0, 255, 0};
const byte BLUE[3] = {0, 0, 255};
const byte ORANGE[3] = {255, 128, 0};
const byte OFF[3] = {0, 0, 0};
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
int sleepMel1[] = {e, 0, e, g};
int sleepMel2[] = {80, 20, 200, 800};

String scannedObjects[5] = {"", "", "", "", ""};

File savedObjects;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(rRGB, OUTPUT);
  pinMode(gRGB, OUTPUT);
  pinMode(bRGB, OUTPUT);
  pinMode(soundPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), lightBlink, RISING);
}

bool scanning = false;

bool buttonPress(){
  int buttonState;
  buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
      delay(debounceTime);
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH) {
        return true;
      } 
    } 
      return false;
}

String scanEAN(){
  while (Serial.available() == 0){};
  return Serial.readString();
}

bool stopScan(){
}

void saveProductsToFile(String scanObj[], String name){
  int numObj = 5;
  savedObjects = SD.open(name, FILE_WRITE);
  for (int i = 0; i < numObj; i++){
    savedObjects.println(scanObj[i]);
    Serial.println(scanObj[i]);
  }
    savedObjects.close();
}

void readProductsFromFile(String name){
  savedObjects = SD.open(name);
  while (savedObjects.available()) {
      Serial.println(savedObjects.read());
    }
    // close the file:
    savedObjects.close();
  
}

void setColor(const byte color[3]) {
  analogWrite(rRGB, color[0]);
  analogWrite(gRGB, color[1]);
  analogWrite(bRGB, color[2]);
}

void lightBlink(){
  setColor(RED);
  delay(500);
  setColor(GREEN);
  delay(500);
  setColor(BLUE);
  delay(500);
  setColor(ORANGE);
  delay(500);
  setColor(OFF);
  delay(500);
}

void wakeUp(){

}

void loop() {
  /*playMelody(startMelody1, startMelody2, sizeof(startMelody1)/sizeof(startMelody1[0]));
  delay(1000);
  playMelody(successMel1, successMel2, sizeof(successMel1)/sizeof(successMel1[0]));
  delay(1000);
  playMelody(failureMel1, failureMel2, sizeof(failureMel1)/sizeof(failureMel1[0]));

  lightBlink();*/
  timeInactive();
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
  int i = 0;
  do {
    scannedObjects[i] = scanEAN();
    i++;
    while(!buttonPress() && Serial.available() == 0){};
  } while(!buttonPress() && i < 5);
  saveProductsToFile(scannedObjects, "savedobjects.txt");
  lightBlink();
  readProductsFromFile("savedobjects.txt");
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
}
      if (currentTime - startTime >= 12000) {
        enterSleepMode();       //when inactive for 120 seconds, enters sleepmode
  }
  }

void enterSleepMode() {
  playMelody(sleepMel1, sleepMel2, sizeof(sleepMel1)/sizeof(sleepMel1[0]));
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
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
