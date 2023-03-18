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
    client->subscribe(MQTT_TARGET_TOPIC);
    client->subscribe(MQTT_COLDSTART_TOPIC);
    client->subscribe(MQTT_WINDOW_TOPIC);
    client->subscribe(MQTT_CYCLE_TOPIC);
    client->subscribe(MQTT_P_TOPIC);
    client->subscribe(MQTT_I_TOPIC);
    client->subscribe(MQTT_D_TOPIC);
}

bool MqttHandler::Loop()
{
    bool state = true;

    state = state && client->loop();

    if (!client->connected())
        state = state && reconnect();

    if (millis() - intervalStart > publishIntervalInMs)
        publish();

    return state;
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

void MqttHandler::publish()
{
    intervalStart = millis();

    client->publish(MQTT_BOILER_TOPIC, String(model->boilerTemp, 3).c_str());
    client->publish(MQTT_HEAD_TOPIC, String(model->headTemp, 3).c_str());
    client->publish(MQTT_PID_TOPIC, String(model->pidControl, 3).c_str());
    client->publish(MQTT_HEATER_TOPIC, model->heaterState ? "1" : "0");
    client->publish(MQTT_TARGET_GET_TOPIC, String(controls->setpoint, 3).c_str());
    client->publish(MQTT_COLD_GET_TOPIC, String(controls->coldstart, 3).c_str());
    client->publish(MQTT_WINDOW_GET_TOPIC, String(controls->windowMs, 3).c_str());
    client->publish(MQTT_CYCLE_GET_TOPIC, String(controls->dutyCycle, 3).c_str());
    client->publish(MQTT_P_GET_TOPIC, String(controls->kP, 3).c_str());
    client->publish(MQTT_I_GET_TOPIC, String(controls->kI, 3).c_str());
    client->publish(MQTT_D_GET_TOPIC, String(controls->kD, 3).c_str());

    Serial.print(".");
}

void MqttHandler::Callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
        message += (char)payload[i];

    double value = atof(message.c_str());
    Serial.print(".");
    Serial.print(value);
    Serial.print(".");

    if (strcmp(topic, MQTT_TARGET_TOPIC) == 0)
        controls->setpoint = value;

    if (strcmp(topic, MQTT_COLDSTART_TOPIC) == 0)
        controls->coldstart = value;

    if (strcmp(topic, MQTT_WINDOW_TOPIC) == 0)
        controls->windowMs = value;

    if (strcmp(topic, MQTT_CYCLE_TOPIC) == 0)
        controls->dutyCycle = value;

    if (strcmp(topic, MQTT_P_TOPIC) == 0)
        controls->kP = value;

    if (strcmp(topic, MQTT_I_TOPIC) == 0)
        controls->kI = value;

    if (strcmp(topic, MQTT_D_TOPIC) == 0)
        controls->kD = value;
}

void MqttHandler::copyModel()
{
    oldModel.boilerTemp = model->boilerTemp;
    oldModel.headTemp = model->headTemp;
    oldModel.heaterState = model->heaterState;
    oldModel.pidControl = model->pidControl;
}