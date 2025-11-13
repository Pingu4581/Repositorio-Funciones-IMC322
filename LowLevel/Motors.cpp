#include "Motors.h"

void setMotorPWM(int ENA, int ENB, int ENC, int END, int speed) {  // Pide las variables de los pines pwm y el porcentaje de velocidad de 0 a 100
    int speedPWM = map(speed, 0, 100, 0, 255);
    analogWrite(ENA, speedPWM);
    analogWrite(ENB, speedPWM);
    analogWrite(ENC, speedPWM);
    analogWrite(END, speedPWM);
}

void stopMotors(uint8_t inPins[8]) {   // pide los 8 pines para la funcion en forma de: uint8_t inPins[8] = {22, 23, 24, 25, 26, 27, 28, 29};
    for (int i = 0; i < 8; i++) {
        digitalWrite(inPins[i], LOW);
    }
}

void moveForward(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed) {
    setMotorPWM(ENA, ENB, ENC, END, speed);
    digitalWrite(inPins[0], LOW); digitalWrite(inPins[1], HIGH);
    digitalWrite(inPins[2], HIGH); digitalWrite(inPins[3], LOW);
    digitalWrite(inPins[4], HIGH); digitalWrite(inPins[5], LOW);
    digitalWrite(inPins[6], HIGH); digitalWrite(inPins[7], LOW);
}

void moveBackward(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed) {
    setMotorPWM(ENA, ENB, ENC, END, speed);
    digitalWrite(inPins[0], HIGH); digitalWrite(inPins[1], LOW);
    digitalWrite(inPins[2], LOW);  digitalWrite(inPins[3], HIGH);
    digitalWrite(inPins[4], LOW);  digitalWrite(inPins[5], HIGH);
    digitalWrite(inPins[6], LOW);  digitalWrite(inPins[7], HIGH);
}

void turnLeft(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed) {
    setMotorPWM(ENA, ENB, ENC, END, speed);
    digitalWrite(inPins[0], HIGH); digitalWrite(inPins[1], LOW);
    digitalWrite(inPins[2], HIGH); digitalWrite(inPins[3], LOW);
    digitalWrite(inPins[4], LOW);  digitalWrite(inPins[5], HIGH);
    digitalWrite(inPins[6], HIGH); digitalWrite(inPins[7], LOW);
}

void turnRight(uint8_t inPins[8], int ENA, int ENB, int ENC, int END, int speed) {
    setMotorPWM(ENA, ENB, ENC, END, speed);
    digitalWrite(inPins[0], LOW);  digitalWrite(inPins[1], HIGH);
    digitalWrite(inPins[2], LOW);  digitalWrite(inPins[3], LOW);
    digitalWrite(inPins[4], HIGH); digitalWrite(inPins[5], LOW);
    digitalWrite(inPins[6], LOW);  digitalWrite(inPins[7], HIGH);
}