#ifndef MQTTINTERFACE_H
#define MQTTINTERFACE_H

#include <Credentials.h>
#include <Defaults.h>
#include <MqttConfig.h>

#include <StateModel.h>

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MqttInterface
{

public:
    MqttInterface(PubSubClient *client, StateModel *state);
    void Subscribe();
    void Loop();
    void Callback(char *topic, byte *payload, unsigned int length);

private:
    PubSubClient *client;
    StateModel *state;
    StateModel oldState;

    unsigned long intervalInMs = PUBLISH_INTERVAL_MS;
    unsigned long loopStart = 0;

    void copyModel();
    void copyControls();
    void publish();
};

#endif