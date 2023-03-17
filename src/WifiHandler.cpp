#include <WifiHandler.h>

WifiHandler::WifiHandler()
{
  WiFi.setHostname(HOSTNAME);
}

bool WifiHandler::Connect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  int tries = 0;
  while (tries < 10 && WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    tries++;
  }

  return tries < 10 ? true : false;
}

bool WifiHandler::Loop()
{
  if (WiFi.status() == WL_CONNECTED)
    return true;

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);

  return Connect();
}