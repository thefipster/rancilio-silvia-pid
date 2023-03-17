#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <PID_v1.h>
#include <TsicSensor.h>

#include <Pins.h>
#include <Defaults.h>
#include <PublishModel.h>
#include <ControlModel.h>

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

    TsicSensor *boiler;
    TsicSensor *grouphead;

    PublishModel *publish;
    ControlModel *controls;

    void readTemps();
    void updateParameters();
    void evaluate();
};

#endif