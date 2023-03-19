#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

class WifiConnection
{

public:
    WifiConnection(char *ssid, char *key);
    WifiConnection(char *hostname, char *ssid, char *key);

    bool Connect();
    bool Loop();

private:
    char* ssid;
    char* key;
};

#endif