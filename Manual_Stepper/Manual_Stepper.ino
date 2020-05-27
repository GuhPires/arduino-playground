/*  
 *  MANUAL STEPPER
 *  
 *  OBS: - Do not plug the Arduino via Serial, remove pins 0, 1 and comment every line related these pins (AKA LED_X and LED_Y lines) if you do so.
 *       - Unplug pins 0 and 1 everytime you upload the code.
 *       - ALWAYS USE AN EXTERNAL POWER SOURCE.
 *  
 *  Author: Gustavo G. Pires
 *  GitHub: GuhPires
*/

#include <Stepper.h>

#define AXIS_BTN 6    // Button to change the selected axis
#define MANUAL_BTN 12 // Button to turn ON/OFF manual mode
#define LED_X 0       // LED indicating that y axis is selected
#define LED_Y 1       // LED indicating that y axis is selected
#define LED 13        // LED indicating manual mode
#define POT A0        // Potentiometer to control rotations
#define SPR 2048      // Steps per Revolution (28BYJ-48)
#define RPM 15        // Max Stepper RPM (28BYJ-48)

//                           Steps-IN1-IN3-IN2-IN4
Stepper yAxisStepper = Stepper(SPR, 8, 10, 9, 11);  // Initializing Y Axis Stepper
//                           Steps-IN1-IN3-IN2-IN4
Stepper xAxisStepper = Stepper(SPR, 2, 4, 3, 5);    // Initializing X Axis Stepper

const int maxYSteps = SPR * 1.2;    // Max steps into Y Axis
const int maxXSteps = SPR * 3.2;    // Max steps into X Axis

int currManualState = 0;
int lstManualState = 0;
int manualState = 0;

int currSelectedAxis = 0;
int lstSelectedAxis = 0;
int selectedAxis = 0;

int currPotVal = 0;
int lstPotVal = 0;

void setup() {
  pinMode(AXIS_BTN, INPUT);
  pinMode(MANUAL_BTN, INPUT);
  pinMode(LED_X, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED, OUTPUT);
  yAxisStepper.setSpeed(RPM);
  xAxisStepper.setSpeed(RPM);
//  Serial.begin(9600);
}

void loop() {
  manualState = DRE(MANUAL_BTN, currManualState, lstManualState, manualState);      // Check for manual mode state
  digitalWrite(LED, manualState);
  if(manualState){
    selectedAxis = DRE(AXIS_BTN, currSelectedAxis, lstSelectedAxis, selectedAxis);  // Check for current selected axis
//    Serial.print("SELECTED AXIS: ");
//    Serial.println(selectedAxis);
    if(selectedAxis == 0) {
      digitalWrite(LED_X, HIGH);
      digitalWrite(LED_Y, LOW);
      int xSteps = PotToStepConvertion(maxXSteps);
      xAxisStepper.step(xSteps);
    } else if(selectedAxis == 1) {
      digitalWrite(LED_X, LOW);
      digitalWrite(LED_Y, HIGH);
      int ySteps = PotToStepConvertion(maxYSteps);
      yAxisStepper.step(-ySteps);
    }
  } else {
    digitalWrite(LED_X, LOW);
    digitalWrite(LED_Y, LOW);
  }
}

// Detect button Rising Edge Function
int DRE(int button, int curr, int lst, int state) {
  curr = digitalRead(button);
  if(curr != lst) {
    if(curr == HIGH) {
      state = !state;
    }
    delay(100);
  }
  lst = curr;
  return state;
}

// Pot rotations to Steps conversion Function
int PotToStepConvertion(int maxSteps) {
  currPotVal = analogRead(POT);
//  Serial.print("ROTATIONS: ");
//  Serial.println(currPotVal);
  int steps = currPotVal - lstPotVal;
  if(abs(steps) >= 5) {
    steps = map(steps, 0, 1023, 0, maxSteps); 
  } else {
    steps = 0;
  }
//  Serial.print("STEPS: ");
//  Serial.println(steps);
  lstPotVal = currPotVal;
  delay(50);
  return steps;
}
 
