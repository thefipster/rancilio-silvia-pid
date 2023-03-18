#include <Controller.h>

Controller::Controller(ControlModel* controls, PublishModel* publish)
{
  setpoint = SETPOINT;

  this->controls = controls;
  this->publish = publish;

  pinMode(SSR_PIN, OUTPUT);

  boiler = TsicSensor::create(TSIC_BOILER_PIN, TsicExternalVcc, TsicType::TSIC_306);
  grouphead = TsicSensor::create(TSIC_HEAD_PIN, TsicExternalVcc, TsicType::TSIC_306);

  pid = new PID(&input, &output, &setpoint, KP, KI, KD, DIRECT);
}

void Controller::Loop()
{
  readTemps();
  updateParameters();
  pid->Compute();
  evaluate();
  digitalWrite(SSR_PIN, publish->heaterState ? 1 : 0);
}

void Controller::readTemps()
{
  if (boiler->newValueAvailable())
    publish->boilerTemp = boiler->getTempCelsius();

  if (grouphead->newValueAvailable())
    publish->headTemp = grouphead->getTempCelsius();
}

void Controller::updateParameters()
{
  input = publish->boilerTemp;
  setpoint = controls->setpoint;

  pid->SetOutputLimits(0, controls->windowMs);
  pid->SetMode(AUTOMATIC);
  pid->SetTunings(controls->kP, controls->kI, controls->kD);
}

void Controller::evaluate()
{
  long timeDelta = millis() - loopStart;
  if (timeDelta > controls->windowMs / controls->dutyCycle)
    loopStart = millis();

  publish->pidControl = input < controls->coldstart ? 1 : output / controls->windowMs;
  publish->heaterState = input < controls->coldstart ? true : output > timeDelta;
}