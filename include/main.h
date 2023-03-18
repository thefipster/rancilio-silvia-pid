#ifndef Main_h
#define Main_h

#include <Arduino.h>
#include <PubSubClient.h>
#include <AsyncElegantOTA.h>

#include <WifiHandler.h>
#include <HttpHandler.h>
#include <MqttHandler.h>
#include <Controller.h>
#include <PublishModel.h>
#include <ControlModel.h>

#include <Credentials.h>
#include <Defaults.h>
#include <Pins.h>
#include <MqttConfig.h>

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WifiHandler *wifi;
Controller *controller;
HttpHandler *http;
MqttHandler *mqtt;
PublishModel model;
ControlModel control;

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);

#endif