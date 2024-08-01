#include <Servo.h>

Servo baseServo;
Servo armServo;
Servo clawServo;

int baseServoPin = 9;  // Connect the base servo to pin 9
int armServoPin = 10;  // Connect the arm servo to pin 10
int clawServoPin = 11; // Connect the claw servo to pin 11

int basePosition = 90; // Initial position for base servo
int armPosition = 90;  // Initial position for arm servo
int clawPosition = 90; // Initial position for claw servo

int baseTarget = 90;
int armTarget = 90;
int clawTarget = 90;

const int stepDelay = 20;  // Delay between steps in milliseconds
const int stepSize = 2;    // Degrees to move per step

void setup() {
  Serial.begin(9600);
  baseServo.attach(baseServoPin);
  armServo.attach(armServoPin);
  clawServo.attach(clawServoPin);
  baseServo.write(basePosition); // Set initial position
  armServo.write(armPosition);   // Set initial position
  clawServo.write(clawPosition); // Set initial position
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'L') {
      baseTarget = constrain(baseTarget - 30, 0, 180); // Move left by 10 degrees, within range
    } else if (command == 'R') {
      baseTarget = constrain(baseTarget + 30, 0, 180); // Move right by 10 degrees, within range
    } else if (command == 'U') {
      armTarget = constrain(armTarget + 30, 0, 90);  // Move up by 10 degrees, within range
    } else if (command == 'D') {
      armTarget = constrain(armTarget - 30, 0, 90);  // Move down by 10 degrees, within range
    } else if (command == 'O') {
      clawTarget = constrain(clawTarget + 30, 0, 60);  // Open claw
    } else if (command == 'C') {
      clawTarget = constrain(clawTarget - 30, 0, 60);  // Close claw
    }
  }

  // Gradually move base servo to the target position
  if (basePosition != baseTarget) {
    if (basePosition < baseTarget) {
      basePosition = min(basePosition + stepSize, baseTarget);
    } else {
      basePosition = max(basePosition - stepSize, baseTarget);
    }
    baseServo.write(basePosition);
  }

  // Gradually move arm servo to the target position
  if (armPosition != armTarget) {
    if (armPosition < armTarget) {
      armPosition = min(armPosition + stepSize, armTarget);
    } else {
      armPosition = max(armPosition - stepSize, armTarget);
    }
    armServo.write(armPosition);
  }

  // Gradually move claw servo to the target position
  if (clawPosition != clawTarget) {
    if (clawPosition < clawTarget) {
      clawPosition = min(clawPosition + stepSize, clawTarget);
    } else {
      clawPosition = max(clawPosition - stepSize, clawTarget);
    }
    clawServo.write(clawPosition);
  }

  delay(stepDelay); // Control the speed of the movement
}
