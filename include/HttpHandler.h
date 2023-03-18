#ifndef HTTPHANDLER_H
#define HTTPHANDLER_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class HttpHandler
{
public:
    HttpHandler();
    
    void Setup();
    AsyncWebServer& GetServerReference();

private:
    AsyncWebServer *server;
};

#endif