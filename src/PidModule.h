#ifndef PidModule_h
#define PidModule_h

#include <Arduino.h>
#include <PID_v1.h>

void setupPid(int windowSize, double setPoint, double coldstart, double dutycycle);
void setPidTuning(double kP, double kI, double kD);
double getPidOutput();
double getP();
double getI();
double getD();
bool updatePid(double input);
void setPidParameters(double setpoint, double coldstart, double dutywindow, double dutycycle);

#endif