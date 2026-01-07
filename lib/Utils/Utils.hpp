#pragma once
#include <Arduino.h>

//#define DEBUG // Uncomment for debug messages
#define TIME_TASKS // Uncomment to get time information about tasks

#define NUMBER_OF_TASKS 7

// Task periods
#define READBUTTON_PERIOD 20
#define ACCEL1_PERIOD 33
#define ACCEL2_PERIOD 33
#define LCD_PERIOD 33
#define GAME_PERIOD 100

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