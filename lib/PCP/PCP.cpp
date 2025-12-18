#include "PCP.hpp"

// Structure initialization
PCPMutex xButtonMutex;
PCPMutex xAccel1Mutex;
PCPMutex xAccel2Mutex;

// Mutex init
void pcp_mutex_init(PCPMutex* m)
{
    Serial.println("pcp_mutex_init: Initializing mutex...");
    m->mutexHandle = xSemaphoreCreateMutex();
    
    if(m->mutexHandle == NULL) 
    {
        Serial.printf("pcp_mutex_init: failed to create mutex()");
        exit(EXIT_FAILURE);
    }

    m->currentTask = NULL;
    m->mutexCeiling = 0;
    m->mutexSaved = 0;
}

// Lock mutex
// Can lead to deadlocks if lower priority task takes mutex and
// higher priority task starts executing before lower task's priority is changed
// Possible change: First check if mutex available, then change priority to ceiling
// and only then lock the mutex
void pcp_mutex_lock(PCPMutex* m)
{
    xSemaphoreTake(m->mutexHandle, portMAX_DELAY);
    
    m->currentTask = xTaskGetCurrentTaskHandle();
    m->mutexSaved = uxTaskPriorityGet(m->currentTask);
    
    if(m->mutexSaved < m->mutexCeiling) vTaskPrioritySet(m->currentTask, m->mutexCeiling);
}

// Unlock mutex
void pcp_mutex_unlock(PCPMutex* m)
{    
    if(m->mutexSaved > 0)
    {        
        vTaskPrioritySet(m->currentTask, m->mutexSaved);
        m->mutexSaved = 0;
    }

    m->currentTask = NULL;

    xSemaphoreGive(m->mutexHandle);
}

void pcp_mutex_set_ceiling(PCPMutex* m, int ceiling)
{
    m->mutexCeiling = ceiling;
}