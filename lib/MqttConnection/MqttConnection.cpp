#include <MqttConnection.h>

MqttConnection::MqttConnection(PubSubClient *client, IPAddress ip, uint16_t port)
{
    this->client = client;
    this->ip = ip;
    this->port = port;

    client->setServer(ip, port);
}

void MqttConnection::SetHeartbeat(char *topic, unsigned long intervalMs)
{
    this->intervalMs = intervalMs;
    this->topic = topic;
}

bool MqttConnection::Connect(char *clientId, char *username, char *password)
{
    this->clientId = clientId;
    this->username = username;
    this->password = password;

    return connect();
}

bool MqttConnection::Loop()
{
    client->loop();

    if (millis() - loopStart > intervalMs)
    {
        client->publish(topic, "on");
        loopStart = millis();
    }

    if (!client->connected())
        return connect();

    return true;
}

bool MqttConnection::connect()
{
    int tries = 0;

    do
    {
        client->connect(clientId, username, password);
        uint8_t timeout = 5;
        while (timeout && (!client->connected()))
        {
            timeout--;
            delay(500);
        }
    } while (tries < 5 && !client->connected());

    return client->connected();
}