#pragma once
#include <Arduino.h>

void loadConfig();
void saveConfig();
void resetConfig();

int getConfig(const String& key);
void setConfig(const String& key, int value);

int getConfigByIndex(int index);
String getConfigKey(int index);

#ifndef CONFIG_H
#define CONFIG_H

/*
 * This example demonstrate how to use asynchronous client & server APIs
 * in order to establish tcp socket connections in client server manner.
 * server is running (on port 7050) on one ESP, acts as AP, and other clients running on
 * remaining ESPs acts as STAs. after connection establishment between server and clients
 * there is a simple message transfer in every 2s. clients connect to server via it's host name
 * (in this case 'esp_server') with help of DNS service running on server side.
 *
 * Note: default MSS for ESPAsyncTCP is 536 byte and defualt ACK timeout is 5s.
*/

#define SSID "ESP-TEST"
#define PASSWORD "123456789"

#define SERVER_HOST_NAME "esp_server"

#define TCP_PORT 7050
#define DNS_PORT 53

#endif // CONFIG_H


#define CONFIG_MAGIC 0xA5F1
#define CONFIG_VERSION 1

struct ConfigData {
    uint16_t magic = CONFIG_MAGIC;
    uint8_t version = CONFIG_VERSION;
    float offset1 = 0.0f;
    float offset2 = 0.0f;
    float maxOpen1 = 20.0f;
    float maxOpen2 = 20.0f;
    float accel = 100.0f;
    float decel = 100.0f;
    uint16_t motionHoldMs = 1000;
    bool watchdogEnabled = false;
    uint16_t watchdogTimeout = 0;
};