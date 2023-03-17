#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>
#include <Credentials.h>

class WifiHandler
{

public:
    WifiHandler();

    bool Connect();
    bool Loop();

private:
};

#endif