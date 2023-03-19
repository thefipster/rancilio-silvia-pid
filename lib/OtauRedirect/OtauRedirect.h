#ifndef OTAUREDIRECT_H
#define OTAUREDIRECT_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class OtauRedirect
{
public:
    AsyncWebServer *server;
    OtauRedirect();
    void Begin();
};
#endif