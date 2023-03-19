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
#include <MqttHandler.h>
#include <OtauRedirect.h>
#include <WifiConnection.h>
#include <MqttConnection.h>

WiFiClient WifiClient;
PubSubClient MqttClient(WifiClient);
WifiConnection Wifi(HOSTNAME, WIFI_SSID, WIFI_PASS);
OtauRedirect Otau;

MqttConnection *Mqtt;
Controller *Pid;
MqttHandler *MqttInterface;

PublishModel SenseData;
ControlModel ControlData;

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);

#endif