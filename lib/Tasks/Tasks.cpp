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


void TaskReadButtons(void*)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xPeriod = pdMS_TO_TICKS();
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

void TaskUpdateGamePhysics(void*){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(33);
    const float deltaTime = 0.033f;
    
    Serial.println("TaskUpdatePhysics: Task started");
    
    for(;;) {
        // Copy accel data
        pcp_mutex_lock(&xAccel1Mutex);
        float localAccel1X = gameState.accel1X;
        float localAccel1Y = gameState.accel1Y;
        pcp_mutex_unlock(&xAccel1Mutex);
        
        pcp_mutex_lock(&xAccel2Mutex);
        float localAccel2X = gameState.accel2X;
        float localAccel2Y = gameState.accel2Y;
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
    const TickType_t xPeriod = pdMS_TO_TICKS(50);
    
    Serial.println("TaskGameLogic: Task started");
    
    for(;;) {
        // Copy button states
        pcp_mutex_lock(&xButtonMutex);
        bool localSelectPressed = selectButtonPressed;
        bool localCyclePressed = cycleButtonPressed;
        
        // Clear edge-triggered flags
        selectButtonPressed = false;
        cycleButtonPressed = false;
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
    const TickType_t xPeriod = pdMS_TO_TICKS(33);
    
    Serial.println("TaskRenderLCD1: Task started");
    
    for(;;) {
        // Copy game state
        pcp_mutex_lock(&xGameStateMutex);
        GameState localState = gameState;
        pcp_mutex_unlock(&xGameStateMutex);
        
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
    const TickType_t xPeriod = pdMS_TO_TICKS(33);
    
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
