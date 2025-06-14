
#pragma once

#include <stdint.h>

struct ReferenceState {
  bool isHomed[2] = {false, false};
  bool isMoving = false;
};

extern ReferenceState refState;

void startReference(uint8_t ch);
void runReference();
bool referenceInProgress();
