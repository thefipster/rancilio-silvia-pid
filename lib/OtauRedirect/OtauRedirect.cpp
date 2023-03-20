#include <OtauRedirect.h>

OtauRedirect::OtauRedirect()
{
    server = new AsyncWebServer(80);
}

void OtauRedirect::Begin()
{
    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->redirect("/update"); });

    server->begin();
}
