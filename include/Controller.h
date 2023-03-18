#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <PID_v1.h>
#include <TsicSensor.h>

#include <ControlModel.h>
#include <PublishModel.h>

#include <Defaults.h>
#include <Pins.h>

class Controller
{

public:
    Controller(ControlModel* controls, PublishModel* publish);
    
    void Loop();

private:
    PID *pid;

    double input;
    double output;
    double setpoint;
    unsigned long loopStart = 0;

    TsicSensor *boilerSensor;
    TsicSensor *groupheadSensor;

    PublishModel *publishModel;
    ControlModel *controlModel;

    void readTemps();
    void updateParameters();
    void evaluate();
};

#endif