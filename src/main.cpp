#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include "config.h"
#include "web_server.h"
#include "stepper_control.h"

void setup() {
    Serial.begin(115200);
    delay(500);

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS konnte nicht gestartet werden!");
        return;
    }

    loadConfig();
    setupStepperControl();      // enthält referenzieren in Reihenfolge
    if (getConfig("wifiAp") == 1) {
        WiFi.softAP("Aquaflux");
        Serial.print("Access Point aktiv unter IP: ");
        Serial.println(WiFi.softAPIP());
        setupWebServer();
    } else {
        Serial.println("WLAN deaktiviert durch Konfiguration.");
    }

    Serial.println("Setup abgeschlossen.");
}

void loop() {
    runStepperControl(); // falls benötigt für stepper.run()
}
