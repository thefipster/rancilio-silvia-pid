#include <MqttHandler.h>

MqttHandler::MqttHandler(PubSubClient *client, ControlModel *controls, PublishModel *publish)
{
    this->client = client;
    this->controls = controls;
    this->model = publish;
}

bool MqttHandler::Connect()
{
    client->setServer(MQTT_IP, MQTT_PORT);
    return client->connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
}

void MqttHandler::Subscribe()
{
    client->subscribe(MQTT_TARGET_SUB_TOPIC);
    client->subscribe(MQTT_COLDSTART_SUB_TOPIC);
    client->subscribe(MQTT_WINDOW_SUB_TOPIC);
    client->subscribe(MQTT_CYCLE_SUB_TOPIC);
    client->subscribe(MQTT_P_SUB_TOPIC);
    client->subscribe(MQTT_I_SUB_TOPIC);
    client->subscribe(MQTT_D_SUB_TOPIC);
}

bool MqttHandler::Loop()
{
    bool ok = client->loop();

    if (!client->connected())
        ok = reconnect();

    if (!ok)
        return false;

    if (millis() - heartbeatStart > heartbeatIntervalInMs)
        heartbeat();

    if (millis() - loopStart > publishIntervalInMs)
        publish();

    return true;
}

bool MqttHandler::reconnect()
{
    int tries = 0;
    if (!client->connected())
    {
        while (tries < MQTT_RETRY_LIMIT && !client->connected())
        {
            client->connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
            uint8_t timeout = 5;
            while (timeout && (!client->connected()))
            {
                timeout--;
                delay(500);
            }
        }
    }

    return tries < MQTT_RETRY_LIMIT ? true : false;
}

void MqttHandler::heartbeat()
{
    heartbeatStart = millis();
    client->publish(MQTT_STATE_TOPIC, "on");
}

void MqttHandler::publish()
{
    loopStart = millis();

    if (model->boilerTemp != oldModel.boilerTemp)
        client->publish(MQTT_BOILER_TOPIC, String(model->boilerTemp, 3).c_str());

    if (model->headTemp != oldModel.headTemp)
        client->publish(MQTT_HEAD_TOPIC, String(model->headTemp, 3).c_str());

    if (model->pidControl != oldModel.pidControl)
        client->publish(MQTT_PID_TOPIC, String(model->pidControl, 3).c_str());

    if (model->heaterState != oldModel.heaterState)
        client->publish(MQTT_HEATER_TOPIC, model->heaterState ? "on" : "off");

    if (controls->setpoint != oldControls.setpoint)
        client->publish(MQTT_TARGET_PUB_TOPIC, String(controls->setpoint, 3).c_str());

    if (controls->coldstart != oldControls.coldstart)
        client->publish(MQTT_COLDSTART_PUB_TOPIC, String(controls->coldstart, 3).c_str());

    if (controls->windowMs != oldControls.windowMs)
        client->publish(MQTT_WINDOW_PUB_TOPIC, String(controls->windowMs, 3).c_str());

    if (controls->dutyCycle != oldControls.dutyCycle)
        client->publish(MQTT_CYCLE_PUB_TOPIC, String(controls->dutyCycle, 3).c_str());

    if (controls->kP != oldControls.kP)
        client->publish(MQTT_P_PUB_TOPIC, String(controls->kP, 3).c_str());

    if (controls->kI != oldControls.kI)
        client->publish(MQTT_I_PUB_TOPIC, String(controls->kI, 3).c_str());

    if (controls->kD != oldControls.kD)
        client->publish(MQTT_D_PUB_TOPIC, String(controls->kD, 3).c_str());

    oldControls.copyFrom(*controls);
    oldModel.copyFrom(*model);
}

void MqttHandler::Callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
        message += (char)payload[i];

    double value = atof(message.c_str());

    if (strcmp(topic, MQTT_TARGET_SUB_TOPIC) == 0)
        controls->setpoint = value;

    if (strcmp(topic, MQTT_COLDSTART_SUB_TOPIC) == 0)
        controls->coldstart = value;

    if (strcmp(topic, MQTT_WINDOW_SUB_TOPIC) == 0)
        controls->windowMs = value;

    if (strcmp(topic, MQTT_CYCLE_SUB_TOPIC) == 0)
        controls->dutyCycle = value;

    if (strcmp(topic, MQTT_P_SUB_TOPIC) == 0)
        controls->kP = value;

    if (strcmp(topic, MQTT_I_SUB_TOPIC) == 0)
        controls->kI = value;

    if (strcmp(topic, MQTT_D_SUB_TOPIC) == 0)
        controls->kD = value;
}