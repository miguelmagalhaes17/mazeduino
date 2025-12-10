#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM303.h> // Accelerometer utils
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h> // LCD utils

// -----------------------------------------------------------------------------------------------------

// Pin definitions
// Accelerometer Pins
#define ACCEL1_SDA_PIN 12
#define ACCEL1_SCL_PIN 13
#define ACCEL2_SDA_PIN 14
#define ACCEL2_SCL_PIN 15

// Button Pins
#define SELBUTTON_PIN 16
#define CYCLEBUTTON_PIN 17

// LCD Pins
#define LCD1_CLK_PIN 11
#define LCD1_DIN_PIN 10
#define LCD1_DC_PIN 9
#define LCD1_CS_PIN 8
#define LCD1_RST_PIN 7
#define LCD2_CLK_PIN 6
#define LCD2_DIN_PIN 5
#define LCD2_DC_PIN 4
#define LCD2_CS_PIN 3
#define LCD2_RST_PIN 2

// -----------------------------------------------------------------------------------------------------

// Debug
#define DEBUG // Uncomment for debug messages

// RMS Task Structure
typedef struct RmsTask {
  TaskFunction_t function;
  const char* name;
  uint32_t stackSize;
  uint32_t periodMs;
  UBaseType_t priority;
  TaskHandle_t handle;
} RmsTask;

// Task array
RmsTask tasks[] = {
  //{ TaskFast,   "FAST",    4096, 10,   0, NULL }
};

// Task related varibles
int taskCount = sizeof(tasks) / sizeof(tasks[0]);

// Button related variables
bool readSelectButtonState = 0;
bool readCycleButtonState = 0;
bool selectButtonState = 0;
bool cycleButtonState = 0;
bool prevSelectButtonState = 0;
bool prevCycleButtonState = 0;

// Mutexes
SemaphoreHandle_t  xButtonMutex = NULL;
SemaphoreHandle_t  xAccel1Mutex = NULL;
SemaphoreHandle_t  xAccel2Mutex = NULL;

// Accelerometer objects
Adafruit_LSM303 accel1;
Adafruit_LSM303 accel2;

// LCD objects
Adafruit_PCD8544 lcd1 = Adafruit_PCD8544(LCD1_CLK_PIN,
										 LCD1_DIN_PIN,
										 LCD1_DC_PIN,
										 LCD1_CS_PIN,
										 LCD1_RST_PIN);										
Adafruit_PCD8544 lcd2 = Adafruit_PCD8544(LCD2_CLK_PIN,
										 LCD2_DIN_PIN,
										 LCD2_DC_PIN,
										 LCD2_CS_PIN,
										 LCD2_RST_PIN);										 

// -----------------------------------------------------------------------------------------------------

// Function declarations
void assignRmsPriorities();
void createRmsTasks();
void TaskReadButtons();

// -----------------------------------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------------------------------

// RMS Priority Assignment
void assignRmsPriorities() {
  // Sort tasks by period (shorter period = higher priority)
  for(int i = 0; i < taskCount - 1; i++) {
    for(int j = i + 1; j < taskCount; j++) {
      if(tasks[j].periodMs < tasks[i].periodMs) {
        RmsTask tmp = tasks[i];
        tasks[i] = tasks[j];
        tasks[j] = tmp;
      }
    }
  }

  // Assign priorities (highest number = highest priority in FreeRTOS)
  UBaseType_t priority = taskCount;
  for(int i = 0; i < taskCount; i++) {
    tasks[i].priority = priority--;
    
	#ifdef DEBUG
	  Serial.printf("assignRmsPriorities: Assigned %s priority %u, period %ums\n", 
                 tasks[i].name, tasks[i].priority, tasks[i].periodMs);
    #endif
  }
}

// Create all RMS tasks
void createRmsTasks() {
  assignRmsPriorities();

  for(int i = 0; i < taskCount; i++) {
    xTaskCreatePinnedToCore(
      tasks[i].function,
      tasks[i].name,
      tasks[i].stackSize,
      NULL,
      tasks[i].priority,
      &tasks[i].handle,
      1  // Run on core 1
    );
	
	#ifdef DEBUG
		Serial.printf("createRmsTasks: Created %s with priority %u, period %ums\n", 
                  tasks[i].name, tasks[i].priority, tasks[i].periodMs);
	#endif
  }
}

// Task Implementations
// Read both buttons
void TaskReadButtons()
{
  readSelectButtonState = !digitalRead(SELBUTTON_PIN);
  readCycleButtonState = !digitalRead(CYCLEBUTTON_PIN);
  
  if(readSelectButtonState != prevSelectButtonState)
  {
    if (xSemaphoreTake(xButtonMutex, portMAX_DELAY))
    {
      selectButtonState = readSelectButtonState;
      prevSelectButtonState = readSelectButtonState;
      xSemaphoreGive(xButtonMutex);
    }
  }
  
  if(readCycleButtonState != prevCycleButtonState)
  {
    if (xSemaphoreTake(xButtonMutex, portMAX_DELAY))
    {
      cycleButtonState = readCycleButtonState;
      prevCycleButtonState = readCycleButtonState;
      xSemaphoreGive(xButtonMutex);		
    }
  }
  
  #ifdef DEBUG
	Serial.printf("TaskReadButtons: Select Button: %d\n"
                "Cycle Button: %d\n"
                , selectButtonState , cycleButtonState);
  #endif
  
}

// Read accelerometer 1
void TaskReadAccel1()
{
	if (xSemaphoreTake(xAccel1Mutex, portMAX_DELAY))
	{
		accel1.read();
		xSemaphoreGive(xAccel1Mutex);
	}
	
	#ifdef DEBUG
		Serial.printf("TaskReadAccel1: X=%.2f Y=%.2f Z=%.2f m/s^2 | ", 
					  accel1.accelData.x,
            accel1.accelData.y,
            accel1.accelData.z);		
	#endif
}

// Read accelerometer 2
void TaskReadAccel2()
{
	if (xSemaphoreTake(xAccel2Mutex, portMAX_DELAY))
	{
		accel2.read();
		xSemaphoreGive(xAccel2Mutex);
	}	
	
	#ifdef DEBUG	
		Serial.printf("TaskReadAccel2: X=%.2f Y=%.2f Z=%.2f m/s^2 | ", 
					  accel2.accelData.x,
            accel2.accelData.y,
            accel2.accelData.z);	
	#endif
}

// Write to LCD
void TaskDisplayLCD()
{
	// lcd1 lcd2
}

// Run Game logic or Main Screen logic
void TaskGameLogic()
{

}
