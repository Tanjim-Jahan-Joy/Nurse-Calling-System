// Bed side module program
#include <SoftwareSerial.h>
const int bufferSize = 4;
const String ID = "101";
String message[bufferSize];

#define redLedCori      8 //14
#define greenLedCori    12 //18
#define yellowLedCori   13 //19

#define redLedBed     2 //4
#define greenLedBed   4 //6
#define yellowLedBed  7 //13

#define redLedON   A5 //28
#define redLedOff  A4 //27

#define greenLedOnOff   A3 //26
#define yellowLedOnOFF  A2 //25

bool yellowButtonPressed = false;   // To track button state
unsigned long yellowLastDebounceTime = 0;  // Last time button state changed

bool greenButtonPressed = false;  // To track green button state
unsigned long greenLastDebounceTime = 0;  // Last time green button state changed

const unsigned long debounceDelay = 50;  // Debounce delay

char redLedStatus = '.';
char greenLedStatus = '.';
char yellowLedStatus = '.';

#define RS485_RO 5 // 11
#define RS485_DI 6 // 12
#define Rs485_DE_RE 6 // 12
SoftwareSerial RS_SLAVE(RS485_RO, RS485_DI);

void setup() {
  digitalWrite(redLedON,       INPUT_PULLUP);
  digitalWrite(redLedOff,      INPUT_PULLUP);
  digitalWrite(greenLedOnOff,  INPUT_PULLUP);
  digitalWrite(yellowLedOnOFF, INPUT_PULLUP);

  Serial.begin(9600);
  // Serial.println("--------");
  
  digitalWrite(redLedCori, LOW);
  digitalWrite(redLedBed,  LOW);
  digitalWrite(greenLedCori, LOW);
  digitalWrite(greenLedBed, LOW);
  //------------------------------//
  pinMode(yellowLedCori, OUTPUT);
  digitalWrite(yellowLedCori, LOW);
  digitalWrite(yellowLedBed, LOW);

  RS_SLAVE.begin(9600);
  pinMode(Rs485_DE_RE, OUTPUT);
  digitalWrite(Rs485_DE_RE, HIGH);

}

void loop() {
  if(digitalRead(redLedON) == LOW && redLedStatus == '.' && greenLedStatus == '.' && yellowLedStatus == '.'){
    digitalWrite(redLedCori, HIGH);
    digitalWrite(redLedBed,  HIGH);
    redLedStatus = 'R';
    //Serial.println("red.........");
    //RS_SLAVE.write("RED On*");
    delay(100);
  }

  if(digitalRead(redLedOff) == LOW && redLedStatus == 'R'){
    digitalWrite(redLedCori, LOW);
    digitalWrite(redLedBed,  LOW);
    redLedStatus = '.';
    //Serial.println("red of.........");
    //RS_SLAVE.write("RED Off*");
    delay(100);
  }

  //int greenButtonState = digitalRead(greenLedOnOff);

  // Check for button press (LOW) and debounce
  if (digitalRead(greenLedOnOff) == LOW && !greenButtonPressed && (millis() - greenLastDebounceTime > debounceDelay)) {
    greenButtonPressed = true;  // Record button press
    greenLastDebounceTime = millis();  // Reset debounce timer

    if (redLedStatus == '.' && greenLedStatus == '.') {
      digitalWrite(greenLedCori, HIGH);
      digitalWrite(greenLedBed, HIGH);
      greenLedStatus = 'G';
      //RS_SLAVE.write("Green On*");
    }
    else if (redLedStatus == '.' && greenLedStatus == 'G') {
      digitalWrite(greenLedCori, LOW);
      digitalWrite(greenLedBed, LOW);
      greenLedStatus = '.';
      //RS_SLAVE.write("Green Off*");
    }
    delay(100);  // Add a small delay to prevent accidental double toggles
  }

  // Check if green button is released
  if (digitalRead(greenLedOnOff) == HIGH) {
    greenButtonPressed = false;  // Reset button pressed state
  }

  // Read the state of the button
  //int buttonState = digitalRead(yellowLedOnOFF);

  // Check for button press (LOW) and debounce
  if (digitalRead(yellowLedOnOFF) == LOW && !yellowButtonPressed && (millis() - yellowLastDebounceTime > debounceDelay)) {
    yellowButtonPressed = true;  // Record button press
    yellowLastDebounceTime = millis();  // Reset debounce timer

    if (redLedStatus == '.' && yellowLedStatus == '.') {
      digitalWrite(yellowLedCori, HIGH);
      digitalWrite(yellowLedBed, HIGH);
      yellowLedStatus = 'Y';
      //RS_SLAVE.write("yellow On*");
    }
    else if (redLedStatus == '.' && yellowLedStatus == 'Y') {
      digitalWrite(yellowLedCori, LOW);
      digitalWrite(yellowLedBed, LOW);
      yellowLedStatus = '.';
      //RS_SLAVE.write("yellow Off*");
    }
    delay(100);  // Add a small delay to prevent accidental double toggles
  }

  // Check if button is released
  if (digitalRead(yellowLedOnOFF)) {
    yellowButtonPressed = false;  // Reset button pressed state
  }

  digitalWrite(Rs485_DE_RE, LOW);
  if(RS_SLAVE.available()){

    String tempID = "";
    while(RS_SLAVE.available()){
      tempID += char(RS_SLAVE.read());
      delay(5);
    }
    
    if(tempID == ID){
      digitalWrite(Rs485_DE_RE, HIGH);
      message[0] = ID;
      message[1] = redLedStatus;
      message[2] = greenLedStatus;
      message[3] = yellowLedStatus;
      
      for (int i = 0; i < bufferSize; i++) {
        if(i < (bufferSize - 1)){
          message[i] += "|";
        }
        RS_SLAVE.write(message[i].c_str());
      }
    }
  }
  delay(50);
}