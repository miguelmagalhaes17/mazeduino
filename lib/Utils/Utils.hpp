#pragma once
#include <Arduino.h>
#include <string.h>

#define DEBUG // Uncomment for debug messages
#define TIME_TASKS // Uncomment to get time information about tasks

#define NUMBER_OF_TASKS 1

// Task Timing Structure
// Times in micros
typedef struct TaskTiming {
  uint64_t timeSum = 0;
  uint64_t timeCount = 0;
  int timeMax = 0;
  int timeMin = 999999;
} TaskTiming;

typedef struct NVSSave {
  // Put NVS related variables here
  std::string nvsTaskKey;
  TaskTiming taskTime;
} NVSSave;

void task_time_calculations(TaskTiming* tt);