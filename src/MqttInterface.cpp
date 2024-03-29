#include <MqttInterface.h>

MqttInterface::MqttInterface(PubSubClient *client, ControlModel *controls, PublishModel *publish)
{
    this->client = client;
    this->controls = controls;
    this->model = publish;
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
    if (model->boilerTemp != oldModel.boilerTemp)
        client->publish(BOILER_TOPIC, String(model->boilerTemp, 3).c_str());

    if (model->headTemp != oldModel.headTemp)
        client->publish(HEAD_TOPIC, String(model->headTemp, 3).c_str());

    if (model->pidControl != oldModel.pidControl)
        client->publish(PID_TOPIC, String(model->pidControl, 3).c_str());

    if (model->heaterState != oldModel.heaterState)
        client->publish(HEATER_TOPIC, model->heaterState ? "on" : "off");

    if (controls->setpoint != oldControls.setpoint)
        client->publish(TARGET_PUB_TOPIC, String(controls->setpoint, 3).c_str());

    if (controls->coldstart != oldControls.coldstart)
        client->publish(COLDSTART_PUB_TOPIC, String(controls->coldstart, 3).c_str());

    if (controls->windowMs != oldControls.windowMs)
        client->publish(WINDOW_PUB_TOPIC, String(controls->windowMs, 3).c_str());

    if (controls->dutyCycle != oldControls.dutyCycle)
        client->publish(CYCLE_PUB_TOPIC, String(controls->dutyCycle, 3).c_str());

    if (controls->kP != oldControls.kP)
        client->publish(P_PUB_TOPIC, String(controls->kP, 3).c_str());

    if (controls->kI != oldControls.kI)
        client->publish(I_PUB_TOPIC, String(controls->kI, 3).c_str());

    if (controls->kD != oldControls.kD)
        client->publish(D_PUB_TOPIC, String(controls->kD, 3).c_str());

    oldControls.copyFrom(*controls);
    oldModel.copyFrom(*model);

    loopStart = millis();
}

void MqttInterface::Callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
        message += (char)payload[i];

    double value = atof(message.c_str());

    if (strcmp(topic, TARGET_SUB_TOPIC) == 0)
        controls->setpoint = value;

    if (strcmp(topic, COLDSTART_SUB_TOPIC) == 0)
        controls->coldstart = value;

    if (strcmp(topic, WINDOW_SUB_TOPIC) == 0)
        controls->windowMs = value;

    if (strcmp(topic, CYCLE_SUB_TOPIC) == 0)
        controls->dutyCycle = value;

    if (strcmp(topic, P_SUB_TOPIC) == 0)
        controls->kP = value;

    if (strcmp(topic, I_SUB_TOPIC) == 0)
        controls->kI = value;

    if (strcmp(topic, D_SUB_TOPIC) == 0)
        controls->kD = value;
}