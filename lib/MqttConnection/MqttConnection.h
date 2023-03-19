#ifndef MQTTCONNECTION_H
#define MQTTCONNECTION_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MqttConnection
{

public:
    MqttConnection(PubSubClient *client, IPAddress ip, uint16_t port);
    bool Connect(char* clientId, char* username, char* password);
    void SetHeartbeat(char* topic, unsigned long intervalMs);
    bool Loop();

private:
    PubSubClient *client;
    IPAddress ip;
    uint16_t port;
    char* username;
    char* password;
    char* clientId;

    bool heartbeat = false;
    unsigned long loopStart = 0;
    unsigned long intervalMs;

    char* topic;

    bool connect();
};

#endif
