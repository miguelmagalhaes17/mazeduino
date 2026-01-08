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
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW); // Debug pin to measure setup timing
  delay(1000);
  // I2C Accelerometer init (IF NOT USING ONE OF THE ACCELS COMMENT THE CORRESPODING Wire)
  //Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  //Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

  // Start both I2C buses
  I2C_0.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  delay(1000);
  I2C_1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);
  delay(1000);
  initAccel(I2C_0);
  delay(1000);
  initAccel(I2C_1);

  // Check accelerometers
  /*
  if(!accel1.begin(LSM303_ADDRESS_ACCEL, &Wire)) {
    Serial.printf("main: Accelerometer 1 not found!\n");    
  } else {
    Serial.printf("main: Accelerometer 1 initialized\n");
  }
  
  if(!accel2.begin(LSM303_ADDRESS_ACCEL, &Wire1)) {  // Use Wire1 for second accelerometer
    Serial.printf("main: Accelerometer 2 not found!\n");
  } else {
    Serial.printf("main: Accelerometer 2 initialized\n");
  }
  */

  // Button init
  pinMode(SELBUTTON_PIN, INPUT_PULLUP);
  pinMode(CYCLEBUTTON_PIN, INPUT_PULLUP);

  // LCD init (must be before any display calls)
  lcd1.begin();
  lcd1.setContrast(60);
  lcd1.display();
  delay(2000);
  lcd1.clearDisplay();

  lcd2.begin();
  lcd2.setContrast(60);
  lcd2.display();
  delay(2000);
  lcd2.clearDisplay();

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


  //Serial.printf("main: All tasks initialized and mutexes configured.\n");
  //Serial.printf("main: Starting scheduler and resuming tasks...\n");
  //vTaskStartScheduler();
  //Serial.printf("=== NUNCA DEVE CHEGAR AQUI CRLLLLLLLLLL ==="); // não é suposto passar o scheduler depois dele ser iniciado
  
  // NOW start all tasks - they have proper mutex ceilings and all tasks exist
  //startRmsTasks();
  //  ("main: Scheduler starting...\n");

}

void loop()
{
  #ifdef TIME_TASKS
    vTaskDelay(pdMS_TO_TICKS(TIME_TASKS_INFO_PRINT_TIME));
    Serial.print("-------------------------------------------------------------------\n");
    Serial.print("------------------------- TASK TIME INFO --------------------------\n");
    Serial.print("Task: Button task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttButtons.timeMin, round(ttButtons.timeSum/ttButtons.timeCount), ttButtons.timeMax);
    Serial.print("Task: Accel1 task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttAccel1.timeMin, round(ttAccel1.timeSum/ttAccel1.timeCount), ttAccel1.timeMax);
    Serial.print("Task: Accel2 task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttAccel2.timeMin, round(ttAccel2.timeSum/ttAccel2.timeCount), ttAccel2.timeMax);
    Serial.print("Task: LCD task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttLCD.timeMin, round(ttLCD.timeSum/ttLCD.timeCount), ttLCD.timeMax);
    Serial.print("Task: Game Physics task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttGamePhysics.timeMin, round(ttGamePhysics.timeSum/ttGamePhysics.timeCount), ttGamePhysics.timeMax);
    Serial.print("Task: Game Logic task\n");
    Serial.printf("Min: %d | Avg: %d | Max: %d |\n", ttGameLogic.timeMin, round(ttGameLogic.timeSum/ttGameLogic.timeCount), ttGameLogic.timeMax);
    Serial.print("-------------------------------------------------------------------\n");
    Serial.print("-------------------------------------------------------------------\n");
  #else
    // Empty - all work is done in FreeRTOS tasks
    // This task runs at lowest priority (1 by default)
    Serial.printf("main: Loop running - sleeping forever.");
    vTaskDelay(portMAX_DELAY);  // Sleep forever
  #endif
}