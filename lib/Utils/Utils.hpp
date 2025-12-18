#pragma once

#include <Arduino.h>

#define DEBUG // Uncomment for debug messages
#define TIME_TASKS // Uncomment to get time information about tasks

// Task Timing Structure
typedef struct TaskTiming {
  uint64_t timeSum;
  uint64_t timeCount;
  int timeMax;
  int timeMin;
} TaskTiming;