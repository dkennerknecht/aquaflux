
#include "reference.h"

ReferenceState refState;

void startReference(uint8_t ch) {
  if (ch < 1 || ch > 2) return;
  refState.isMoving = true;
  refState.isHomed[ch - 1] = false;
}

void runReference() {
  // hier würde der FSM für die mehrstufige Referenzfahrt laufen
  // am Ende:
  // refState.isHomed[0] = true;
  // refState.isHomed[1] = true;
  refState.isMoving = false;
}

bool referenceInProgress() {
  return refState.isMoving;
}
