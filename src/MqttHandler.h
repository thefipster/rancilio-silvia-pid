#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <Credentials.h>
#include <Defaults.h>
#include <MqttConfig.h>

#include <ControlModel.h>
#include <PublishModel.h>

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MqttHandler
{

public:
    MqttHandler(PubSubClient *client, ControlModel *controls, PublishModel *publish);
    void Subscribe();
    void Loop();
    void Callback(char *topic, byte *payload, unsigned int length);

private:
    PubSubClient *client;
    ControlModel *controls;
    ControlModel oldControls;
    PublishModel *model;
    PublishModel oldModel;

    unsigned long intervalInMs = MQTT_PUBLISH_INTERVAL_MS;
    unsigned long loopStart = 0;

    void copyModel();
    void copyControls();
    void publish();
};

#endif