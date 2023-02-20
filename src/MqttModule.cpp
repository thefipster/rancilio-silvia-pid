#include "MqttModule.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

char *mqttBoilerTopic, *mqttHeadTopic, *mqttPidTopic;

int mqttPublishWindow = 1000;

void setupMqtt(IPAddress ip, uint16_t port, int interval)
{
    Serial.println("MQTT setup");
    mqttClient.setServer(ip, port);
    mqttPublishWindow = interval;
}

void setupMqttTopics(char *boilerTopic, char *headTopic, char *pidTopic)
{
    mqttBoilerTopic = boilerTopic;
    mqttHeadTopic = headTopic;
    mqttPidTopic = pidTopic;
}

void connectMqtt(char *clientId, char *user, char *password)
{
    if (!mqttClient.connect(clientId, user, password))
    {
        Serial.println("MQTT connection failed.");
    }
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

        mqttClient.publish("lab/pid/state", state ? "1" : "0");
    }
}

void ensureMqtt(char *clientId, char *user, char *password)
{
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