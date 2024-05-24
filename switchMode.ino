const byte rRGB = 9;
const byte gRGB = 10;
const byte bRGB = 11;

const byte RED[3] = {255, 0, 0};
const byte GREEN[3] = {0, 255, 0};
const byte BLUE[3] = {0, 0, 255};
const byte ORANGE[3] = {255, 128, 0};
const byte OFF[3] = {0, 0, 0};

unsigned long previousMillis = 0;
const int interval = 1000;

enum Mode { 
START_UP, 
SCAN_IN_MODE, 
REMOVE_MODE,
IDLE_MODE }; // modes used defined
Mode CURRENT_MODE = startUp; // initial mode

void setup() {

  pinMode(rRGB, OUTPUT);
  pinMode(gRGB, OUTPUT);
  pinMode(bRGB, OUTPUT);
  setStartUpLED();

}

unsigned long testTime = 0;

void loop() {
  sessionTimeout();
  testTime = millis();

  switch (CURRENT_MODE) {
    case START_UP:
      if ((connectionFailed == true) || (testTime - previousMillis < 3000)) {
        setNotConnectedLED();
      } else if ((connectionSuccess() == true) || (testTime - previousMillis >= 3000)) {
        setConnectedLED();
        CURRENT_MODE = SCAN_IN_MODE;
        previousMillis = testTime;
      }
      break;

    case SCAN_IN_MODE:
      if ((!buttonPressRemove()) || (testTime - previousMillis < 3000)) {
        setScanInLED();
        scanEAN();
      } else if ((buttonPressRemove()) || (testTime - previousMillis >= 3000)) {
        CURRENT_MODE = REMOVE_MODE;
        previousMillis = testTime;
      }
      break;

    case REMOVE_MODE:
      if ((!buttonPressAdd()) || (testTime - previousMillis < 3000)) {
        setScanOutLED();
        scanEAN();
      } else if ((buttonPressAdd()) || (testTime - previousMillis >= 3000)) {
        CURRENT_MODE = SCAN_IN_MODE;
        previousMillis = testTime;
      }
      break;

      case IDLE_MODE:  // call if shutdown or sessionTimeout = true
      standByLED();
  }
}

void shutDownButton() {

  // if buttonState is HIGH for > 2000
  CURRENT_MODE = IDLE_MODE;
}

void setColor(const byte color[3]) {
  analogWrite(rRGB, color[0]);
  analogWrite(gRGB, color[1]);
  analogWrite(bRGB, color[2]);
}

// Play LED sequence for start up product
void setStartUpLED() {
  setColor(RED);
  delay(500);
  setColor(GREEN);
  delay(500);
  setColor(BLUE);
  delay(500);
  setColor(OFF);
}

// Indicate stand-by mode
void setStandByLED() {
    setColor(RED); 
}

// Indicate connected to wifi
void setConnectedLED() {
  setColor(BLUE);
  delay(1500);
  setColor(OFF);
}

// Indicate not connected wifi 
void setNotConnectedLED() {
  setColor(ORANGE);
  delay(1500);
  setColor(OFF);
}

// Indicate scan in mode
void setScanInLED() {
 setColor(GREEN);
}

// Indicate remove mode
void setRemoveLED() {
 setColor(OFF);
 delay(750);
 setScanInLED();
}