
#pragma once

enum StepperError {
  ERROR_NONE = 0,
  ERROR_NOT_REFERENCED = 1,
  ERROR_SOFT_LIMIT = 2,
  ERROR_MOTION_BLOCKED = 3,
  ERROR_UNKNOWN = 99
};
