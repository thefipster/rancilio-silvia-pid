#ifndef MAIN_H
#define MAIN_H

#include <Credentials.h>
#include <Defaults.h>
#include <MqttConfig.h>
#include <Pins.h>

#include <ControlModel.h>
#include <PublishModel.h>

#include <Arduino.h>
#include <AsyncElegantOTA.h>
#include <PubSubClient.h>

#include <Controller.h>
#include <HttpHandler.h>
#include <MqttHandler.h>
#include <WifiHandler.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

WifiHandler *wifi;
Controller *controller;
HttpHandler *http;
MqttHandler *mqtt;

PublishModel senseModel;
ControlModel controlModel;

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);

#endif