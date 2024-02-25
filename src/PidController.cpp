#include <PidController.h>

PidController::PidController(StateModel *state)
{
  setpoint = SETPOINT;

  this->state = state;

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
  digitalWrite(SSR_PIN, state->heaterState ? 1 : 0);
}

void PidController::readTemps()
{
  if (boilerSensor->newValueAvailable())
    state->boilerTemp = boilerSensor->getTempCelsius();

  if (groupheadSensor->newValueAvailable())
    state->headTemp = groupheadSensor->getTempCelsius();
}

void PidController::updateParameters()
{
  input = state->boilerTemp;
  setpoint = state->setpoint;

  pid->SetOutputLimits(0, state->windowMs);
  pid->SetTunings(state->kP, state->kI, state->kD);
}

void PidController::evaluate()
{
  long timeDelta = millis() - windowStart;
  if (timeDelta > state->windowMs / state->dutyCycle)
    windowStart = millis();

  state->pidControl = input < state->coldstart ? -1 : output / state->windowMs;
  state->heaterState = input < state->coldstart ? true : output > timeDelta;
}