#include<string.h>
String sendMessage;
String receivedMessage;

void setup() {
  Serial.begin(9600);    // Initialize the Serial monitor for debugging
  //Serial1.begin(9600);   // Initialize Serial1 for sending data
}

void loop() {
  //Serial.print("\nHej");
  //delay(3000);
  while (Serial.available() == 0) {}
  //  Serial.print("\nDu");
  //  int receivedChar = Serial.read();
  //  if (receivedChar > 1) {
  //  Serial.print(receivedChar);  // Print the received message in the Serial monitor
  //  Serial.print("\nDu");
    String input = Serial.readString();
    unsigned long int intinput = input.toInt();
    Serial.print("Echo: ");
    Serial.println(input);
    Serial.println(intinput);
  //  }
  

 
  
}

/*void serialEvent() {
    String input = Serial.readString();
    Serial.print("Echo: ");
    Serial.println(input);
}*/