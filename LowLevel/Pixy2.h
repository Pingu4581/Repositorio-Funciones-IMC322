#pragma once
#include <Pixy2.h>

extern Pixy2 pixy;
extern volatile int redFlagSize;
extern volatile int blueFlagSize;
extern volatile int redFlagX;

void setupPixy();
void pixyTask(void *pvParameters);
int trackObject(const char* sigName);