#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <PID_v1.h>
#include <TsicSensor.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <WifiHandler.h>
#include <Controller.h>
#include <PublishModel.h>
#include <ControlModel.h>

#include <Credentials.h>
#include <Defaults.h>
#include <Pins.h>
#include <MqttConfig.h>

unsigned long publishIntervalInMs = MQTT_PUBLISH_INTERVAL_MS;
unsigned long intervalStart = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);
AsyncWebServer server(80);
PubSubClient *client;

WifiHandler *wifi;
Controller *controller;
PublishModel model;
ControlModel control;

void setup();
void loop();

void httpSetup();

void mqttSetup();
bool mqttLoop();
void mqttPublish();
void mqttCallback(char *topic, byte *payload, unsigned int length);

#endif