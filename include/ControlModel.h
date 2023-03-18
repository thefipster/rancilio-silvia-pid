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

    void copyFrom(ControlModel model)
    {
        setpoint = model.setpoint;
        windowMs = model.windowMs;
        coldstart = model.coldstart;
        dutyCycle = model.dutyCycle;
        kP = model.kP;
        kI = model.kI;
        kD = model.kD;
    }
};

#endif