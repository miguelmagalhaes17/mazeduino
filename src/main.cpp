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
#include <Accel_Utils.hpp>

void setup()
{
  Serial.begin(9600);
  delay(1000);  // Give serial time to initialize
  
  // I2C Accelerometer init (IF NOT USING ONE OF THE ACCELS COMMENT THE CORRESPODING Wire)
  //Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  //Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

  // Start both I2C buses
  I2C_0.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  I2C_1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);
  initAccel(I2C_0);
  initAccel(I2C_1);

  // Check accelerometers
  /*
  if(!accel1.begin(LSM303_ADDRESS_ACCEL, &Wire)) {
    Serial.println("main: Accelerometer 1 not found!\n");    
  } else {
    Serial.println("main: Accelerometer 1 initialized\n");
  }
  
  if(!accel2.begin(LSM303_ADDRESS_ACCEL, &Wire1)) {  // Use Wire1 for second accelerometer
    Serial.println("main: Accelerometer 2 not found!\n");
  } else {
    Serial.println("main: Accelerometer 2 initialized\n");
  }
  */

  // Button init
  pinMode(SELBUTTON_PIN, INPUT_PULLUP);
  pinMode(CYCLEBUTTON_PIN, INPUT_PULLUP);

  // LCD init (must be before any display calls)
  lcd1.begin();
  lcd1.setContrast(60);
  lcd1.clearDisplay();
  lcd1.display();

  lcd2.begin();
  lcd2.setContrast(60);
  lcd2.clearDisplay();
  lcd2.display();

  // Game state init
  game_init();

  // Task init
  pcp_mutex_init(&xButtonMutex);
  pcp_mutex_init(&xAccel1Mutex);
  pcp_mutex_init(&xAccel2Mutex);
  pcp_mutex_init(&xGameStateMutex);
  pcp_mutex_init(&xDisplayStateMutex);

  
  
  // Create tasks in SUSPENDED state
  createRmsTasks();

 
  
  // Configure mutex ceilings BEFORE starting tasks
  pcp_mutex_set_ceiling(&xButtonMutex , pcp_mutex_init_find_ceiling(xButtonMutex.mutexHandle));
  pcp_mutex_set_ceiling(&xAccel1Mutex , pcp_mutex_init_find_ceiling(xAccel1Mutex.mutexHandle));
  pcp_mutex_set_ceiling(&xAccel2Mutex , pcp_mutex_init_find_ceiling(xAccel2Mutex.mutexHandle));
  pcp_mutex_set_ceiling(&xGameStateMutex , pcp_mutex_init_find_ceiling(xGameStateMutex.mutexHandle));
  pcp_mutex_set_ceiling(&xDisplayStateMutex , pcp_mutex_init_find_ceiling(xDisplayStateMutex.mutexHandle));

  //vTaskStartScheduler();
  
  Serial.println("main: All tasks initialized and mutexes configured.\n");
  Serial.printf("main: Starting scheduler and resuming tasks...\n");
  
  // NOW start all tasks - they have proper mutex ceilings and all tasks exist
  //startRmsTasks();
  //Serial.printf("main: Scheduler starting...\n");

  

}

void loop()
{
  // Empty - all work is done in FreeRTOS tasks
  // This task runs at lowest priority (1 by default)
  Serial.println("main: Loop running - sleeping forever.");
  vTaskDelay(portMAX_DELAY);  // Sleep forever
}