#include "config.h"
#include <Preferences.h>

Preferences preferences;

struct ConfigDefaults {
    const char* key;
    int defaultValue;
};

ConfigDefaults defaults[] = {
        {"wifiAp", 1},
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
    if (key == "softLimit1" || key == "softLimit2") {
        const int HARD_LIMIT = 30;
        if (value > HARD_LIMIT) {
            Serial.printf("Fehler: SoftLimit %s (=%d) darf HardLimit (%d) nicht überschreiten!\n", key.c_str(), value, HARD_LIMIT);
            return;
        }
    }
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


int getConfigByIndex(int index) {
    const char* keys[] = {
            "offset1", "offset2",
            "softLimit1", "softLimit2",
            "accel1", "accel2",
            "wifiAp"
    };
    if (index < 0 || index >= sizeof(keys)/sizeof(keys[0])) return -1;
    return getConfig(keys[index]);
}

String getConfigKey(int index) {
    const char* keys[] = {
            "offset1", "offset2",
            "softLimit1", "softLimit2",
            "accel1", "accel2",
            "wifiAp"
    };
    if (index < 0 || index >= sizeof(keys)/sizeof(keys[0])) return "";
    return String(keys[index]);
}
