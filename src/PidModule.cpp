#include "PidModule.h"

double pidInput, pidOutput, pidSetpoint, pidWindowSizeInMs, pidColdstart, pidDutyCycle;

double kP = 33;
double kI = 255;
double kD = 0;

unsigned long windowStartTime;

PID myPID(&pidInput, &pidOutput, &pidSetpoint, kP, kI, kD, DIRECT);

void setupPid(int windowSize, double setPoint, double coldstart, double dutycycle)
{
    pidColdstart = coldstart;
    pidWindowSizeInMs = windowSize;
    pidSetpoint = setPoint;
    pidDutyCycle = dutycycle;
    windowStartTime = millis();
    myPID.SetOutputLimits(0, pidWindowSizeInMs);
    myPID.SetMode(AUTOMATIC);
}

void setPidTuning(double kP, double kI, double kD)
{
    myPID.SetTunings(kP, kI, kD);
}

void setPidParameters(double setpoint, double coldstart, double dutywindow, double dutycycle)
{
    pidColdstart = coldstart;
    pidWindowSizeInMs = dutywindow;
    pidSetpoint = setpoint;
    pidDutyCycle = dutycycle;
}

double getPidOutput()
{
    if (pidInput < pidColdstart)
    {
        return 1;
    }

    return pidOutput / pidWindowSizeInMs;
}

double getP()
{
    return myPID.GetKp();
}

double getI()
{
    return myPID.GetKi();
}

double getD()
{
    return myPID.GetKd();
}

bool updatePid(double input)
{
    pidInput = input;
    myPID.Compute();

    long timeDelta = millis() - windowStartTime;
    if (timeDelta > pidWindowSizeInMs / pidDutyCycle)
    {
        windowStartTime = millis();
    }

    if (pidInput < pidColdstart)
    {
        return true;
    }

    return pidOutput > timeDelta;
}
