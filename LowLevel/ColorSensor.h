#pragma once
#include <Arduino.h>

enum Color { RED, BLUE, YELLOW, BLACK, NONE };

void setupColorSensor(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t outPin);
uint32_t readColor(char channel, uint8_t s2, uint8_t s3, uint8_t outPin);
Color identifyColor(uint32_t red, uint32_t green, uint32_t blue);