#include <HttpHandler.h>

HttpHandler::HttpHandler()
{
    server = new AsyncWebServer(80);
}

void HttpHandler::Setup()
{
    server->onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/plain", "Not found"); });

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(418, "text/plain", "I am a coffee machine."); });

    server->begin();
}

AsyncWebServer& HttpHandler::GetServerReference()
{
    return *server;
}