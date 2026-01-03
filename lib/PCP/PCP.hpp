#pragma once
#include <Arduino.h>
#include <Utils.hpp>

// PCP mutex struct
typedef struct PCPMutex {
    SemaphoreHandle_t mutexHandle;
    TaskHandle_t currentTask;
    int mutexCeiling;
    int mutexSaved;
}PCPMutex;

// Mutexes
extern PCPMutex xButtonMutex;
extern PCPMutex xAccel1Mutex;
extern PCPMutex xAccel2Mutex;
extern PCPMutex xDisplayStateMutex; 
extern PCPMutex xGameStateMutex;

// Function declaration
void pcp_mutex_init(PCPMutex* m);
void pcp_mutex_lock(PCPMutex* m);
void pcp_mutex_unlock(PCPMutex* m);
void pcp_mutex_set_ceiling(PCPMutex* m, int ceiling);