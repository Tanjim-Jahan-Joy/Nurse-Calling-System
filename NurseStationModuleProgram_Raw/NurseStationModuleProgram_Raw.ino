//Nurse Station Module- Program
#include <SoftwareSerial.h>
#define RS485_RO 12 // 18
#define RS485_DI 13 // 19
#define RS485_DE_RE 4 // 6

const int noOfRoom = 10;
const int bufferSize = 4;
String message[4];
const String RoomAdd[noOfRoom] = {"101","102","103","104","105","106","107","108","109","110"};
SoftwareSerial RS_MASTER(RS485_RO, RS485_DI);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  RS_MASTER.begin(9600);
  pinMode(RS485_DE_RE, OUTPUT);
  digitalWrite(RS485_DE_RE, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < noOfRoom; i++){
    digitalWrite(RS485_DE_RE, HIGH);
    RS_MASTER.write(RoomAdd[i].c_str());
    digitalWrite(RS485_DE_RE, LOW);
    delay(200);
    if(RS_MASTER.available()){
      Serial.println(".......");
      int bufferIndex = 0;
      String tempMsg = "";
      while(RS_MASTER.available()){
        char msg = RS_MASTER.read();
        if(msg == '|'){
          message[bufferIndex] = tempMsg;
          bufferIndex++;
          tempMsg = "";
        }else{
          tempMsg += msg;
        }
        delay(5);
      }
      message[bufferIndex] = tempMsg;
      for(int i = 0; i < bufferSize; i++){
        Serial.print(message[i]);
      }
      Serial.print("\n");
      delay(200);
    }
  }
}