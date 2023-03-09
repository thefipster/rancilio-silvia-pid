#include "MqttModule.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

float targetTemperature = 92;

char *mqttBoilerTopic, *mqttHeadTopic, *mqttPidTopic, *mqttHeaterTopic;

int mqttPublishWindow = 1000;

void setupMqtt(IPAddress ip, uint16_t port, int interval)
{
    Serial.println("MQTT setup");
    mqttClient.setServer(ip, port);
    mqttPublishWindow = interval;
}

void setupMqttTopics(char *boilerTopic, char *headTopic, char *pidTopic, char *heaterTopic)
{
    mqttBoilerTopic = boilerTopic;
    mqttHeadTopic = headTopic;
    mqttPidTopic = pidTopic;
    mqttHeaterTopic = heaterTopic;
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        message += (char)payload[i];
    }
    float num =atof(message.c_str());
    targetTemperature = num;
}

void connectMqtt(char *clientId, char *user, char *password)
{
    if (!mqttClient.connect(clientId, user, password))
    {
        Serial.println("MQTT connection failed.");
    }

    mqttClient.setCallback(callback);
    mqttClient.subscribe("hassio/input_number/kitchen_rancilio_target_temperature/state");
}

float getTargetTemperature()
{
    return targetTemperature;
}

unsigned long mqttPublishWindowStart = 0;
void publish(double boiler, double head, double pid)
{
    unsigned long now = millis();
    if (now - mqttPublishWindowStart > mqttPublishWindow)
    {
        mqttPublishWindowStart += mqttPublishWindow;

        mqttClient.publish(mqttBoilerTopic, String(boiler, 3).c_str());
        mqttClient.publish(mqttHeadTopic, String(head, 3).c_str());
        mqttClient.publish(mqttPidTopic, String(pid, 3).c_str());
    }
}

unsigned long mqttPublishStateWindowStart = 0;
void publishState(bool state)
{
    unsigned long now = millis();
    if (now - mqttPublishStateWindowStart > mqttPublishWindow)
    {
        mqttPublishStateWindowStart += mqttPublishWindow;

        mqttClient.publish(mqttHeaterTopic, state ? "1" : "0");
    }
}

void ensureMqtt(char *clientId, char *user, char *password)
{
    mqttClient.loop();

    if (!mqttClient.connected())
    {
        while (!mqttClient.connected())
        {
            mqttClient.connect(clientId, user, password);
            uint8_t timeout = 8;
            while (timeout && (!mqttClient.connected()))
            {
                timeout--;
                delay(1000);
            }
        }
    }
}