#include "NetworkModule.h"

void setupWifi(char *hostname, char *ssid, char *password)
{
  WiFi.setHostname(hostname);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void ensureWifi(char *ssid, char *password)
{
  int wifi_retry = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_retry < 5)
  {
    wifi_retry++;
    Serial.println("WiFi not connected. Try to reconnect");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(100);
  }
  if (wifi_retry >= 5)
  {
    Serial.println("\nReboot");
    ESP.restart();
  }
}