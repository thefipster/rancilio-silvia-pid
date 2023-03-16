#include "main.h"

double headTemp = 0;
bool heaterState = false;

double pidInput;
double pidOutput;
double pidControl = 0;
double pidSetpoint = 92;
double pidWindowMs = 1000;
double pidColdstart = 85;
double pidDutyCycle = 0.2;

double pidKp = 33;
double pidKi = 255;
double pidKd = 0;

unsigned long pidLoopStart = 0;

unsigned long mqttWindowMs = 1000;
unsigned long mqttLoopStart = 0;

AsyncWebServer server(80);
TsicSensor *boilerSensor;
TsicSensor *headSensor;
PID boilerPid(&pidInput, &pidOutput, &pidSetpoint, pidKp, pidKi, pidKd, DIRECT);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup()
{
  // serial
  Serial.begin(SERIAL_BAUD);

  // wifi
  WiFi.setHostname(HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  // solid state relay
  pinMode(SSR_PIN, OUTPUT);

  // tsic temperature probes
  boilerSensor = TsicSensor::create(TSIC_BOILER_PIN, TsicExternalVcc, TsicType::TSIC_306);
  headSensor = TsicSensor::create(TSIC_HEAD_PIN, TsicExternalVcc, TsicType::TSIC_306);

  // pid
  boilerPid.SetOutputLimits(0, pidWindowMs);
  boilerPid.SetMode(AUTOMATIC);
  boilerPid.SetTunings(pidKp, pidKi, pidKd);

  // mqtt
  mqttClient.setServer(MQTT_IP, MQTT_PORT);
  if (mqttClient.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASS))
  {
    mqttClient.setCallback(callback);

    mqttClient.subscribe(MQTT_TARGET_TOPIC);
    mqttClient.subscribe(MQTT_COLDSTART_TOPIC);
    mqttClient.subscribe(MQTT_WINDOW_TOPIC);
    mqttClient.subscribe(MQTT_CYCLE_TOPIC);
    mqttClient.subscribe(MQTT_P_TOPIC);
    mqttClient.subscribe(MQTT_I_TOPIC);
    mqttClient.subscribe(MQTT_D_TOPIC);
  }

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(418, "text/plain", "I am a coffee machine."); });
  AsyncElegantOTA.begin(&server);
  server.begin();
}

void loop()
{
  ensureConnections();

  // get temps
  if (boilerSensor->newValueAvailable())
    pidInput = boilerSensor->getTempCelsius();

  if (headSensor->newValueAvailable())
    headTemp = headSensor->getTempCelsius();

  // compute pid
  boilerPid.Compute();
  long timeDelta = millis() - pidLoopStart;
  if (timeDelta > pidWindowMs / pidDutyCycle)
    pidLoopStart = millis();

  pidControl = pidInput < pidColdstart ? 1 : pidOutput / pidWindowMs;
  heaterState = pidInput < pidColdstart ? true : pidOutput > timeDelta;

  // set ssr
  digitalWrite(SSR_PIN, heaterState ? 1 : 0);

  // publish sense and control values
  if (millis() - mqttLoopStart > mqttWindowMs)
  {
    mqttLoopStart += mqttWindowMs;
    mqttClient.publish(MQTT_BOILER_TOPIC, String(pidInput, 3).c_str());
    mqttClient.publish(MQTT_HEAD_TOPIC, String(headTemp, 3).c_str());
    mqttClient.publish(MQTT_PID_TOPIC, String(pidControl, 3).c_str());
    mqttClient.publish(MQTT_HEATER_TOPIC, heaterState ? "1" : "0");
  }

  // update pid
  boilerPid.SetTunings(pidKp, pidKi, pidKd);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String message;
  for (int i = 0; i < length; i++)
    message += (char)payload[i];

  double value = atof(message.c_str());

  if (strcmp(topic, MQTT_TARGET_TOPIC) == 0)
    pidSetpoint = value;

  if (strcmp(topic, MQTT_COLDSTART_TOPIC) == 0)
    pidColdstart = value;

  if (strcmp(topic, MQTT_WINDOW_TOPIC) == 0)
    pidWindowMs = value;

  if (strcmp(topic, MQTT_CYCLE_TOPIC) == 0)
    pidDutyCycle = value;

  if (strcmp(topic, MQTT_P_TOPIC) == 0)
    pidKp = value;

  if (strcmp(topic, MQTT_I_TOPIC) == 0)
    pidKi = value;

  if (strcmp(topic, MQTT_D_TOPIC) == 0)
    pidKd = value;
}

void ensureConnections()
{
  // ensure wifi connection
  int wifi_retry = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_retry < 5)
  {
    wifi_retry++;
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(500);
  }

  if (wifi_retry >= 5)
    ESP.restart();

  // ensure mqtt connection
  mqttClient.loop();
  if (!mqttClient.connected())
  {
    while (!mqttClient.connected())
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
}