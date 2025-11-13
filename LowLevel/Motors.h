#pragma once
#include <Arduino.h>

void setMotorPWM(int ENA, int ENB, int ENC, int END, int speed);
void stopMotors(uint8_t inPins[8]);
void moveForward(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed);
void moveBackward(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed);
void turnLeft(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed);
void turnRight(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed);