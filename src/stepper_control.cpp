#include "stepper_control.h"
#include <AccelStepper.h>

#define ENDSTOP_PIN_1 14
#define ENDSTOP_PIN_2 27
#define STEP_PIN_1 26
#define DIR_PIN_1 25
#define STEP_PIN_2 33
#define DIR_PIN_2 32

AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_1, DIR_PIN_1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_2, DIR_PIN_2);

ReferenceState refState1 = REF_NOT_STARTED;
ReferenceState refState2 = REF_NOT_STARTED;

bool isReferencing = false;

const float REF_FAST_SPEED = -800; // negative = Richtung auf Sensor zu
const float REF_SLOW_SPEED = -200;
const int REF_BACKOFF_STEPS = 200; // Zurückfahren nach Endstop-Treffer
const int REF_OFFSET_STEPS = 100;  // Offset nach dem Sensor

void setupStepperControl() {
  pinMode(ENDSTOP_PIN_1, INPUT_PULLUP);
  pinMode(ENDSTOP_PIN_2, INPUT_PULLUP);

  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(300);
  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(300);

  triggerReference(1); // automatische Referenzfahrt
}

void triggerReference(uint8_t channel) {
  if (channel == 1 && refState1 == REF_NOT_STARTED) {
    stepper1.setSpeed(REF_FAST_SPEED);
    refState1 = REF_IN_PROGRESS;
    isReferencing = true;
  } else if (channel == 2 && refState2 == REF_NOT_STARTED && refState1 == REF_DONE) {
    stepper2.setSpeed(REF_FAST_SPEED);
    refState2 = REF_IN_PROGRESS;
    isReferencing = true;
  }
}

bool isReferenceComplete(uint8_t channel) {
  if (channel == 1) return refState1 == REF_DONE;
  if (channel == 2) return refState2 == REF_DONE;
  return false;
}

void handleReference(AccelStepper& stepper, uint8_t endstopPin, ReferenceState& state) {
  static bool slowPhase = false;
  static bool backingOff = false;
  static long backoffTarget = 0;

  if (state == REF_IN_PROGRESS) {
    if (!slowPhase) {
      if (digitalRead(endstopPin) == LOW) {
        stepper.stop();
        stepper.runToPosition(); // sicher anhalten
        slowPhase = true;
        stepper.setCurrentPosition(0);
        stepper.setSpeed(REF_SLOW_SPEED);
      } else {
        stepper.runSpeed();
      }
    } else if (!backingOff) {
      if (digitalRead(endstopPin) == LOW) {
        stepper.stop();
        stepper.runToPosition();
        backoffTarget = stepper.currentPosition() + REF_BACKOFF_STEPS;
        stepper.moveTo(backoffTarget);
        backingOff = true;
      } else {
        stepper.runSpeed();
      }
    } else {
      if (stepper.distanceToGo() == 0) {
        stepper.setCurrentPosition(-REF_OFFSET_STEPS);
        state = REF_DONE;
        slowPhase = false;
        backingOff = false;
        isReferencing = false;
      } else {
        stepper.run();
      }
    }
  }
}

void handleStepperLoop() {
  if (refState1 == REF_IN_PROGRESS) {
    handleReference(stepper1, ENDSTOP_PIN_1, refState1);
  } else if (refState1 == REF_DONE && refState2 == REF_NOT_STARTED) {
    triggerReference(2);
  } else if (refState2 == REF_IN_PROGRESS) {
    handleReference(stepper2, ENDSTOP_PIN_2, refState2);
  } else {
    if (!isReferencing) {
      stepper1.run();
      stepper2.run();
    }
  }
}


#include "config.h"

const float STEPS_PER_MM = 8.0;         // TR8x2 (2 mm Steigung), 1/32 Step: 200 * 32 / 2mm
const float HARD_LIMIT_MM = 30.0;

void moveToMm(uint8_t channel, float targetMm) {
  int softLimit = getConfig(channel == 1 ? "softLimit1" : "softLimit2");
  int accel = getConfig(channel == 1 ? "accel1" : "accel2");

  if (targetMm < 0 || targetMm > HARD_LIMIT_MM || targetMm > softLimit) {
    Serial.printf("Ziel %.2fmm liegt außerhalb der erlaubten Grenzen!", targetMm);
    return;
  }

  long targetSteps = targetMm * STEPS_PER_MM;
  if (channel == 1) {
    stepper1.setAcceleration(accel);
    stepper1.moveTo(targetSteps);
  } else if (channel == 2) {
    stepper2.setAcceleration(accel);
    stepper2.moveTo(targetSteps);
  }
}

float getCurrentMm(uint8_t channel) {
  long steps = (channel == 1) ? stepper1.currentPosition() : stepper2.currentPosition();
  return steps / STEPS_PER_MM;
}


void runStepperControl() {
  // hier würde ggf. stepper1.run() etc. zyklisch aufgerufen
}
