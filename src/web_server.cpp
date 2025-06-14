#include "web_server.h"
#include "stepper_control.h"
#include "config.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

AsyncWebServer server(80);
bool unitModePercent = true;

void setUnitMode(bool usePercent) {
    unitModePercent = usePercent;
}
bool getUnitMode() {
    return unitModePercent;
}

void handleSet(AsyncWebServerRequest *request) {
    if (request->hasParam("ch", true) && request->hasParam("val", true)) {
        int ch = request->getParam("ch", true)->value().toInt();
        float val = request->getParam("val", true)->value().toFloat();
        if (unitModePercent) {
            int soft = getConfig(ch == 1 ? "softLimit1" : "softLimit2");
            val = (val / 100.0) * soft;
        }
        moveToMm(ch, val);
        request->send(200, "text/plain", "OK");
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
}

void handleReference(AsyncWebServerRequest *request) {
    if (request->hasParam("ch", true)) {
        int ch = request->getParam("ch", true)->value().toInt();
        triggerReference(ch);
        request->send(200, "text/plain", "REF STARTED");
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
}

void handleStatus(AsyncWebServerRequest *request) {
    JsonDocument doc;

    int chParam = request->hasParam("ch") ? request->getParam("ch")->value().toInt() : 0;
    int from = chParam >= 1 && chParam <= 2 ? chParam : 1;
    int to = chParam >= 1 && chParam <= 2 ? chParam : 2;

    for (int ch = from; ch <= to; ch++) {
        float posMm = getCurrentMm(ch);
        int limit = getConfig(ch == 1 ? "softLimit1" : "softLimit2");
        int offset = getConfig(ch == 1 ? "offset1" : "offset2");
        int ref = isReferenceComplete(ch) ? 1 : 0;
        float val = unitModePercent ? (posMm / limit) * 100.0 : posMm;
        doc["pos" + String(ch)] = val;
        doc["ref" + String(ch)] = ref;
        doc["limit" + String(ch)] = limit;
        doc["offset" + String(ch)] = offset;
    }
    doc["unit"] = unitModePercent ? "percent" : "mm";

    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
}




void setupWebServer() {
    server.on("/set", HTTP_POST, handleSet);
    server.on("/reference", HTTP_POST, handleReference);
    server.on("/status", HTTP_GET, handleStatus);
    server.on("/unit", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("mode", true)) {
            String m = request->getParam("mode", true)->value();
            unitModePercent = (m == "percent");
            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Bad Request");
        }
    });

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/offset", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("ch", true) && request->hasParam("val", true)) {
            int ch = request->getParam("ch", true)->value().toInt();
            int val = request->getParam("val", true)->value().toInt();
            setConfig(ch == 1 ? "offset1" : "offset2", val);
            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Bad Request");
        }
    });

    server.begin();
}
