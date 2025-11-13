#include "ColorSensor.h"

void setupColorSensor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outPin) {
  pinMode(s0, OUTPUT); pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT); pinMode(s3, OUTPUT);
  pinMode(outPin, INPUT);
  digitalWrite(s0, HIGH); digitalWrite(s1, LOW);
}

uint32_t readColor(char channel, uint8_t s2, uint8_t s3, uint8_t outPin) {
  if (channel == 'R') { digitalWrite(s2, LOW);  digitalWrite(s3, LOW); }
  if (channel == 'G') { digitalWrite(s2, HIGH); digitalWrite(s3, HIGH); }
  if (channel == 'B') { digitalWrite(s2, LOW);  digitalWrite(s3, HIGH); }
  delay(20);
  return pulseIn(outPin, LOW);
}

Color identifyColor(uint32_t red, uint32_t green, uint32_t blue) {
  float total = red + green + blue;
  if (total > 2100) return BLACK;
  float rN = red / total;
  float gN = green / total;
  float bN = blue / total;
  if (rN < 0.3 && gN < 0.32 && bN > 0.33) return YELLOW;
  if (rN < 0.3 && gN < 0.40 && bN < 0.40) return RED;
  if (bN < 0.27 && rN > 0.4 && gN < 0.30) return BLUE;
  return NONE;
}