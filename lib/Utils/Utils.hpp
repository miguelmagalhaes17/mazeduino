#pragma once
#include <Arduino.h>

#define DEBUG // Uncomment for debug messages
#define TIME_TASKS // Uncomment to get time information about tasks

// Task Timing Structure
// Times in micros
typedef struct TaskTiming {
  uint64_t timeSum = 0;
  uint64_t timeCount = 0;
  int timeMax = 0;
  int timeMin = 999999;
  int timeStart = 0;
  int timeEnd = 0;
} TaskTiming;

void time_calculations(TaskTiming* tt);