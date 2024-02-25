#ifndef STATEMODEL_H
#define STATEMODEL_H

#include <Defaults.h>

struct StateModel
{
    double boilerTemp;
    double headTemp;
    double pidControl;
    bool heaterState;

    double setpoint = SETPOINT;
    double windowMs = WINDOW;
    double coldstart = COLDSTART;
    double dutyCycle = DUTYCYCLE;
    double kP = KP;
    double kI = KI;
    double kD = KD;

    void copyFrom(StateModel model)
    {
        boilerTemp = model.boilerTemp;
        headTemp = model.headTemp;
        pidControl = model.pidControl;
        heaterState = model.heaterState;

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