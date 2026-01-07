#include "Tasks.hpp"

// Accelerometer objects
//Adafruit_LSM303_Accel_Unified accel1(12345);
//Adafruit_LSM303_Accel_Unified accel2(67890);

// Accel events
//sensors_event_t eventAccel1;
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

//AccelerometerData accel1;
//AccelerometerData accel2;


void TaskReadButtons(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(READBUTTON_PERIOD);
  
  for(;;)
  {
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
        readData(accel1, I2C_0);
        // Serial.printf("TaskReadAccel1: X=%.2f Y=%.2f Z=%.2f R=%.2f P=%.2f \n", 
        //                     accel1.x,
        //             accel1.y,
        //             accel1.z,
        //             accel1.roll,
        //             accel1.pitch);
    pcp_mutex_unlock(&xAccel1Mutex);
    
	  #ifdef DEBUG
	  	Serial.printf("TaskReadAccel1: X=%.2f Y=%.2f Z=%.2f R=%.2f P=%.2f \n", 
	  				  accel1.x,
              accel1.y,
              accel1.z,
              accel1.roll,
              accel1.pitch);	
	  #endif
    vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
}

// Read accelerometer 2
void TaskReadAccel2(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS(ACCEL2_PERIOD);  
    for(;;)
    {
        pcp_mutex_lock(&xAccel2Mutex);
        readData(accel2, I2C_1);
        // Serial.printf("TaskReadAccel2: X=%.2f Y=%.2f Z=%.2f R=%.2f P=%.2f \n", 
        //                 accel2.x,
        //         accel2.y,
        //         accel2.z,
        //         accel2.roll,
        //         accel2.pitch);
        pcp_mutex_unlock(&xAccel2Mutex);
        
        vTaskDelayUntil( &xLastWakeTime, xPeriod );
  }
}

void TaskUpdateGamePhysics(void*){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(LCD_PERIOD);
    const float deltaTime = 0.033f;
    
    Serial.println("TaskUpdatePhysics: Task started");
    
    for(;;) {
        // Copy accel data
        pcp_mutex_lock(&xAccel1Mutex);
        float localAccel1X = accel1.x;
        float localAccel1Y = accel1.y;

        Serial.printf("TaskUpdatePhysics: Accel1 X=%.2f Y=%.2f\n", localAccel1X, localAccel1Y);

        pcp_mutex_unlock(&xAccel1Mutex);
        
        pcp_mutex_lock(&xAccel2Mutex);
        float localAccel2X = accel2.x;
        float localAccel2Y = accel2.y;
        pcp_mutex_unlock(&xAccel2Mutex);
        
        // Update physics with deltaTime
        pcp_mutex_lock(&xGameStateMutex);
        gameState.accel1X = localAccel1X;
        gameState.accel1Y = localAccel1Y;
        gameState.accel2X = localAccel2X;
        gameState.accel2Y = localAccel2Y;
        
        game_update_physics(deltaTime);  // <-- Calls game.cpp function
        pcp_mutex_unlock(&xGameStateMutex);

        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void TaskGameLogic(void* pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(GAME_PERIOD);
    
    Serial.println("TaskGameLogic: Task started");

    //game_init();
    
    for(;;) {
        // Copy button states
        pcp_mutex_lock(&xButtonMutex);
        bool localSelectPressed = selectButtonState;
        bool localCyclePressed = cycleButtonState;
        
        // Clear edge-triggered flags
        selectButtonState = false;
        cycleButtonState = false;
        pcp_mutex_unlock(&xButtonMutex);
        
        // Update game logic
        pcp_mutex_lock(&xGameStateMutex);
        game_update_logic(localSelectPressed, localCyclePressed);
        pcp_mutex_unlock(&xGameStateMutex);
        
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// Write to LCD
void TaskRenderLCD1(void* pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(LCD_PERIOD);
    
    //Serial.println("TaskRenderLCD1: Task started");
    
    for(;;) {
        // Copy game state
        pcp_mutex_lock(&xGameStateMutex);
        GameState localState = gameState;
        pcp_mutex_unlock(&xGameStateMutex);

        //Serial.println("TaskRenderLCD1: Rendering frame");
        
        // Now render based on game mode
        switch (localState.mode) {
            case MENU:
                // Render menu
                lcd1.clearDisplay();
                lcd1.setTextSize(1);
                lcd1.setTextColor(BLACK);
                lcd1.setCursor(10, 5);
                lcd1.println("MAZE RACE");
                
                lcd1.setCursor(15, 20);
                if (localState.menuSelection == PLAY) {
                    lcd1.setTextColor(WHITE, BLACK);
                } else {
                    lcd1.setTextColor(BLACK);
                }
                lcd1.println("PLAY");
                
                lcd1.setCursor(15, 30);
                if (localState.menuSelection == CREDITS) {
                    lcd1.setTextColor(WHITE, BLACK);
                } else {
                    lcd1.setTextColor(BLACK);
                }
                lcd1.println("CREDITS");
                lcd1.display();
                break;
    
            case PLAYING:
            case PAUSED:
                // Render game
                lcd1.clearDisplay();
                
                // Draw maze walls
                for (int row = 0; row < MAZE_ROWS; row++) {
                    for (int col = 0; col < MAZE_COLS; col++) {
                        if (localState.maze.walls[row][col]) {
                            lcd1.fillRect(col * MAZE_CELL_SIZE, row * MAZE_CELL_SIZE,
                                        MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLACK);
                        }
                    }
                }
                
                // Draw finish zone
                lcd1.fillRect(localState.maze.finishX, localState.maze.finishY,
                            localState.maze.finishWidth, localState.maze.finishHeight, BLACK);
                
                // Draw player 1's ball
                lcd1.fillCircle((int)localState.player1.x, (int)localState.player1.y, 
                               BALL_RADIUS, WHITE);
                lcd1.drawCircle((int)localState.player1.x, (int)localState.player1.y, 
                               BALL_RADIUS, BLACK);
                
                // Show PAUSED text if paused
                if (localState.mode == PAUSED) {
                    lcd1.setTextSize(1);
                    lcd1.setTextColor(WHITE, BLACK);
                    lcd1.setCursor(25, 20);
                    lcd1.println("PAUSED");
                }
                
                lcd1.display();
                break;
                
            case GAMEOVER:
                // Render game over
                lcd1.clearDisplay();
                lcd1.setTextSize(1);
                lcd1.setTextColor(BLACK);
                
                lcd1.setCursor(10, 10);
                lcd1.println("GAME OVER!");
                
                lcd1.setCursor(10, 22);
                lcd1.printf("Player %d", localState.winner);
                lcd1.setCursor(10, 30);
                lcd1.println("WINS!");
                
                lcd1.setCursor(5, 42);
                lcd1.println("Press SELECT");
                
                lcd1.display();
                break;
        }
        
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

void TaskRenderLCD2(void* pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(LCD_PERIOD);
    
    Serial.println("TaskRenderLCD2: Task started");
    
    for(;;) {
        // Copy game state
        pcp_mutex_lock(&xGameStateMutex);
        GameState localState = gameState;
        pcp_mutex_unlock(&xGameStateMutex);
        
        // Render based on game mode
        switch (localState.mode) {
            case MENU:
                // Render menu
                lcd2.clearDisplay();
                lcd2.setTextSize(1);
                lcd2.setTextColor(BLACK);
                lcd2.setCursor(10, 5);
                lcd2.println("MAZE RACE");
                
                lcd2.setCursor(15, 20);
                if (localState.menuSelection == PLAY) {
                    lcd2.setTextColor(WHITE, BLACK);
                } else {
                    lcd2.setTextColor(BLACK);
                }
                lcd2.println("PLAY");
                
                lcd2.setCursor(15, 30);
                if (localState.menuSelection == CREDITS) {
                    lcd2.setTextColor(WHITE, BLACK);
                } else {
                    lcd2.setTextColor(BLACK);
                }
                lcd2.println("CREDITS");
                lcd2.display();
                break;
                
            case PLAYING:
            case PAUSED:
                // Render game
                lcd2.clearDisplay();
                
                // Draw maze walls
                for (int row = 0; row < MAZE_ROWS; row++) {
                    for (int col = 0; col < MAZE_COLS; col++) {
                        if (localState.maze.walls[row][col]) {
                            lcd2.fillRect(col * MAZE_CELL_SIZE, row * MAZE_CELL_SIZE,
                                        MAZE_CELL_SIZE, MAZE_CELL_SIZE, BLACK);
                        }
                    }
                }
                
                // Draw finish zone
                lcd2.fillRect(localState.maze.finishX, localState.maze.finishY,
                            localState.maze.finishWidth, localState.maze.finishHeight, BLACK);
                
                // Draw player 2's ball (not player 1!)
                lcd2.fillCircle((int)localState.player2.x, (int)localState.player2.y, 
                               BALL_RADIUS, WHITE);
                lcd2.drawCircle((int)localState.player2.x, (int)localState.player2.y, 
                               BALL_RADIUS, BLACK);
                
                // Show PAUSED text if paused
                if (localState.mode == PAUSED) {
                    lcd2.setTextSize(1);
                    lcd2.setTextColor(WHITE, BLACK);
                    lcd2.setCursor(25, 20);
                    lcd2.println("PAUSED");
                }
                
                lcd2.display();
                break;
                
            case GAMEOVER:
                // Render game over
                lcd2.clearDisplay();
                lcd2.setTextSize(1);
                lcd2.setTextColor(BLACK);
                
                lcd2.setCursor(10, 10);
                lcd2.println("GAME OVER!");
                
                lcd2.setCursor(10, 22);
                lcd2.printf("Player %d", localState.winner);
                lcd2.setCursor(10, 30);
                lcd2.println("WINS!");
                
                lcd2.setCursor(5, 42);
                lcd2.println("Press SELECT");
                
                lcd2.display();
                break;
        }
        
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
