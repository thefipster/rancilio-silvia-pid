#include <PidController.h>

PidController::PidController(ControlModel *controlModel, PublishModel *publishModel)
{
  setpoint = SETPOINT;

  this->controlModel = controlModel;
  this->publishModel = publishModel;

  pinMode(SSR_PIN, OUTPUT);

  boilerSensor = TsicSensor::create(TSIC_BOILER_PIN, TsicExternalVcc, TsicType::TSIC_306);
  groupheadSensor = TsicSensor::create(TSIC_HEAD_PIN, TsicExternalVcc, TsicType::TSIC_306);

  pid = new PID(&input, &output, &setpoint, KP, KI, KD, DIRECT);
  pid->SetMode(AUTOMATIC);
}

void PidController::Loop()
{
  readTemps();
  updateParameters();
  pid->Compute();
  evaluate();
  digitalWrite(SSR_PIN, publishModel->heaterState ? 1 : 0);
}

void PidController::readTemps()
{
  if (boilerSensor->newValueAvailable())
    publishModel->boilerTemp = boilerSensor->getTempCelsius();

  if (groupheadSensor->newValueAvailable())
    publishModel->headTemp = groupheadSensor->getTempCelsius();
}

void PidController::updateParameters()
{
  input = publishModel->boilerTemp;
  setpoint = controlModel->setpoint;

  pid->SetOutputLimits(0, controlModel->windowMs);
  pid->SetTunings(controlModel->kP, controlModel->kI, controlModel->kD);
}

void PidController::evaluate()
{
  long timeDelta = millis() - windowStart;
  if (timeDelta > controlModel->windowMs / controlModel->dutyCycle)
    windowStart = millis();

  publishModel->pidControl = input < controlModel->coldstart ? -1 : output / controlModel->windowMs;
  publishModel->heaterState = input < controlModel->coldstart ? true : output > timeDelta;
}