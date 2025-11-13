#pragma once
#include <Servo.h>

extern Servo gripperServo;
extern Servo liftServo;

void setupServos(uint8_t gripperPin, uint8_t liftPin,
                 int gripperClosedAngle, int gripperOpenAngle,
                 int liftDownAngle);

void openGripper(int gripperOpenAngle);
void closeGripper(int gripperClosedAngle);
void liftUp(int liftUpAngle);
void liftDown(int liftDownAngle);