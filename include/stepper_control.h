#pragma once
#include <Arduino.h>

enum ReferenceState {
  REF_NOT_STARTED,
  REF_IN_PROGRESS,
  REF_DONE,
  REF_ERROR
};

void setupStepperControl();
void handleStepperLoop();
bool isReferenceComplete(uint8_t channel);
void triggerReference(uint8_t channel);
