#pragma once
#include <Arduino.h>
#include <Tasks.hpp>
#include <PCP.hpp>
#include <vector>
#include <Utils.hpp>

#define NUMBER_OF_TASKS 1

struct PCPMutex; // Forward declaration to avoid circular includes

// RMS Task Structure
typedef struct RmsTask {
  TaskFunction_t function;
  const char* name;
  uint32_t stackSize;
  uint32_t periodMs;  
  UBaseType_t priority;
  TaskHandle_t handle;
  //TaskTiming taskTime;
  std::vector<PCPMutex*> mutexes;
} RmsTask;

// Task array
extern RmsTask tasks[NUMBER_OF_TASKS];

// Task related varibles
extern int taskCount;

// RMS function declaration
void assignRmsPriorities();
void createRmsTasks();
int pcp_mutex_init_find_ceiling(SemaphoreHandle_t handle); // Used here to avoid circular dependecy because I eat glue