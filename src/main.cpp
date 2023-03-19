#include "main.h"

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Wifi.Connect();
  Otau.Begin();
  AsyncElegantOTA.begin(Otau.server);

  Mqtt = new MqttHandler(&MqttClient, &ControlData, &SenseData);
  Mqtt->Connect();
  MqttClient.setCallback(callback);
  Mqtt->Subscribe();

  PidLoop = new Controller(&ControlData, &SenseData);
}

void loop()
{
  bool wifiState = Wifi.Loop();
  bool mqttState = Mqtt->Loop();

  if (!wifiState || !mqttState)
    ESP.restart();

  PidLoop->Loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Mqtt->Callback(topic, payload, length);
}