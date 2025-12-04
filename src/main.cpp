#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM303.h>

// -----------------------------------------------------------------------------------------------------

// Accelerometer objects
Adafruit_LSM303 accel1;
Adafruit_LSM303 accel2;

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
  { TaskFast,   "FAST",    4096, 10,   0, NULL },
  { TaskMedium, "MEDIUM",  4096, 50,   0, NULL },
  { TaskSlow,   "SLOW",    4096, 200,  0, NULL }/*,
  { TaskAccel,  "ACCEL",   8192, 100,  0, NULL }  // Accelerometer reading task*/
};

int taskCount = sizeof(tasks) / sizeof(tasks[0]);
int selectButtonState = LOW;
int cycleButtonState = LOW;

// -----------------------------------------------------------------------------------------------------

// Task function declarations
void TaskFast(void* pv);
void TaskMedium(void* pv);
void TaskSlow(void* pv);
void TaskReadButtons(void* pv);
//void TaskAccel(void* pv);  // New accelerometer task

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
      1  // Run on core 1, leave core 0 for WiFi/BT if needed
    );

    Serial.printf("Created %s with priority %u, period %ums\n", 
                  tasks[i].name, tasks[i].priority, tasks[i].periodMs);
  }
}

// Task Implementations
void TaskFast(void* pv) {
  while(1) {
    Serial.println("Fast task");
    vTaskDelay(pdMS_TO_TICKS(tasks[0].periodMs));  // Use task's period
  }
}

void TaskMedium(void* pv) {
  while(1) {
    Serial.println("Medium task");
    vTaskDelay(pdMS_TO_TICKS(tasks[1].periodMs));
  }
}

void TaskSlow(void* pv) {
  while(1) {
    Serial.println("Slow task");
    vTaskDelay(pdMS_TO_TICKS(tasks[2].periodMs));
  }
}

/*void TaskReadButtons()
{
  selectButtonState = digitalRead(SELBUTTON_PIN);
  cycleButtonState = digitalRead(CYCLEBUTTON_PIN);
}
void TaskReadAccel1()
{

}
void TaskReadAccel2()
{

}
void TaskDisplayLCD()
{

}
void TaskGameLogic()
{

}*/

/*
void TaskAccel(void* pv) {
  // Initialize accelerometers
  Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);
  
  if(!accel1.begin()) {
    Serial.println("Accelerometer 1 not found!");
  } else {
    Serial.println("Accelerometer 1 initialized");
  }
  
  if(!accel2.begin(&Wire1)) {  // Use Wire1 for second accelerometer
    Serial.println("Accelerometer 2 not found!");
  } else {
    Serial.println("Accelerometer 2 initialized");
  }
  
  while(1) {
    // Read from first accelerometer
    sensors_event_t event1;
    accel1.getEvent(&event1);
    
    Serial.printf("Accel1: X=%.2f Y=%.2f Z=%.2f m/s^2 | ", 
                  event1.acceleration.x, 
                  event1.acceleration.y, 
                  event1.acceleration.z);
    
    // Read from second accelerometer
    sensors_event_t event2;
    accel2.getEvent(&event2);
    
    Serial.printf("Accel2: X=%.2f Y=%.2f Z=%.2f m/s^2\n", 
                  event2.acceleration.x, 
                  event2.acceleration.y, 
                  event2.acceleration.z);
    
    vTaskDelay(pdMS_TO_TICKS(tasks[3].periodMs));  // 100ms period
  }
}
*/
void setup()
{
  Serial.begin(115200);
  
  // I2C Accelerometer init
  Wire.begin(ACCEL1_SDA_PIN, ACCEL1_SCL_PIN);
  Wire1.begin(ACCEL2_SDA_PIN, ACCEL2_SCL_PIN);

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