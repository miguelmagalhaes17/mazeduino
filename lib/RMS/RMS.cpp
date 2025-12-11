#include "RMS.hpp"

// Task array
// Add NULL to end of mutex array, otherwise PCP functions die
RmsTask tasks[NUMBER_OF_TASKS] = {
  {TaskReadButtons , "ReadButtons" , 4096 , 0 , 0 , NULL , {&xButtonMutex , NULL}},
  {TaskReadAccel1 , "ReadAccel1" , 4096 , 0 , 0 , NULL , {&xAccel2Mutex , NULL}},
  {TaskReadAccel2 , "ReadAccel2" , 4096 , 0 , 0 , NULL , {&xAccel1Mutex , NULL}},
  {TaskDisplayLCD , "DisplayLCD" , 4096 , 0 , 0 , NULL , {NULL}},
  {TaskGameLogic , "GameLogic" , 4096 , 0 , 0 , NULL , {&xButtonMutex , &xAccel1Mutex , &xAccel2Mutex , NULL}},
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

// Finds highest priority task and assigns the ceiling accordingly
int pcp_mutex_init_find_ceiling(SemaphoreHandle_t handle)
{
    int k = 0;
    int highestPriority = 0;

    for(int i ; i < taskCount ; i++)
    {
        k = 0;
        while(tasks[i].mutexes[k] != NULL)
        {
            if(tasks[i].mutexes[k]->mutexHandle == handle && tasks[i].priority > highestPriority) 
                highestPriority = tasks[i].priority;
            k++;
        }
        
    }

    return highestPriority;
}