#include <Arduino.h>
#include "motors.h"
#include "ultrasonic.h"
#include "colorSensor.h"
#include "pixy2.h"
#include "servos.h"

// ======== Pines ========
#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25
#define IN5 26
#define IN6 27
#define IN7 28
#define IN8 29

#define ENA 5
#define ENB 6
#define ENC 9
#define END 10

#define TRIG_PIN 30
#define ECHO_PIN 31

#define S0 32
#define S1 33
#define S2 34
#define S3 35
#define OUT_PIN 36

#define GRIPPER_SERVO_PIN 37
#define LIFT_SERVO_PIN    38

#define GRIPPER_OPEN_ANGLE   -375
#define GRIPPER_CLOSED_ANGLE 360
#define LIFT_DOWN_ANGLE      0
#define LIFT_UP_ANGLE        180

uint8_t motorPins[8] = {IN1, IN2, IN3, IN4, IN5, IN6, IN7, IN8};

// ======== Tareas FreeRTOS externas ========
extern void colorTask(void *pvParameters);
extern void pixyTask(void *pvParameters);
extern void roverTask(void *pvParameters);

void setup() {
  Serial.begin(9600);

  // Pines de motores
  for (int i = 0; i < 8; i++) pinMode(motorPins[i], OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(ENC, OUTPUT); pinMode(END, OUTPUT);

  // Sensor de color
  setupColorSensor(S0, S1, S2, S3, OUT_PIN);

  // Servos
  setupServos(GRIPPER_SERVO_PIN, LIFT_SERVO_PIN,
              GRIPPER_CLOSED_ANGLE, GRIPPER_OPEN_ANGLE,
              LIFT_DOWN_ANGLE);

  // Pixy2
  setupPixy();

  // Sensor ultrasonico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Tareas FreeRTOS
  xTaskCreate(colorTask, "ColorTask", 512, NULL, 3, NULL);
  xTaskCreate(pixyTask, "PixyTask", 512, NULL, 1, NULL);
  xTaskCreate(roverTask, "RoverTask", 512, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop() {
  // vacío: la lógica corre en FreeRTOS
}