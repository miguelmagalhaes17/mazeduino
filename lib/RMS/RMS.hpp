#pragma once
#include <Arduino.h>
#include <Tasks.hpp>
#include <Priority_Ceiling_Protocol.hpp>

#define NUMBER_OF_TASKS

// RMS Task Structure
typedef struct RmsTask {
  TaskFunction_t function;
  const char* name;
  uint32_t stackSize;
  uint32_t periodMs;
  UBaseType_t priority;
  TaskHandle_t handle;
} RmsTask;

// Task array
extern RmsTask tasks[NUMBER_OF_TASKS];

// Task related varibles
extern int taskCount;

// RMS function declaration
void assignRmsPriorities();
void createRmsTasks();