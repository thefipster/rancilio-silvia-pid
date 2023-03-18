#include "main.h"

void setup()
{
    Serial.begin(SERIAL_BAUD);

    wifi = new WifiHandler();
    wifi->Connect();

    http = new HttpHandler();
    http->Begin();
    AsyncElegantOTA.begin(http->server);

    mqtt = new MqttHandler(&mqttClient, &controlModel, &senseModel);
    mqtt->Connect();
    mqttClient.setCallback(callback);
    mqtt->Subscribe();

    controller = new Controller(&controlModel, &senseModel);
}

void loop()
{
    bool wifiState = wifi->Loop();
    bool mqttState = mqtt->Loop();

    if (!wifiState || !mqttState)
        ESP.restart();

    controller->Loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
    mqtt->Callback(topic, payload, length);
}