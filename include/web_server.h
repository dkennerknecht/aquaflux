#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void setupWebServer();
void setUnitMode(bool usePercent); // true = %, false = mm
bool getUnitMode();
