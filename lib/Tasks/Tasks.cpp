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
void TaskReadButtons(void*)
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
    if(xSemaphoreTake(xButtonMutex, portMAX_DELAY))
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
void TaskReadAccel1(void*)
{
	if(xSemaphoreTake(xAccel1Mutex, portMAX_DELAY))
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
void TaskReadAccel2(void*)
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
void TaskDisplayLCD(void*)
{
	// lcd1 lcd2
}

// Run Game logic or Main Screen logic
void TaskGameLogic(void*)
{

}