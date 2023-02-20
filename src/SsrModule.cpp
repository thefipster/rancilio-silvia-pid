#include "SsrModule.h"

u_int8_t ssrPin;

void setupSsr(u_int8_t pin)
{
  ssrPin = pin;
  pinMode(ssrPin, OUTPUT);
}

void setSsr(bool state)
{
  digitalWrite(ssrPin, state ? 1 : 0);
}