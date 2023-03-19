#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

#include <Defaults.h>
#include <Pins.h>

#include <ControlModel.h>
#include <PublishModel.h>

#include <Arduino.h>
#include <PID_v1.h>
#include <TsicSensor.h>

class PidController
{

public:
    PidController(ControlModel *controls, PublishModel *publish);

    void Loop();

private:
    PID *pid;
    double input;
    double output;
    double setpoint;

    unsigned long windowStart = 0;

    TsicSensor *boilerSensor;
    TsicSensor *groupheadSensor;

    PublishModel *publishModel;
    ControlModel *controlModel;

    void readTemps();
    void updateParameters();
    void evaluate();
};

#endif