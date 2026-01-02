#include "RMS.hpp"

// Task array
// Add NULL to end of mutex array, otherwise PCP functions die
// IF CHANGE PERIOD HERE CHANGE IN THE TASKS
RmsTask tasks[NUMBER_OF_TASKS] = {
  {TaskReadButtons , "ReadButtons" , 4096 , 10 , 0 , NULL /*, NULL*/ , {&xButtonMutex , NULL}},
  //{TaskReadAccel1 , "ReadAccel1" , 4096 , 50 , 0 , NULL /*, NULL*/ , {&xAccel1Mutex , NULL}},
  //{TaskReadAccel2 , "ReadAccel2" , 4096 , 33 , 0 , NULL /*, NULL*/ , {&xAccel2Mutex , NULL}},
  //{TaskDisplayLCD , "DisplayLCD" , 4096 , 33 , 0 , NULL /*, NULL*/ , {NULL}},
  //{TaskGameLogic , "GameLogic" , 4096 , 33 , 0 , NULL /*, NULL*/ , {&xButtonMutex , &xAccel1Mutex , &xAccel2Mutex , NULL}},
};

// Task related varibles
int taskCount = NUMBER_OF_TASKS;//(tasks) / sizeof(tasks[0]);

// RMS Priority Assignment
void assignRmsPriorities() {
  Serial.println("assignRmsPriorities: Assigning RMS priorities...");
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
  UBaseType_t priority = taskCount + 1;
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
  Serial.printf("createRmsTasks: Creating %d RMS tasks...\n", taskCount);
  Serial.printf("sizeoftasks: %d\n", sizeof(tasks));
  Serial.printf("sieoftask[0]: %d\n", sizeof(tasks[0]));
  assignRmsPriorities();

  for(int i = 0; i < taskCount; i++) {
    Serial.printf("createRmsTasks: Creating task %s...\n", tasks[i].name);
    xTaskCreatePinnedToCore(
      tasks[i].function,
      tasks[i].name,
      tasks[i].stackSize,
      NULL,
      tasks[i].priority,
      &tasks[i].handle/*,
      1*/  // Run on core 1
    );

    vTaskSuspend(tasks[i].handle);
	
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
    int i = 0;

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