#ifndef MqttModule_h
#define MqttModule_h

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

void setupMqtt(IPAddress ip, uint16_t port, int interval);
void setupMqttTopics(char *boilerTopic, char *headTopic, char *pidTopic);
void connectMqtt(char* clientId, char* user, char* password);
void publish(double boiler, double head, double pid);
void publishState(bool state);
void ensureMqtt(char *clientId, char *user, char *password);

#endif