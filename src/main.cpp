#include "main.h"

void setup()
{
  Serial.begin(SERIAL_BAUD);

  Wifi.Connect();
  Otau.Begin();
  AsyncElegantOTA.begin(Otau.server);

  Mqtt.SetHeartbeat(STATE_TOPIC, HEARTBEAT_INTERVAL_MS);
  Mqtt.Connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
  MqttClient.setCallback(callback);
  Interface.Subscribe();

  Pid = new PidController(&ControlData, &SenseData);
}

void loop()
{
  bool wifiState = Wifi.Loop();
  bool mqttState = Mqtt.Loop();

  if (!wifiState || !mqttState)
    ESP.restart();

  Interface.Loop();
  Pid->Loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Interface.Callback(topic, payload, length);
}