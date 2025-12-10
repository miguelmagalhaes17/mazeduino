#pragma once
#include <Arduino.h>

// Mutexes
extern SemaphoreHandle_t xButtonMutex;
extern SemaphoreHandle_t xAccel1Mutex;
extern SemaphoreHandle_t xAccel2Mutex;		