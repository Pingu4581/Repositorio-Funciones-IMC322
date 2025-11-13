#include <Arduino.h>
#include "motors.h"
#include "ultrasonic.h"
#include "pixy2.h"
#include "servos.h"

// ======== Estados ========
enum RoverState { SEARCH_YELLOW, SEARCH_RED, SEARCH_BLUE };
RoverState currentState = SEARCH_YELLOW;

// ======== Señales ========
volatile bool blackDetected   = false;
volatile bool yellowDetected  = false;
volatile bool blueDetected    = false;
volatile bool keyDeposited    = false;
volatile bool flagCaptured    = false;

// ======== Movements ========
void forwardS() {
  // velocidad suave
  moveForward(motorPins, ENA, ENB, ENC, END, 40);
}

void forward() {
  moveForward(motorPins, ENA, ENB, ENC, END, 70);
}

void backward() {
  moveBackward(motorPins, ENA, ENB, ENC, END, 70);
}

void turnLeft() {
  turnLeft(motorPins, ENA, ENB, ENC, END, 70);
}

void turnRight() {
  turnRight(motorPins, ENA, ENB, ENC, END, 70);
}

void stopMotors() {
  stopMotors(motorPins);
}

// ======== Lógica principal ========
void roverLogic() {
  if (blackDetected) {
    stopMotors();
    backward();   vTaskDelay(pdMS_TO_TICKS(1000));
    stopMotors(); vTaskDelay(pdMS_TO_TICKS(100));
    turnRight();  vTaskDelay(pdMS_TO_TICKS(1500));
    stopMotors(); vTaskDelay(pdMS_TO_TICKS(100));
    Serial.println("Interrupción global: línea negra detectada");
    blackDetected = false;
    return;
  }


  switch (currentState) {
    case SEARCH_YELLOW:
      if (yellowDetected) {
        stopMotors(); // vTaskDelay(pdMS_TO_TICKS(50));

        gripperServo.write(GRIPPER_OPEN_ANGLE);  // abre pinza
        vTaskDelay(pdMS_TO_TICKS(400));

        backward(); vTaskDelay(pdMS_TO_TICKS(500));
        stopMotors(); vTaskDelay(pdMS_TO_TICKS(200));
        turnRight(); vTaskDelay(pdMS_TO_TICKS(700));

        keyDeposited = true;
        yellowDetected = false;  // ← resetea la señal
        currentState = SEARCH_RED;
        break;

      } else {
          
        for (int i = 0; i < 8; i++) {
            if (blackDetected || yellowDetected) break;
            forward(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));


          for (int i = 0; i < 40; i++) {
            if (blackDetected || yellowDetected) break;
            turnLeft(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

          for (int i = 0; i < 20; i++) {
            if (blackDetected || yellowDetected) break;
            turnRight(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));
      }
      break;

    case SEARCH_RED:
      if (keyDeposited) {
        if (redFlagSize > 30 && redFlagX != -1) {
          long distance = readDistanceCM();
          Serial.print("Distancia a objeto: "); Serial.println(distance);
          Serial.print("Posición horizontal bandera: "); Serial.println(redFlagX);

          int centerX = 160;
          int errorX = redFlagX - centerX;

          if (distance > 4) {
            if (abs(errorX) > 10) {
             if (errorX < 0) {
                turnLeft(); vTaskDelay(pdMS_TO_TICKS(map(abs(errorX), 7, 160, 100, 400)));
              } else {
                turnRight(); vTaskDelay(pdMS_TO_TICKS(map(abs(errorX), 7, 160, 100, 400)));
              }
            } else {
              forwardS(); vTaskDelay(pdMS_TO_TICKS(map(distance, 10, 100, 200, 600)));
            }
            stopMotors(); vTaskDelay(pdMS_TO_TICKS(200));
          }


          // Captura si está cerca
          if (distance <= 4) {
           stopMotors(); vTaskDelay(pdMS_TO_TICKS(200));

           gripperServo.write(GRIPPER_CLOSED_ANGLE); // agarra
           vTaskDelay(pdMS_TO_TICKS(600));

           liftServo.write(LIFT_UP_ANGLE);           // eleva
           vTaskDelay(pdMS_TO_TICKS(100));
           
           backward();  vTaskDelay(pdMS_TO_TICKS(1000));
           stopMotors(); vTaskDelay(pdMS_TO_TICKS(100));
           
            flagCaptured = true;
           currentState = SEARCH_BLUE;
          }
        } else {
          // Búsqueda si no hay bandera
        for (int i = 0; i < 8; i++) {
            if (blackDetected || redFlagSize >30 ) break;
            forwardS(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));


          for (int i = 0; i < 40; i++) {
            if (blackDetected || redFlagSize >30 ) break;
            turnLeft(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

          for (int i = 0; i < 20; i++) {
            if (blackDetected || redFlagSize >30 ) break;
            turnRight(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

        }

        // Pixy actualiza automáticamente en pixyTask()
      }
      break;

    case SEARCH_BLUE:
      if (blueDetected && flagCaptured) {
        stopMotors();
      // liftServo.write(LIFT_DOWN_ANGLE);         // baja
      //  vTaskDelay(pdMS_TO_TICKS(700));

        gripperServo.write(GRIPPER_OPEN_ANGLE);   // suelta
        vTaskDelay(pdMS_TO_TICKS(1000));

        // VICTORY SCREECH!!!!!!!
        backward(); vTaskDelay(pdMS_TO_TICKS(600));
        stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));
        turnRight(); vTaskDelay(pdMS_TO_TICKS(500));
        turnLeft();  vTaskDelay(pdMS_TO_TICKS(500));
        turnRight(); vTaskDelay(pdMS_TO_TICKS(500));
        stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

        // reiniciar ciclo de misión
        keyDeposited = false;
        flagCaptured  = false;
        currentState  = SEARCH_YELLOW;
        gripperServo.write(GRIPPER_CLOSED_ANGLE);
        blueDetected = false;
        // Espera para evitar detección inmediata
        vTaskDelay(pdMS_TO_TICKS(1500));

      } else {

        for (int i = 0; i < 8; i++) {
            if (blackDetected || blueDetected) break;
            forward(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));


          for (int i = 0; i < 40; i++) {
            if (blackDetected || blueDetected) break;
            turnLeft(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

          for (int i = 0; i < 20; i++) {
            if (blackDetected || blueDetected) break;
            turnRight(); vTaskDelay(pdMS_TO_TICKS(100));
          }
          stopMotors(); vTaskDelay(pdMS_TO_TICKS(300));

      }
      break;
  }
}

void roverTask(void *pvParameters) {
  (void) pvParameters;
  while (1) {
    roverLogic();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}