#include "TsicModule.h"

TsicSensor *tsicSensorOne;
TsicSensor *tsicSensorTwo;
float lastTsicOneTemp = 0;
float lastTsicTwoTemp = 0;

void setupTsic(u_int8_t pin1, u_int8_t pin2)
{
    tsicSensorOne = TsicSensor::create(pin1, TsicExternalVcc, TsicType::TSIC_306);
    tsicSensorTwo = TsicSensor::create(pin2, TsicExternalVcc, TsicType::TSIC_306);
}

float getTsicOneTemp()
{
    if (tsicSensorOne->newValueAvailable())
    {
        lastTsicOneTemp = tsicSensorOne->getTempCelsius();
    }

    return lastTsicOneTemp;
}

float getTsicTwoTemp()
{
    if (tsicSensorTwo->newValueAvailable())
    {
        lastTsicTwoTemp = tsicSensorTwo->getTempCelsius();
    }

    return lastTsicTwoTemp;
}
