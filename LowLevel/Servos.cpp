#include "servos.h"

Servo gripperServo;
Servo liftServo;

void setupServos(uint8_t gripperPin, uint8_t liftPin,
                 int gripperClosedAngle, int gripperOpenAngle,
                 int liftDownAngle) {
  gripperServo.attach(gripperPin);
  liftServo.attach(liftPin);
  gripperServo.write(gripperClosedAngle);
  liftServo.write(liftDownAngle);
}

void openGripper(int gripperOpenAngle) {
  gripperServo.write(gripperOpenAngle);
}

void closeGripper(int gripperClosedAngle) {
  gripperServo.write(gripperClosedAngle);
}

void liftUp(int liftUpAngle) {
  liftServo.write(liftUpAngle);
}

void liftDown(int liftDownAngle) {
  liftServo.write(liftDownAngle);
}