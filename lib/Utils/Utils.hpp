#pragma once
#include <Arduino.h>
#include <string>

//#define DEBUG // Uncomment for debug messages
//#define TIME_TASKS // Uncomment to get time information about tasks
#define TIME_TASKS_INFO_PRINT_TIME 30000 // in ms

#define NUMBER_OF_TASKS 1

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
  uint64_t timeStart = 0;
  uint64_t timeEnd = 0;
  int timeMax = 0;
  int timeMin = 999999;

} TaskTiming;

void time_calculations(TaskTiming* tt);
void print_time_calculations(String taskName, TaskTiming* tt);

/* In micros (us)
Buttons: Iter: 2002 | Max: 10 | Min: 2 | Avg: 4.436563
Accel1: Iter: 2002 | Max: 1098 | Min: 1084 | Avg: 1087.698853
Accel2: Iter: 2002 | Max: 1098 | Min: 1083 | Avg: 1087.681763
LCD1: Iter: 2002 | Max: 2160 | Min: 2142 | Avg: 2148.965088
LCD2: Iter: 2002 | Max: 2151 | Min: 2135 | Avg: 2142.605469
GameLogic: Iter: 2002 | Max: 20 | Min: 18 | Avg: 19.998001
Physics: Iter: 2002 | Max: 22 | Min: 15 | Avg: 16.350149
*/