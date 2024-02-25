#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <Defaults.h>
#include <Pins.h>

#include <StateModel.h>

#include <Arduino.h>
#include <PID_v1.h>
#include <TsicSensor.h>

class PidController
{

public:
    PidController(StateModel *state);

    void Loop();

private:
    PID *pid;
    double input;
    double output;
    double setpoint;

    unsigned long windowStart = 0;

    TsicSensor *boilerSensor;
    TsicSensor *groupheadSensor;

    StateModel *state;

    void readTemps();
    void updateParameters();
    void evaluate();
};

#endif