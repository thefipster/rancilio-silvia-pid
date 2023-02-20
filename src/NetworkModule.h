#ifndef NetworkModule_h
#define NetworkModule_h

#include <Arduino.h>
#include <WiFi.h>

void setupWifi(char* hostname, char* ssid, char* password);
void ensureWifi(char *ssid, char *password);

#endif