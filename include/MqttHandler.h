#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <ControlModel.h>
#include <PublishModel.h>

#include <Credentials.h>
#include <Defaults.h>
#include <MqttConfig.h>

class MqttHandler
{

public:
    MqttHandler(PubSubClient *client, ControlModel *controls, PublishModel *publish);
    bool Connect();
    void Subscribe();
    bool Loop();
    void Callback(char *topic, byte *payload, unsigned int length);

private:
    PubSubClient *client;
    ControlModel *controls;
    ControlModel oldControls;
    PublishModel *model;
    PublishModel oldModel;

    unsigned long publishIntervalInMs = MQTT_PUBLISH_INTERVAL_MS;
    unsigned long loopStart = 0;

    unsigned long heartbeatIntervalInMs = MQTT_HEARTBEAT_INTERVAL_MS;
    unsigned long heartbeatStart = 0;

    void copyModel();
    void copyControls();
    bool reconnect();
    void publish();
    void heartbeat();
};

#endif