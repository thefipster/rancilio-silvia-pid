#include "main.h"

void setup()
{
  Serial.begin(SERIAL_BAUD);

  Wifi.Connect();
  server.on("/", []() {
    server.send(418, "text/plain", "Hi! I'm actually a coffee machine.");
  });

  ElegantOTA.begin(&server);
  server.begin();

  Mqtt.SetHeartbeat(STATE_TOPIC, HEARTBEAT_INTERVAL_MS);
  Mqtt.Connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
  MqttClient.setCallback(callback);
  Interface.Subscribe();

  Pid = new PidController(&State);
}

void loop()
{
  Wifi.Loop();
  Mqtt.Loop();
  Interface.Loop();
  Pid->Loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Interface.Callback(topic, payload, length);
}