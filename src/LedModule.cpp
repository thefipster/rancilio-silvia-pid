#include "LedModule.h"

u_int8_t ledPin;
int ledWindowSizeInMs = 500;
unsigned long ledWindowStartTime;

void setupLed(u_int8_t pin)
{
    ledPin = pin;
    pinMode(ledPin, OUTPUT);
}

void updateLed()
{
  unsigned long now = millis();
  if (now - ledWindowStartTime > ledWindowSizeInMs)
  {
    ledWindowStartTime += ledWindowSizeInMs;
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}
