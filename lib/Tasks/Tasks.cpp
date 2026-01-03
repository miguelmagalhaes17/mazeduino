#include "Tasks.hpp"

// Accelerometer objects
Adafruit_LSM303_Accel_Unified accel1(12345);
//Adafruit_LSM303_Accel_Unified accel2(67890);

// Accel events
sensors_event_t eventAccel1;
//sensors_event_t eventAccel2;

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
void TaskReadButtons(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(READBUTTON_PERIOD);
  for(;;){
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
	  Serial.printf("TaskReadButtons: \nSelect Button: %d\n"
                  "Cycle Button: %d\n"
                  , selectButtonState , cycleButtonState);
    #endif
    vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
}

// Read accelerometer 1
void TaskReadAccel1(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(ACCEL1_PERIOD);
  for(;;){
	  pcp_mutex_lock(&xAccel1Mutex);
    accel1.getEvent(&eventAccel1);
    pcp_mutex_unlock(&xAccel1Mutex);
    
	  #ifdef DEBUG
	  	Serial.printf("TaskReadAccel1: X=%.2f Y=%.2f Z=%.2f m/s^2 \n", 
	  				  eventAccel1.acceleration.x,
              eventAccel1.acceleration.y,
              eventAccel1.acceleration.z);	
	  #endif
    vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
}

/*// Read accelerometer 2
void TaskReadAccel2(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(ACCEL2_PERIOD);  
  for(;;){
	  pcp_mutex_lock(&xAccel2Mutex);
    accel1.getEvent(&eventAccel2);
    pcp_mutex_unlock(&xAccel2Mutex);
    
	  #ifdef DEBUG	
	  	Serial.printf("TaskReadAccel2: X=%.2f Y=%.2f Z=%.2f m/s^2 \n", 
	  				  eventAccel2.acceleration.x,
              eventAccel2.acceleration.y,
              eventAccel2.acceleration.z);
	  #endif
  }
  vTaskDelayUntil( &xLastWakeTime, xPeriod );
}*/

// Write to LCD
void TaskDisplayLCD(void*)
{
  /*TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(LCD_PERIOD);*/
  for(;;){
  /*  pcp_mutex_lock(&xDisplayStateMutex);
    Select localSelection = displayState.menuSelection;
    pcp_mutex_unlock(&xDisplayStateMutex);

    game_render(displayState.currentState, localSelection, displayState.ballX, displayState.ballY);
    vTaskDelayUntil( &xLastWakeTime, xPeriod );*/
  }
	// lcd1 lcd2
}

// Run Game logic or Main Screen logic
void TaskGameLogic(void*)
{
  /*TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(GAME_PERIOD);*/
  //game_init();
  for(;;){
    /*game_render();
    pcp_mutex_lock(&xButtonMutex);
    bool localSelectButton = selectButtonState;
    bool localCycleButton = cycleButtonState;
    pcp_mutex_unlock(&xButtonMutex);

    game_update(localSelectButton, localCycleButton);

    vTaskDelayUntil( &xLastWakeTime, xPeriod );*/ 
  }
}