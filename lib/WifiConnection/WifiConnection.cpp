#include <WifiConnection.h>

WifiConnection::WifiConnection(char *ssid, char *key)
{
  this->ssid = ssid;
  this->key = key;
}

WifiConnection::WifiConnection(char *hostname, char *ssid, char *key)
    : WifiConnection(ssid, key)
{
  WiFi.setHostname(hostname);
}

bool WifiConnection::Connect()
{
  WiFi.begin(ssid, key);
  int tries = 0;
  while (tries < 5 && WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tries++;
  }

  return tries < 5 ? true : false;
}

bool WifiConnection::Loop()
{
  if (WiFi.status() == WL_CONNECTED)
    return true;

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);

  return Connect();
}