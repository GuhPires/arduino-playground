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

const int maxXSteps = SPR * 3.6;                            // Max steps into X Axis
const int maxYSteps = SPR * 1.2;                            // Max steps into Y Axis
const int initRotationsX = maxXSteps / 4.8;                 // Rotations in the x axis needed to get into the first slot
const int initRotationsY = 0;                               // Rotations in the y axis needed to get into the first slot
const int stepsPerSlotX = (maxXSteps - initRotationsX) / 1.9; // Steps between slots in the x axis
const int stepsPerSlotY = maxYSteps;                        // Steps between slots in the y axis

const int colors[2][3] = {{1, 2, 3}, {4, 5, 6}};  // 1 -> White; 2 -> Red; 3 -> Yellow; 4 -> Orange; 5 -> Green; 6 -> Blue;

void setup() {
  dropperServo.attach(SERVO);
  dropperServo.write(MIN_DEG);
  xAxisStepper.setSpeed(RPM);
  yAxisStepper.setSpeed(RPM);
  Serial.begin(9600);
  delay(2000);
  fullLap();
}

void loop() {
  int slot = 0;
  int xCoord = 0;
  int yCoord = 0;
  if(Serial.available() > 0) {
    int numVal = Serial.parseInt();
    int num = constrain(numVal, 0, 6);
    Serial.print("NUM: ");
    Serial.println(num);
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
      Serial.println(colors[i][j]);
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
  }
}

void readColor() {
  int colorVal = analogRead(LDR);
  Serial.print("VALUE: ");
  Serial.println(colorVal);
  switch(colorVal) {
    case 85 ... 92:
      Serial.println("WHITE");
    break;
    case 44 ... 50:
      Serial.println("RED");
      break;
    case 36 ... 42:
      Serial.println("BLUE");
    break;
    case 96 ... 108:
      Serial.println("ORANGE");
    break;
    case 54 ... 58:
      Serial.println("GREEN");
    break;
    case 93 ... 95:
      Serial.println("YELLOW");
    break;
    default:
      Serial.println("NONE");
    break;
  }
  delay(1000);
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
