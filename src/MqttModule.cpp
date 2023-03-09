#include "MqttModule.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

double target = 92;
double coldstart = 80;
double window = 1000;
double cycle = 0.2;
double pValue = 55;
double iValue = 255;
double dValue = 0;

char *mqttBoilerTopic;
char *mqttHeadTopic;
char *mqttPidTopic;
char *mqttHeaterTopic;
char *mqttTargetTopic;
char *mqttColdstartTopic;
char *mqttWindowTopic;
char *mqttCycleTopic;
char *mqttPValueTopic;
char *mqttIValueTopic;
char *mqttDValueTopic;

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

void connectMqtt(char *clientId, char *user, char *password)
{
    if (!mqttClient.connect(clientId, user, password))
    {
        Serial.println("MQTT connection failed.");
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message;
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        message += (char)payload[i];
    }

    double value = atof(message.c_str());

    if (strcmp(topic, mqttTargetTopic) == 0)
    {
        target = value;
    }

    if (strcmp(topic, mqttColdstartTopic) == 0)
    {
        coldstart = value;
    }

    if (strcmp(topic, mqttWindowTopic) == 0)
    {
        window = value;
    }

    if (strcmp(topic, mqttCycleTopic) == 0)
    {
        cycle = value;
    }

    if (strcmp(topic, mqttPValueTopic) == 0)
    {
        pValue = value;
    }

    if (strcmp(topic, mqttIValueTopic) == 0)
    {
        iValue = value;
    }

    if (strcmp(topic, mqttDValueTopic) == 0)
    {
        dValue = value;
    }
}

void subscribeMqtt(char *target, char *coldstart, char *window, char *cycle, char *pValue, char *iValue, char *dValue)
{
    mqttTargetTopic = target;
    mqttColdstartTopic = coldstart;
    mqttWindowTopic = window;
    mqttCycleTopic = cycle;
    mqttPValueTopic = pValue;
    mqttIValueTopic = iValue;
    mqttDValueTopic = dValue;

    mqttClient.setCallback(callback);

    mqttClient.subscribe(mqttTargetTopic);
    mqttClient.subscribe(mqttColdstartTopic);
    mqttClient.subscribe(mqttWindowTopic);
    mqttClient.subscribe(mqttCycleTopic);
    mqttClient.subscribe(mqttPValueTopic);
    mqttClient.subscribe(mqttIValueTopic);
    mqttClient.subscribe(mqttDValueTopic);
}

double getTarget()
{
    return target;
}

double getColdstart()
{
    return coldstart;
}
double getWindow()
{
    return window;
}

double getCycle()
{
    return cycle;
}

double getPValue()
{
    return pValue;
}

double getIValue()
{
    return iValue;
}

double getDValue()
{
    return dValue;
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