#include "main.h"

#define SSR_PIN 13

#define TSIC_BOILER_PIN 26
#define TSIC_HEAD_PIN 34

#define SERIAL_BAUD 9600

#define MQTT_IP IPAddress(192, 168, 1, 100)
#define MQTT_PORT 1883
#define MQTT_INTERVAL 1000

#define MQTT_CLIENTID "rancilio-silvia"
#define MQTT_USERNAME "{{mqtt_username}}"
#define MQTT_PASSWORD "{{mqtt_password}}"

#define MQTT_PID_TOPIC "kitchen/rancilio/control"
#define MQTT_BOILER_TOPIC "kitchen/rancilio/boiler"
#define MQTT_HEAD_TOPIC "kitchen/rancilio/head"
#define MQTT_HEATER_TOPIC "kitchen/rancilio/state"

#define MQTT_TARGET_TOPIC "hassio/input_number/kitchen_rancilio_target_temperature/state"
#define MQTT_COLDSTART_TOPIC "hassio/input_number/kitchen_rancilio_coldstart_temperature/state"
#define MQTT_WINDOW_TOPIC "hassio/input_number/kitchen_rancilio_duty_window/state"
#define MQTT_CYCLE_TOPIC "hassio/input_number/kitchen_rancilio_duty_cycle/state"
#define MQTT_P_TOPIC "hassio/input_number/kitchen_rancilio_p_value/state"
#define MQTT_I_TOPIC "hassio/input_number/kitchen_rancilio_i_value/state"
#define MQTT_D_TOPIC "hassio/input_number/kitchen_rancilio_d_value/state"

#define PID_WINDOW 1000
#define PID_SETPOINT 90
#define PID_COLDSTART 80

#define PID_KP 55
#define PID_KI 255
#define PID_KD 0
#define PID_DUTY 0.2

#define HOSTNAME "rancilio-silvia"

#define WIFI_SSID "{{wifi_ssid}}"
#define WIFI_PASS "{{wifi_password}}"

AsyncWebServer server(80);

void setup()
{
  setupSerial(SERIAL_BAUD);
  setupSsr(SSR_PIN);
  setupTsic(
      TSIC_BOILER_PIN,
      TSIC_HEAD_PIN);

  setupPid(
      PID_WINDOW,
      PID_SETPOINT,
      PID_COLDSTART,
      PID_DUTY);

  setPidTuning(
      PID_KP,
      PID_KI,
      PID_KD);

  setupWifi(
      HOSTNAME,
      WIFI_SSID,
      WIFI_PASS);

  setupMqtt(
      MQTT_IP,
      MQTT_PORT,
      MQTT_INTERVAL);

  setupMqttTopics(
      MQTT_BOILER_TOPIC,
      MQTT_HEAD_TOPIC,
      MQTT_PID_TOPIC,
      MQTT_HEATER_TOPIC);

  connectMqtt(
      MQTT_CLIENTID,
      MQTT_USERNAME,
      MQTT_PASSWORD);

  subscribeMqtt(
      MQTT_TARGET_TOPIC,
      MQTT_COLDSTART_TOPIC,
      MQTT_WINDOW_TOPIC,
      MQTT_CYCLE_TOPIC,
      MQTT_P_TOPIC,
      MQTT_I_TOPIC,
      MQTT_D_TOPIC);

  setupWebserver();
}

void loop()
{
  ensure();
  sense();
  control();
}

void ensure()
{
  ensureWifi(WIFI_SSID, WIFI_PASS);
  ensureMqtt(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);
}

void sense()
{
  float headTemp = getTsicOneTemp();
  float boilerTemp = getTsicTwoTemp();
  bool heatState = updatePid(boilerTemp);
  double pid = getPidOutput();
  setSsr(heatState);
  publish(boilerTemp, headTemp, pid);
  publishState(heatState);
}

void control()
{
  double webSetpoint = getTarget();
  double webColdstart = getColdstart();
  double webWindow = getWindow();
  double webCycle = getCycle();
  double webKp = getPValue();
  double webKi = getIValue();
  double webKd = getDValue();
  setPidTuning(webKp, webKi, webKp);
  setPidParameters(webSetpoint, webColdstart, webWindow, webCycle);
}

void setupWebserver()
{
  server.onNotFound([](AsyncWebServerRequest *request){ request->send(404, "text/plain", "Not found"); });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(418, "text/plain", "I am a coffee machine, not a teapot."); });
  AsyncElegantOTA.begin(&server);
  server.begin();
}
