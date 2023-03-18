#include "main.h"

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("setup started");

  wifi = new WifiHandler();
  wifi->Connect();

  httpSetup();
  mqttSetup();

  controller = new Controller(&control, &model);
  Serial.println("setup finished");
}

void loop()
{
  bool wifiState = wifi->Loop();
  bool mqttState = mqttLoop();

  if (!wifiState || !mqttState)
    ESP.restart();

  controller->Loop();
  mqttPublish();
}

void httpSetup()
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(418, "text/plain", "I am a coffee machine."); });
  AsyncElegantOTA.begin(&server);
  server.begin();
}

void mqttSetup()
{
  mqttClient.setServer(MQTT_IP, MQTT_PORT);
  mqttClient.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
  mqttClient.setCallback(mqttCallback);

  mqttClient.subscribe(MQTT_TARGET_TOPIC);
  mqttClient.subscribe(MQTT_COLDSTART_TOPIC);
  mqttClient.subscribe(MQTT_WINDOW_TOPIC);
  mqttClient.subscribe(MQTT_CYCLE_TOPIC);
  mqttClient.subscribe(MQTT_P_TOPIC);
  mqttClient.subscribe(MQTT_I_TOPIC);
  mqttClient.subscribe(MQTT_D_TOPIC);
}

bool mqttLoop()
{
  mqttClient.loop();
  
  int tries = 0;
  if (!mqttClient.connected())
  {
    while (tries < 10 && !mqttClient.connected())
    {
      mqttClient.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS);
      uint8_t timeout = 8;
      while (timeout && (!mqttClient.connected()))
      {
        timeout--;
        delay(1000);
      }
    }
  }

  return tries < 10 ? true : false;
}

void mqttPublish()
{
  if (millis() - intervalStart > publishIntervalInMs)
  {
    intervalStart += publishIntervalInMs;
    mqttClient.publish(MQTT_BOILER_TOPIC, String(model.boilerTemp, 3).c_str());
    mqttClient.publish(MQTT_HEAD_TOPIC, String(model.headTemp, 3).c_str());
    mqttClient.publish(MQTT_PID_TOPIC, String(model.pidControl, 3).c_str());
    mqttClient.publish(MQTT_HEATER_TOPIC, model.heaterState ? "1" : "0");

    mqttClient.publish(MQTT_TARGET_GET_TOPIC, String(control.setpoint, 3).c_str());
    mqttClient.publish(MQTT_COLD_GET_TOPIC, String(control.coldstart, 3).c_str());
    mqttClient.publish(MQTT_WINDOW_GET_TOPIC, String(control.windowMs, 3).c_str());
    mqttClient.publish(MQTT_CYCLE_GET_TOPIC, String(control.dutyCycle, 3).c_str());
    mqttClient.publish(MQTT_P_GET_TOPIC, String(control.kP, 3).c_str());
    mqttClient.publish(MQTT_I_GET_TOPIC, String(control.kI, 3).c_str());
    mqttClient.publish(MQTT_D_GET_TOPIC, String(control.kD, 3).c_str());

    Serial.print(".");
  }
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (int i = 0; i < length; i++)
    message += (char)payload[i];

  double value = atof(message.c_str());
  Serial.print(".");
  Serial.print(value);
  Serial.print(".");

  if (strcmp(topic, MQTT_TARGET_TOPIC) == 0)
    control.setpoint = value;

  if (strcmp(topic, MQTT_COLDSTART_TOPIC) == 0)
    control.coldstart = value;

  if (strcmp(topic, MQTT_WINDOW_TOPIC) == 0)
    control.windowMs = value;

  if (strcmp(topic, MQTT_CYCLE_TOPIC) == 0)
    control.dutyCycle = value;

  if (strcmp(topic, MQTT_P_TOPIC) == 0)
    control.kP = value;

  if (strcmp(topic, MQTT_I_TOPIC) == 0)
    control.kI = value;

  if (strcmp(topic, MQTT_D_TOPIC) == 0)
    control.kD = value;
}