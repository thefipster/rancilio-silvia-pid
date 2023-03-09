#ifndef MqttModule_h
#define MqttModule_h

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

void setupMqtt(
    IPAddress ip,
    uint16_t port,
    int interval);

void setupMqttTopics(
    char *boilerTopic,
    char *headTopic,
    char *pidTopic,
    char *heaterTopic);

void connectMqtt(
    char *clientId,
    char *user,
    char *password);

void publish(
    double boiler,
    double head,
    double pid);

void publishState(bool state);

void subscribeMqtt(
    char *target,
    char *coldstart,
    char *window,
    char *cycle,
    char *pValue,
    char *iValue,
    char *dValue);

void ensureMqtt(
    char *clientId,
    char *user,
    char *password);

double getTarget();

double getColdstart();

double getWindow();

double getCycle();

double getPValue();

double getIValue();

double getDValue();

#endif