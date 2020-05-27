/*  
 *  AUTO STEPPER
 *  
 *  OBS: - ALWAYS USE AN EXTERNAL POWER SOURCE.
 *  
 *  Author: Gustavo G. Pires
 *  GitHub: GuhPires
*/

#include <Stepper.h>
#include <Servo.h>

#define LDR A1        // LDR
#define SERVO 7       // Servo motor pin
#define MIN_DEG 60    // Servo's min degrees
#define MAX_DEG 120   // Servo's min degrees
#define SPR 2048      // Steps per Revolution (28BYJ-48)
#define RPM 15        // Max Stepper RPM (28BYJ-48)

Stepper xAxisStepper = Stepper(SPR, 2, 4, 3, 5);
Stepper yAxisStepper = Stepper(SPR, 8, 10, 9, 11);
Servo dropperServo;

const int maxXSteps = SPR * 3.6;                              // Max steps into X Axis
const int maxYSteps = SPR * 1.2;                              // Max steps into Y Axis
const int initRotationsX = maxXSteps / 4.8;                   // Rotations in the x axis needed to get into the first slot
const int initRotationsY = 0;                                 // Rotations in the y axis needed to get into the first slot
const int stepsPerSlotX = (maxXSteps - initRotationsX) / 1.9; // Steps between slots in the x axis
const int stepsPerSlotY = maxYSteps;                          // Steps between slots in the y axis

const int colors[2][3] = {{1, 2, 3}, {4, 5, 6}};  // 1 -> White; 2 -> Red; 3 -> Yellow; 4 -> Orange; 5 -> Green; 6 -> Blue;

void setup() {
  dropperServo.attach(SERVO);
  dropperServo.write(MIN_DEG);
  xAxisStepper.setSpeed(RPM);
  yAxisStepper.setSpeed(RPM);
  Serial.begin(9600);
  delay(2000);
  // CALIBRATION:
//  while(true){
//    readColor();
//  }
  // END CALIBRATION
}

void loop() {
  int slot = 0;
  int xCoord = 0;
  int yCoord = 0;
  Serial.println("==== CHOOSE A MODE ====");
  Serial.println("0 -> SERIAL CONTROL");
  Serial.println("1 -> COLOR CONTROL");
  while(Serial.available() == 0);
  if(Serial.available() > 0) {
    int num = 0;
    int mode = Serial.parseInt();
    if(mode == 0) {
//      Serial.println("CHOOSE A NUMBER BETWEEN 1 AND 6:");
//      Reading:
//      int numVal = Serial.parseInt();
//      num = constrain(numVal, 0, 6);
//      while(num == 0) { goto Reading; }
//      Serial.print("NUM: ");
//      Serial.println(num);
    } else if(mode == 1) {
      num = avgColorReading();
    }
    if(num > 0) {
      slot = num;
    }
  }
  if(slot > 0) {
    for(int i = 0; i < 2; i++) {
//    Serial.print("Y: ");
//    Serial.println(i + 1);
    for(int j = 0; j < 3; j++) {
//      Serial.print("X: ");
//      Serial.println(j + 1);
//      Serial.println(colors[i][j]);
      if(colors[i][j] == slot) {
        xCoord = j + 1;
        yCoord = i + 1;
        goto calculateAxisRotations;
      }
    }
  }
  calculateAxisRotations:
  Serial.print("X COORD: ");
  Serial.println(xCoord);
  Serial.print("Y COORD: ");
  Serial.println(yCoord);
  int xRotations = rotationsToSlot('X', xCoord, initRotationsX);
  int yRotations = rotationsToSlot('Y', yCoord, initRotationsY);
  Serial.print("X ROTATIONS: ");
  Serial.println(xRotations);
  Serial.println("====================");
  Serial.print("Y ROTATIONS: ");
  Serial.println(yRotations);
  delay(1000);
  xAxisStepper.step(xRotations);
  yAxisStepper.step(-yRotations);
  drop(dropperServo);
  yAxisStepper.step(yRotations);
  xAxisStepper.step(-xRotations);
  }
}

int serialMode() {
  
}

int readColor() {
  int colorVal = analogRead(LDR);
  Serial.print("VALUE: ");
  Serial.println(colorVal);
  int color = 0;
  switch(colorVal) {
    case 126 ... 134:
      Serial.println("WHITE");
      color = 1;
    break;
    case 65 ... 75:
      Serial.println("RED");
      color = 2;
      break;
     case 99 ... 110:
      Serial.println("YELLOW");
      color = 3;
    break;
    case 82 ... 98:
      Serial.println("ORANGE");
      color = 4;
    break;
    case 44 ... 50:
      Serial.println("GREEN");
      color = 5;
    break;
    case 36 ... 42:
      Serial.println("BLUE");
      color = 6;
    break;
    default:
      Serial.println("NONE");
    break;
  }
  delay(200);
  return color;
}

int avgColorReading() {
  int colorAvg = 0;
  Serial.println("READING COLOR, PLEASE WAIT...");
  for(int i = 0; i < 20; i++) {
    Serial.print("Reading #");
    Serial.println(i + 1);
    colorAvg += readColor();
  }
  colorAvg = colorAvg / 20;
  Serial.println("DONE!");
  Serial.print("COLOR NUM: ");
  Serial.println(colorAvg);
  return colorAvg;
}

int rotationsToSlot(char axis, int coord, int startRotations) {
  int rotations = startRotations;
  if(axis == 'X') {
    rotations += stepsPerSlotX * (coord - 1);
  } else if (axis == 'Y') {
    rotations += stepsPerSlotY * (coord - 1);
  }
  return rotations;
}

void drop(Servo servo) {
  servo.write(MAX_DEG);
  delay(2000);
  servo.write(MIN_DEG);
}

void fullLap() {
  Serial.println("FULL LAP!");
  xAxisStepper.step(initRotationsX);
  for(int i = 0; i < 2; i++) {
    if((i + 1) % 2 == 0) {
      yAxisStepper.step(stepsPerSlotY); 
    } else {
      yAxisStepper.step(-stepsPerSlotY);
    }
    xAxisStepper.step(stepsPerSlotX);
  }
  yAxisStepper.step(-stepsPerSlotY);
  Serial.println("DONE!");
  yAxisStepper.step(maxYSteps);
  xAxisStepper.step(-maxXSteps);
}
