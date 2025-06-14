#include <Arduino.h>
#include "web_interface.h"
#include "stepper_control.h"
#include "modbus_handler.h"

void setup() {
  Serial.begin(115200);

  setupStepperControl();
  setupWebInterface();
  setupModbus();

  Serial.println("Aquaflux system initialized.");
}

void loop() {
  handleStepperLoop();
  handleModbusLoop();
}
