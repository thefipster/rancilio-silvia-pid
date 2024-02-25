#include <MqttInterface.h>

MqttInterface::MqttInterface(PubSubClient *client, StateModel *state)
{
    this->client = client;
    this->state = state;
}

void MqttInterface::Subscribe()
{
    client->subscribe(TARGET_SUB_TOPIC);
    client->subscribe(COLDSTART_SUB_TOPIC);
    client->subscribe(WINDOW_SUB_TOPIC);
    client->subscribe(CYCLE_SUB_TOPIC);
    client->subscribe(P_SUB_TOPIC);
    client->subscribe(I_SUB_TOPIC);
    client->subscribe(D_SUB_TOPIC);
}

void MqttInterface::Loop()
{
    if (millis() - loopStart > intervalInMs)
        publish();
}

void MqttInterface::publish()
{
    if (state->boilerTemp != oldState.boilerTemp)
        client->publish(BOILER_TOPIC, String(state->boilerTemp, 3).c_str());

    if (state->headTemp != oldState.headTemp)
        client->publish(HEAD_TOPIC, String(state->headTemp, 3).c_str());

    if (state->pidControl != oldState.pidControl)
        client->publish(PID_TOPIC, String(state->pidControl, 3).c_str());

    if (state->heaterState != oldState.heaterState)
        client->publish(HEATER_TOPIC, state->heaterState ? "on" : "off");

    if (state->setpoint != oldState.setpoint)
        client->publish(TARGET_PUB_TOPIC, String(state->setpoint, 3).c_str());

    if (state->coldstart != oldState.coldstart)
        client->publish(COLDSTART_PUB_TOPIC, String(state->coldstart, 3).c_str());

    if (state->windowMs != oldState.windowMs)
        client->publish(WINDOW_PUB_TOPIC, String(state->windowMs, 3).c_str());

    if (state->dutyCycle != oldState.dutyCycle)
        client->publish(CYCLE_PUB_TOPIC, String(state->dutyCycle, 3).c_str());

    if (state->kP != oldState.kP)
        client->publish(P_PUB_TOPIC, String(state->kP, 3).c_str());

    if (state->kI != oldState.kI)
        client->publish(I_PUB_TOPIC, String(state->kI, 3).c_str());

    if (state->kD != oldState.kD)
        client->publish(D_PUB_TOPIC, String(state->kD, 3).c_str());

    oldState.copyFrom(*state);

    loopStart = millis();
}

void MqttInterface::Callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
        message += (char)payload[i];

    double value = atof(message.c_str());

    if (strcmp(topic, TARGET_SUB_TOPIC) == 0)
        state->setpoint = value;

    if (strcmp(topic, COLDSTART_SUB_TOPIC) == 0)
        state->coldstart = value;

    if (strcmp(topic, WINDOW_SUB_TOPIC) == 0)
        state->windowMs = value;

    if (strcmp(topic, CYCLE_SUB_TOPIC) == 0)
        state->dutyCycle = value;

    if (strcmp(topic, P_SUB_TOPIC) == 0)
        state->kP = value;

    if (strcmp(topic, I_SUB_TOPIC) == 0)
        state->kI = value;

    if (strcmp(topic, D_SUB_TOPIC) == 0)
        state->kD = value;
}