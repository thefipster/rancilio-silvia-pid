#ifndef Main_h
#define Main_h

#include "SerialModule.h"
#include "NetworkModule.h"
#include "MqttModule.h"
#include "SsrModule.h"
#include "PidModule.h"
#include "TsicModule.h"

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

void setup();

void loop();

void publish(
    double temp1,
    double temp2,
    double pid);

void notFound(AsyncWebServerRequest *request);

void setupWebserver();

void ensure();

void sense();

void control();

String printConfig();

#endif