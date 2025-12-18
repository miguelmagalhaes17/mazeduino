#include "Tasks.hpp"

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

// Button related variables
bool readSelectButtonState = 0;
bool readCycleButtonState = 0;
bool selectButtonState = 0;
bool cycleButtonState = 0;
bool prevSelectButtonState = 0;
bool prevCycleButtonState = 0;

// Task Implementations
// Read both buttons
//void TaskReadButtons(void*)
//{
//  Serial.println("TaskReadButtons: Reading button states...");
//  readSelectButtonState = !digitalRead(SELBUTTON_PIN);
//  readCycleButtonState = !digitalRead(CYCLEBUTTON_PIN);
//  
//  if(readSelectButtonState != prevSelectButtonState)
//  {
//    pcp_mutex_lock(&xButtonMutex);
//    selectButtonState = readSelectButtonState;
//    prevSelectButtonState = readSelectButtonState;    
//    pcp_mutex_unlock(&xButtonMutex);
//  }
//  
//  if(readCycleButtonState != prevCycleButtonState)
//  {
//    pcp_mutex_lock(&xButtonMutex);
//    cycleButtonState = readCycleButtonState;
//    prevCycleButtonState = readCycleButtonState;
//    pcp_mutex_unlock(&xButtonMutex);
//  }
//
//  #ifdef DEBUG
//	Serial.printf("TaskReadButtons: Select Button: %d\n"
//                "Cycle Button: %d\n"
//                , selectButtonState , cycleButtonState);
//  #endif
//  
//}

void TaskReadButtons(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(50);
  for(;;){
    Serial.println("TaskReadButtons: Reading button states...");
    Serial.println(digitalRead(SELBUTTON_PIN));
    Serial.println(digitalRead(CYCLEBUTTON_PIN));
    Serial.println("----");
    // TODO
    // PA ESTA MERDA NAO FUNCIONA NÃO LÊ DIREITO
    readSelectButtonState = !digitalRead(SELBUTTON_PIN);
    readCycleButtonState = !digitalRead(CYCLEBUTTON_PIN);
    if(readSelectButtonState != prevSelectButtonState){
      pcp_mutex_lock(&xButtonMutex);
      selectButtonState = readSelectButtonState;
      prevSelectButtonState = readSelectButtonState;    
      pcp_mutex_unlock(&xButtonMutex);
    }
  
    if(readCycleButtonState != prevCycleButtonState)
    {
      pcp_mutex_lock(&xButtonMutex);
      cycleButtonState = readCycleButtonState;
      prevCycleButtonState = readCycleButtonState;
      pcp_mutex_unlock(&xButtonMutex);
    }

    #ifdef DEBUG
	  Serial.printf("TaskReadButtons: Select Button: %d\n"
                  "Cycle Button: %d\n"
                  , selectButtonState , cycleButtonState);
    #endif
    vTaskDelayUntil( &xLastWakeTime, xPeriod );}
}

// Read accelerometer 1
void TaskReadAccel1(void*)
{
  for(;;){
	  pcp_mutex_lock(&xAccel1Mutex);
    accel1.read();  
    pcp_mutex_unlock(&xAccel1Mutex);
    
	  #ifdef DEBUG
	  	Serial.printf("TaskReadAccel1: X=%.2f Y=%.2f Z=%.2f m/s^2 | ", 
	  				  accel1.accelData.x,
              accel1.accelData.y,
              accel1.accelData.z);		
	  #endif
  }
}

// Read accelerometer 2
void TaskReadAccel2(void*)
{
  for(;;){
	  pcp_mutex_lock(&xAccel2Mutex);
    accel2.read();
    pcp_mutex_unlock(&xAccel2Mutex);
    
	  #ifdef DEBUG	
	  	Serial.printf("TaskReadAccel2: X=%.2f Y=%.2f Z=%.2f m/s^2 | ", 
	  				  accel2.accelData.x,
              accel2.accelData.y,
              accel2.accelData.z);	
	  #endif
  }
}

// Write to LCD
void TaskDisplayLCD(void*)
{
  for(;;){}
	// lcd1 lcd2
}

// Run Game logic or Main Screen logic
void TaskGameLogic(void*)
{
  game_init();
  for(;;){
    game_render();
    pcp_mutex_lock(&xButtonMutex);
    bool localSelectButton = selectButtonState;
    bool localCycleButton = cycleButtonState;
    pcp_mutex_unlock(&xButtonMutex);

    game_update(localSelectButton, localCycleButton);

    
  }
}