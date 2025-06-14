#include <Arduino.h>
#include <WiFi.h>
#include <ModbusIP_ESP8266.h>  // wird intern auf ESP32 umgeleitet
#include "config.h"
#include "stepper_control.h"

ModbusIP mb;

const int REG_COUNT = 20;  // Reserviert für Werte & Konfiguration
uint16_t regs[REG_COUNT] = {0};

// Mapping: [0-1] Zielwert mm, [2-3] Zielwert %, [4-5] Position mm, [6-7] Position %, [8-9] Offset, [10-11] SoftLimit
// [12-13] Accel, [14] wifiAp, [15] Fehlercode 1, [16] Fehlercode 2

void setupModbus() {
    mb.server();
    mb.begin();
    for (int i = 0; i < REG_COUNT; i++) {
        mb.addHreg(i);
    }
}

void updateModbus() {
    mb.task();

    for (int ch = 1; ch <= 2; ch++) {
        int i = ch - 1;
        // Zielwert mm
        if (regs[i] != mb.Hreg(i)) {
            regs[i] = mb.Hreg(i);
            moveToMm(ch, regs[i]);
        }

        // Offset
        int offIndex = 8 + i;
        if (regs[offIndex] != mb.Hreg(offIndex)) {
            regs[offIndex] = mb.Hreg(offIndex);
            setConfig(ch == 1 ? "offset1" : "offset2", regs[offIndex]);
        }

        // SoftLimit
        int slIndex = 10 + i;
        if (regs[slIndex] != mb.Hreg(slIndex)) {
            if (mb.Hreg(slIndex) <= 30) {
                regs[slIndex] = mb.Hreg(slIndex);
                setConfig(ch == 1 ? "softLimit1" : "softLimit2", regs[slIndex]);
            }
        }

        // Accel
        int acIndex = 12 + i;
        if (regs[acIndex] != mb.Hreg(acIndex)) {
            regs[acIndex] = mb.Hreg(acIndex);
            setConfig(ch == 1 ? "accel1" : "accel2", regs[acIndex]);
        }

        // Fehlercode als Dummy (später setzen bei Error)
        mb.Hreg(15 + i, 0);

        // Position mm und % setzen
        float pos = getCurrentMm(ch);
        regs[4 + i] = pos;
        mb.Hreg(4 + i, regs[4 + i]);

        int sl = getConfig(ch == 1 ? "softLimit1" : "softLimit2");
        regs[6 + i] = (pos / sl) * 100.0;
        mb.Hreg(6 + i, regs[6 + i]);

        // Offset zurückschreiben
        regs[8 + i] = getConfig(ch == 1 ? "offset1" : "offset2");
        mb.Hreg(8 + i, regs[8 + i]);
    }

    // wifiAp zurückschreiben
    regs[14] = getConfig("wifiAp");
    mb.Hreg(14, regs[14]);
}
