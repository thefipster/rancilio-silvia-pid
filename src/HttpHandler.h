#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class HttpHandler
{
public:
    AsyncWebServer *server;

    HttpHandler();

    void Begin();
};

#endif