#include "pixy2.h"

Pixy2 pixy;
volatile int redFlagSize = 0;
volatile int blueFlagSize = 0;
volatile int redFlagX = -1;

void setupPixy() {
  pixy.init();
  pixy.setLamp(1, 0);
}

void pixyTask(void *pvParameters) {
  (void) pvParameters;
  static int noBlocksCount = 0;
  static int pixyRestartCount = 0;

  while (1) {
    pixy.ccc.getBlocks();
    redFlagSize = 0;
    blueFlagSize = 0;

    if (pixy.ccc.numBlocks == 0) {
      noBlocksCount++;
      if (noBlocksCount > 5 && pixyRestartCount < 3) {
        Serial.println("Reiniciando Pixy...");
        pixy.init();
        pixyRestartCount++;
        noBlocksCount = 0;
      }
    }

    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      auto &b = pixy.ccc.blocks[i];
      if (b.m_signature == 1) {
        redFlagSize = b.m_height;
        redFlagX = b.m_x;
        Serial.println("Bandera ROJA detectada");
      } else if (b.m_signature == 2) {
        blueFlagSize = b.m_height;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

int trackObject(const char* sigName) {
  if (strcmp(sigName, "cilindro_rojo") == 0) return redFlagSize;
  if (strcmp(sigName, "cilindro_azul") == 0) return blueFlagSize;
  return 0;
}