#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial BLESerial(11, 10); //RX|TX
Servo myServo;

#define EN1 5
#define M1 4
#define SERVO 10

int deg = 90;

void setup() {
  Serial.begin(9600);
  BLESerial.begin(9600);
  myServo.attach(SERVO);
  myServo.write(deg);
//  while(!Serial);
  Serial.println("READY");
}

void loop(){
  while (BLESerial.available() > 0) {
    delay(200);
    if ((char)BLESerial.read() == '<') {
      char direc = BLESerial.read();
      int velocity = BLESerial.parseInt();
//      Serial.print("DIRECTION:");
//      Serial.println(direc);
//      Serial.print("SPEED:");
//      Serial.println(velocity);
      if ((char)BLESerial.read() == '>') {
//       Serial.println("DRIVING...");
       drive(direc, velocity);
      }
    }
  }
}

void drive(char d, int s) {
   switch(d) {
    case 'F':
      analogWrite(EN1, s);
      digitalWrite(M1, HIGH);
      delay(2000);
    break;
    case 'B':
      analogWrite(EN1, s);
      digitalWrite(M1, LOW);
      delay(2000);
    break;
    case 'R':
      deg = constrain(s, 90, 180);
//      Serial.print("DEGREE: ");
//      Serial.println(deg);
      myServo.write(deg);
      break;
    case 'L':
      deg = constrain(s, 0, 90);
//      Serial.print("DEGREE: ");
//      Serial.println(deg);
      myServo.write(deg);
    break;
    case 'S':
      digitalWrite(EN1, LOW);
    break;
   }
}
