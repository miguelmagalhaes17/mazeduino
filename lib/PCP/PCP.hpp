#pragma once
#include <Arduino.h>

// PCP mutex struct
typedef struct PCPMutex {
    SemaphoreHandle_t mutexHandle;
    int mutexCeiling;
    int mutexSaved;
}PCPMutex;

// Mutexes
extern PCPMutex xButtonMutex;
extern PCPMutex xAccel1Mutex;
extern PCPMutex xAccel2Mutex;

// Function declaration
void pcp_mutex_init(PCPMutex* m, int ceiling);
void pcp_mutex_lock(PCPMutex* m);
void pcp_mutex_unlock(PCPMutex* m);