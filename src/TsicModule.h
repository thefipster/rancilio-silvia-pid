#ifndef TsicModule_h
#define TsicModule_h

#include <Arduino.h>
#include <TsicSensor.h>

void setupTsic(u_int8_t pin1, u_int8_t pin2);
float getTsicOneTemp();
float getTsicTwoTemp();

#endif