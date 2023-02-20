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
#define MQTT_PASSWORD "{{mqtt_passwort}}"

#define MQTT_PID_TOPIC "lab/pid/control"
#define MQTT_BOILER_TOPIC "lab/pid/boiler"
#define MQTT_HEAD_TOPIC "lab/pid/head"

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

double webKp, webKi, webKd, webWindow, webCycle, webColdstart, webSetpoint;

void setup()
{
  setupSerial(SERIAL_BAUD);
  setupLed(LED_BUILTIN);
  setupSsr(SSR_PIN);
  setupTsic(TSIC_BOILER_PIN, TSIC_HEAD_PIN);
  setupPid(PID_WINDOW, PID_SETPOINT, PID_COLDSTART, PID_DUTY);
  setPidTuning(PID_KP, PID_KI, PID_KD);
  webKp = PID_KP;
  webKi = PID_KI;
  webKd = PID_KD;
  webWindow = PID_WINDOW;
  webCycle = PID_DUTY;
  webColdstart = PID_COLDSTART;
  webSetpoint = PID_SETPOINT;
  setupWifi(HOSTNAME, WIFI_SSID, WIFI_PASS);
  setupMqtt(MQTT_IP, MQTT_PORT, MQTT_INTERVAL);
  setupMqttTopics(MQTT_BOILER_TOPIC, MQTT_HEAD_TOPIC, MQTT_PID_TOPIC);
  connectMqtt(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);
  setupWebserver();
}

void loop()
{
  float headTemp = getTsicOneTemp();
  float boilerTemp = getTsicTwoTemp();
  bool heatState = updatePid(boilerTemp);
  double pid = getPidOutput();
  setSsr(heatState);
  publish(boilerTemp, headTemp, pid);
  publishState(heatState);
  updateLed();
  ensureWifi(WIFI_SSID, WIFI_PASS);
  ensureMqtt(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);
  setPidTuning(webKp, webKi, webKp);
  setPidParameters(webSetpoint, webColdstart, webWindow, webCycle);
}

AsyncWebServer server(80);
const char *input_parameter1 = "kp";
const char *input_parameter2 = "ki";
const char *input_parameter3 = "kd";
const char *input_parameter4 = "window";
const char *input_parameter5 = "cycle";
const char *input_parameter6 = "coldstart";
const char *input_parameter7 = "setpoint";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>PID Tuning</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Verdana; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
  </style>
  </head><body>
  <h2>Tuning Parameters</h2> 
  <form action="/get">
    kP <input type="text" name="kp">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    kI: <input type="text" name="ki">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    kD: <input type="text" name="kd">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Window: <input type="text" name="window">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Cycle: <input type="text" name="cycle">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Setpoint: <input type="text" name="setpoint">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Coldstart: <input type="text" name="coldstart">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

void setupWebserver()
{
  server.onNotFound(notFound);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(418, "text/plain", "I am a coffee machine."); });
  server.on("/tune", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String message;
              if (request->hasParam(input_parameter1)) {
                message = request->getParam(input_parameter1)->value();
                webKp = message.toDouble();
              }
              else if (request->hasParam(input_parameter2)) {
                message = request->getParam(input_parameter2)->value();
                webKi = message.toDouble();
              }
              else if (request->hasParam(input_parameter3)) {
                message = request->getParam(input_parameter3)->value();
                webKd = message.toDouble();
              }
              else if (request->hasParam(input_parameter4)) {
                message = request->getParam(input_parameter4)->value();
                webWindow = message.toDouble();
              }
              else if (request->hasParam(input_parameter5)) {
                message = request->getParam(input_parameter5)->value();
                webCycle = message.toDouble();
              }
              else if (request->hasParam(input_parameter6)) {
                message = request->getParam(input_parameter6)->value();
                webColdstart = message.toDouble();
              }
              else if (request->hasParam(input_parameter7)) {
                message = request->getParam(input_parameter7)->value();
                webSetpoint = message.toDouble();
              }
              
              request->redirect("/tune"); });

  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", printConfig()); });

  AsyncElegantOTA.begin(&server);
  server.begin();
}

String printConfig()
{
  return "P: " + String(webKp,2) + "<br />"
  + "I: " + String(webKi,2) + "<br />"
  + "D: " + String(webKd,2) + "<br />"
  + "Setpoint: " + String(webSetpoint,2) + "<br />"
  + "Coldstart: " + String(webColdstart,2) + "<br />"
  + "Window: " + String(webWindow,2) + "<br />"
  + "Cycle: " + String(webCycle,2) + "<br />";
}