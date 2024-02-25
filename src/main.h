#ifndef MAIN_H
#define MAIN_H

#include <Credentials.h>
#include <Defaults.h>
#include <MqttConfig.h>
#include <Pins.h>

#include <StateModel.h>

#include <Arduino.h>
#include <ElegantOTA.h>
#include <PubSubClient.h>

#include <PidController.h>
#include <MqttInterface.h>
#include <OtauRedirect.h>
#include <WifiConnection.h>
#include <MqttConnection.h>

WiFiClient WifiClient;
PubSubClient MqttClient(WifiClient);
WifiConnection Wifi(HOSTNAME, WIFI_SSID, WIFI_PASS);
WebServer server(80);

StateModel State;

MqttConnection Mqtt(&MqttClient, MQTT_IP, MQTT_PORT);
MqttInterface Interface(&MqttClient, &State);

PidController *Pid;

void setup();
void loop();
void callback(char *topic, byte *payload, unsigned int length);

#endif