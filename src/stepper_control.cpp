#include "stepper_control.h"
#include <AccelStepper.h>

AccelStepper stepper1(AccelStepper::DRIVER, 26, 25); // Beispielpins
AccelStepper stepper2(AccelStepper::DRIVER, 33, 32);

void setupStepperControl() {
  stepper1.setMaxSpeed(1000);
  stepper1.setAcceleration(200);
  stepper2.setMaxSpeed(1000);
  stepper2.setAcceleration(200);
}

void handleStepperLoop() {
  stepper1.run();
  stepper2.run();
}
