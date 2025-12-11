#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303.h> // Accelerometer utils
#include <Adafruit_GFX.h> // Auxilary Library for LCD
#include <Adafruit_PCD8544.h> // LCD utils
#include <Tasks.hpp>
#include <RMS.hpp>
#include <PCP.hpp>
#include <string.h>

// Debug
#define DEBUG // Uncomment for debug messages						 

void setup()
{
  Serial.begin(115200);
  delay(1000);  // Give serial time to initialize
  
  // I2C Accelerometer init
  Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

  // Check accelerometers
  if(!accel1.begin()) {
    Serial.println("main: Accelerometer 1 not found!");    
  } else {
    Serial.println("main: Accelerometer 1 initialized");
  }
  
  if(!accel2.begin()) {  // Use Wire1 for second accelerometer
    Serial.println("main: Accelerometer 2 not found!");
  } else {
    Serial.println("main: Accelerometer 2 initialized");
  }
  
  // Button init
  pinMode(SELBUTTON_PIN, INPUT_PULLUP);
  pinMode(CYCLEBUTTON_PIN, INPUT_PULLUP);
    
  // Task init
  createRmsTasks();  
  Serial.println("main: All tasks initialized.");

  // Mutex init
  pcp_mutex_init(&xButtonMutex , pcp_mutex_init_find_ceiling(xButtonMutex.mutexHandle));
  pcp_mutex_init(&xAccel1Mutex , pcp_mutex_init_find_ceiling(xAccel1Mutex.mutexHandle));
  pcp_mutex_init(&xAccel2Mutex , pcp_mutex_init_find_ceiling(xAccel2Mutex.mutexHandle));
  Serial.println("main: All mutexes initialized.");

  Serial.printf("main: Scheduler starting...\n");
}

void loop()
{
  // Empty - all work is done in FreeRTOS tasks
  // This task runs at lowest priority (1 by default)
  vTaskDelay(portMAX_DELAY);  // Sleep forever
}