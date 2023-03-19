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

#include <PidController.h>
#include <MqttInterface.h>
#include <OtauRedirect.h>
#include <WifiConnection.h>
#include <MqttConnection.h>

WiFiClient WifiClient;
PubSubClient MqttClient(WifiClient);
WifiConnection Wifi(HOSTNAME, WIFI_SSID, WIFI_PASS);
OtauRedirect Otau;

PublishModel SenseData;
ControlModel ControlData;

MqttConnection Mqtt(&MqttClient, MQTT_IP, MQTT_PORT);
MqttInterface Interface(&MqttClient, &ControlData, &SenseData);

PidController *Pid;

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);

#endif