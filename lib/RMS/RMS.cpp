#include "RMS.hpp"

// Task array
RmsTask tasks[NUMBER_OF_TASKS] = {
  {TaskReadButtons , "ReadButtons" , 4096 , 0 , 0 , NULL},
  {TaskReadAccel1 , "ReadAccel1" , 4096 , 0 , 0 , NULL},
  {TaskReadAccel2 , "ReadAccel2" , 4096 , 0 , 0 , NULL},
  {TaskDisplayLCD , "DisplayLCD" , 4096 , 0 , 0 , NULL},
  {TaskGameLogic , "GameLogic" , 4096 , 0 , 0 , NULL},
};

// Task related varibles
int taskCount = sizeof(tasks) / sizeof(tasks[0]);

// RMS Priority Assignment
void assignRmsPriorities() {
  // Sort tasks by period (shorter period = higher priority)
  for(int i = 0; i < taskCount - 1; i++) {
    for(int j = i + 1; j < taskCount; j++) {
      if(tasks[j].periodMs < tasks[i].periodMs) {
        RmsTask tmp = tasks[i];
        tasks[i] = tasks[j];
        tasks[j] = tmp;
      }
    }
  }

  // Assign priorities (highest number = highest priority in FreeRTOS)
  UBaseType_t priority = taskCount;
  for(int i = 0; i < taskCount; i++) {
    tasks[i].priority = priority--;
    
	#ifdef DEBUG
	  Serial.printf("assignRmsPriorities: Assigned %s priority %u, period %ums\n", 
                 tasks[i].name, tasks[i].priority, tasks[i].periodMs);
    #endif
  }
}

// Create all RMS tasks
void createRmsTasks() {
  assignRmsPriorities();

  for(int i = 0; i < taskCount; i++) {
    xTaskCreatePinnedToCore(
      tasks[i].function,
      tasks[i].name,
      tasks[i].stackSize,
      NULL,
      tasks[i].priority,
      &tasks[i].handle,
      1  // Run on core 1
    );
	
	#ifdef DEBUG
		Serial.printf("createRmsTasks: Created %s with priority %u, period %ums\n", 
                  tasks[i].name, tasks[i].priority, tasks[i].periodMs);
	#endif
  }
}