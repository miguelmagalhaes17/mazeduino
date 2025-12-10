#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303.h> // Accelerometer utils
#include <Adafruit_GFX.h> // Auxilary Library for LCD
#include <Adafruit_PCD8544.h> // LCD utils
#include <Tasks.hpp>
#include <RMS.hpp>
#include <PCP.hpp>

// Debug
#define DEBUG // Uncomment for debug messages						 

void setup()
{
  Serial.begin(115200);
  
  // I2C Accelerometer init
  Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

  // Check accelerometers
  if(!accel1.begin()) {
    Serial.println("Accelerometer 1 not found!");
  } else {
    Serial.println("Accelerometer 1 initialized");
  }
  
  if(!accel2.begin()) {  // Use Wire1 for second accelerometer
    Serial.println("Accelerometer 2 not found!");
  } else {
    Serial.println("Accelerometer 2 initialized");
  }
  
  // Mutex init
  xButtonMutex = xSemaphoreCreateMutex();
  xAccel1Mutex = xSemaphoreCreateMutex();
  xAccel2Mutex = xSemaphoreCreateMutex();
  
  // Button init
  pinMode(SELBUTTON_PIN, INPUT_PULLUP);
  pinMode(CYCLEBUTTON_PIN, INPUT_PULLUP);
  
  delay(1000);  // Give serial time to initialize
  
  Serial.println("\n=== RMS Scheduler with Accelerometers ===");
  Serial.printf("Total tasks: %d\n", taskCount);
  
  // Create all RMS tasks
  createRmsTasks();
  
  Serial.println("All tasks created. Scheduler starting...\n");
}

void loop()
{
  // Empty - all work is done in FreeRTOS tasks
  // This task runs at lowest priority (1 by default)
  vTaskDelay(portMAX_DELAY);  // Sleep forever
}