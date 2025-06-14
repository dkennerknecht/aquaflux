#pragma once
#include <Arduino.h>

void loadConfig();
void saveConfig();
void resetConfig();

int getConfig(const String& key);
void setConfig(const String& key, int value);
