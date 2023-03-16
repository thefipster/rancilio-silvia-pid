#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <Credentials.h>
#include <Pins.h>
#include <MqttTopics.h>
#include <MqttServer.h>
#include <ESPAsyncWebServer.h>
#include <PID_v1.h>
#include <PubSubClient.h>
#include <TsicSensor.h>
#include <WiFi.h>

#define SERIAL_BAUD 9600
#define HOSTNAME "rancilio-silvia"

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);
void ensureConnections();

#endif