#include "main.h"

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Wifi.Connect();
  Otau.Begin();
  AsyncElegantOTA.begin(Otau.server);

  Mqtt = new MqttConnection(&MqttClient, MQTT_IP, MQTT_PORT);
  Pid = new Controller(&ControlData, &SenseData);
  MqttInterface = new MqttHandler(&MqttClient, &ControlData, &SenseData);

  Mqtt->SetHeartbeat(MQTT_STATE_TOPIC, MQTT_HEARTBEAT_INTERVAL_MS);
  Mqtt->Connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
  MqttClient.setCallback(callback);
  MqttInterface->Subscribe();
}

void loop()
{
  bool wifiState = Wifi.Loop();
  bool mqttState = Mqtt->Loop();

  if (!wifiState || !mqttState)
    ESP.restart();

  MqttInterface->Loop();
  Pid->Loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  MqttInterface->Callback(topic, payload, length);
}