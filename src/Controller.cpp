#include <Controller.h>

Controller::Controller(ControlModel *controlModel, PublishModel *publishModel)
{
  setpoint = SETPOINT;

  this->controlModel = controlModel;
  this->publishModel = publishModel;

  pinMode(SSR_PIN, OUTPUT);

  boilerSensor = TsicSensor::create(TSIC_BOILER_PIN, TsicExternalVcc, TsicType::TSIC_306);
  groupheadSensor = TsicSensor::create(TSIC_HEAD_PIN, TsicExternalVcc, TsicType::TSIC_306);

  pid = new PID(&input, &output, &setpoint, KP, KI, KD, DIRECT);
}

void Controller::Loop()
{
  readTemps();
  updateParameters();
  pid->Compute();
  evaluate();
  digitalWrite(SSR_PIN, publishModel->heaterState ? 1 : 0);
}

void Controller::readTemps()
{
  if (boilerSensor->newValueAvailable())
    publishModel->boilerTemp = boilerSensor->getTempCelsius();

  if (groupheadSensor->newValueAvailable())
    publishModel->headTemp = groupheadSensor->getTempCelsius();
}

void Controller::updateParameters()
{
  input = publishModel->boilerTemp;
  setpoint = controlModel->setpoint;

  pid->SetOutputLimits(0, controlModel->windowMs);
  pid->SetMode(AUTOMATIC);
  pid->SetTunings(controlModel->kP, controlModel->kI, controlModel->kD);
}

void Controller::evaluate()
{
  long timeDelta = millis() - loopStart;
  if (timeDelta > controlModel->windowMs / controlModel->dutyCycle)
    loopStart = millis();

  publishModel->pidControl = input < controlModel->coldstart ? 1 : output / controlModel->windowMs;
  publishModel->heaterState = input < controlModel->coldstart ? true : output > timeDelta;
}