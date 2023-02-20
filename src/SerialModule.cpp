#include "SerialModule.h"

void setupSerial(int baud)
{
    Serial.begin(baud);
    Serial.println();
    Serial.println();
    Serial.println("Serial connected.");
}
