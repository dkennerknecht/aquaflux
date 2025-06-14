#include "../include/config.h"
#include <Preferences.h>

Preferences preferences;

struct ConfigDefaults {
  const char* key;
  int defaultValue;
};

ConfigDefaults defaults[] = {
  {"offset1", 100},
  {"offset2", 120},
  {"softLimit1", 20},
  {"softLimit2", 20},
  {"motionDelay", 1000},
  {"accel1", 300},
  {"accel2", 300},
  {"watchdogTimeout", 0}
};

void loadConfig() {
  preferences.begin("aquaflux", false);
  for (auto& def : defaults) {
    if (!preferences.isKey(def.key)) {
      preferences.putInt(def.key, def.defaultValue);
    }
  }
}

int getConfig(const String& key) {
  return preferences.getInt(key.c_str(), 0);
}

void setConfig(const String& key, int value) {
  preferences.putInt(key.c_str(), value);
}

void saveConfig() {
  preferences.end(); // not strictly needed for putInt, but for cleanliness
  preferences.begin("aquaflux", false);
}

void resetConfig() {
  preferences.clear();
  saveConfig();
}
