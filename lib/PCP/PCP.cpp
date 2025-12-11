#include "PCP.hpp"

// Structure initialization
PCPMutex xButtonMutex;
PCPMutex xAccel1Mutex;
PCPMutex xAccel2Mutex;

// Mutex init
void pcp_mutex_init(PCPMutex* m, int ceiling)
{
    m->mutexHandle = xSemaphoreCreateMutex();
    
    if(m->mutexHandle == NULL) 
    {
        Serial.printf("pcp_mutex_init: failed to create mutex()");
        exit(EXIT_FAILURE);
    }

    m->mutexCeiling = ceiling;
    m->mutexSaved = 0;
}

// Lock mutex
// Can lead to deadlocks if lower priority task takes mutex and
// higher priority task starts executing before lower task's priority is changed
void pcp_mutex_lock(PCPMutex* m)
{
    xSemaphoreTake(m->mutexHandle, portMAX_DELAY);
    
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    m->mutexSaved = uxTaskPriorityGet(task);
    
    if(m->mutexSaved < m->mutexCeiling) vTaskPrioritySet(task, m->mutexCeiling);
}

// Unlock mutex
void pcp_mutex_unlock(PCPMutex* m)
{
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    
    if(m->mutexSaved > 0)
    {
        vTaskPrioritySet(task, m->mutexSaved);
        m->mutexSaved = 0;
    }
    
    xSemaphoreGive(m->mutexHandle);
}