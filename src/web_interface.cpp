#include "web_interface.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void setupWebInterface() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Aquaflux Web Interface");
  });

  server.begin();
}
