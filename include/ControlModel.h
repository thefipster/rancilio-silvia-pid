#ifndef CONTROLMODEL_H
#define CONTROLMODEL_H

struct ControlModel
{
    double setpoint = 92;
    double windowMs = 1000;
    double coldstart = 85;
    double dutyCycle = 0.2;
    double kP = 33;
    double kI = 255;
    double kD = 0;
};

#endif