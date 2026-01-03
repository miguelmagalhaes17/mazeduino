#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303_Accel.h> // Accelerometer utils
#include <Adafruit_GFX.h> // Auxilary Library for LCD
#include <Adafruit_PCD8544.h> // LCD utils
#include <Tasks.hpp>
#include <RMS.hpp>
#include <PCP.hpp>
#include <Utils.hpp>
#include <game.hpp>

void setup()
{
  //Serial.begin(115200);
  Serial.begin(9600);
  delay(1000);  // Give serial time to initialize
  
  // I2C Accelerometer init (IF NOT USING ONE OF THE ACCELS COMMENT THE CORRESPODING Wire)
  Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  //Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

  // Check accelerometers
  if(!accel1.begin(LSM303_ADDRESS_ACCEL, &Wire)) {
    Serial.println("main: Accelerometer 1 not found!\n");    
  } else {
    Serial.println("main: Accelerometer 1 initialized\n");
  }
  
  /*if(!accel2.begin(LSM303_ADDRESS_ACCEL, &Wire1)) {  // Use Wire1 for second accelerometer
    Serial.println("main: Accelerometer 2 not found!\n");
  } else {
    Serial.println("main: Accelerometer 2 initialized\n");
  }*/

  // Button init
  pinMode(SELBUTTON_PIN, INPUT_PULLUP);
  pinMode(CYCLEBUTTON_PIN, INPUT_PULLUP);

  // Task init
  pcp_mutex_init(&xButtonMutex);
  pcp_mutex_init(&xAccel1Mutex);
  //pcp_mutex_init(&xAccel2Mutex);
  createRmsTasks();  
  pcp_mutex_set_ceiling(&xButtonMutex , pcp_mutex_init_find_ceiling(xButtonMutex.mutexHandle));
  pcp_mutex_set_ceiling(&xAccel1Mutex , pcp_mutex_init_find_ceiling(xAccel1Mutex.mutexHandle));
  
  Serial.println("main: All tasks initialized.\n");
  Serial.printf("main: Scheduler starting...\n");

  //for(int i = 0; i < NUMBER_OF_TASKS; i++) vTaskResume(tasks[i].handle);

}

void loop()
{
  // Empty - all work is done in FreeRTOS tasks
  // This task runs at lowest priority (1 by default)
  Serial.println("main: Loop running - sleeping forever.");
  vTaskDelay(portMAX_DELAY);  // Sleep forever
}