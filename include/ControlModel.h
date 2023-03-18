#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

#include <Defaults.h>

struct ControlModel
{
    double setpoint = SETPOINT;
    double windowMs = WINDOW;
    double coldstart = COLDSTART;
    double dutyCycle = DUTYCYCLE;
    double kP = KP;
    double kI = KI;
    double kD = KD;
};

#endif