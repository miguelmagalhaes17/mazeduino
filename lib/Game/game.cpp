#include "game.hpp"
#include <math.h>

GameState gameState;

const uint8_t PREDEFINED_MAZE[MAZE_ROWS][MAZE_COLS] =
{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};






void game_init(){
    gameState.mode = MENU;
    gameState.menuSelection = PLAY;
    gameState.winner = 0;
    gameState.gameStartTime = 0;

    //PLAYERS
    gameState.player1.x = 0;
    gameState.player1.y = 0;
    gameState.player1.velX = 0;
    gameState.player1.velY = 0;
    gameState.player1.hasFinished = false;
    gameState.player1.finishTime = 0;

    gameState.player2.x = 0;
    gameState.player2.y = 0;
    gameState.player2.velX = 0;
    gameState.player2.velY = 0;
    gameState.player2.hasFinished = false;
    gameState.player2.finishTime = 0;

    gameState.accel1X = gameState.accel1Y = 0.0f;
    gameState.accel2X = gameState.accel2Y = 0.0f;

    game_generate_maze();
}

void game_generate_maze() {
    Serial.println("game_generate_maze: Loading maze...");
    
    // Copy predefined maze
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            gameState.maze.walls[row][col] = PREDEFINED_MAZE[row][col];
        }
    }

    
    // Start position: top-left corner
    gameState.maze.startX = MAZE_CELL_SIZE + BALL_RADIUS;  // Inside first cell
    gameState.maze.startY = MAZE_CELL_SIZE + BALL_RADIUS;
    
    gameState.maze.finishX = LCD_WIDTH - (MAZE_CELL_SIZE * 3);
    gameState.maze.finishY = LCD_HEIGHT - (MAZE_CELL_SIZE * 3);
    gameState.maze.finishWidth = MAZE_CELL_SIZE;
    gameState.maze.finishHeight = MAZE_CELL_SIZE;
    
    Serial.printf("game_generate_maze: Start=(%d,%d) Finish=(%d,%d)\n",
                  gameState.maze.startX, gameState.maze.startY,
                  gameState.maze.finishX, gameState.maze.finishY);
}

// ============================================================================ //
// MENU LOGIC                                                                   //
// ============================================================================ //
void game_update_menu(bool selectPressed, bool cyclePressed) {
    if (cyclePressed) {
        gameState.menuSelection = (MenuSelect)((gameState.menuSelection + 1) % SELECT_MAX);
        Serial.printf("game_update_menu: Menu selection changed to %d\n", gameState.menuSelection);
    }
    
    if (selectPressed) 
    {
        switch (gameState.menuSelection) {
            case PLAY:
                gameState.mode = PLAYING;
                gameState.winner = 0;
                gameState.gameStartTime = millis();
                
                // Reset players to start position
                gameState.player1.x = gameState.maze.startX;
                gameState.player1.y = gameState.maze.startY;
                gameState.player1.velX = 0.0f;
                gameState.player1.velY = 0.0f;
                gameState.player1.hasFinished = false;
                
                gameState.player2.x = gameState.maze.startX;
                gameState.player2.y = gameState.maze.startY;
                gameState.player2.velX = 0.0f;
                gameState.player2.velY = 0.0f;
                gameState.player2.hasFinished = false;
                
                Serial.println("game_update_menu: Starting game!");
                break;
                
            case CREDITS:
                gameState.mode = CREDITS;
                if (selectPressed) {
                    gameState.mode = MENU;
                }
                // wtv
                break;
        }
    }
}

// ============================================================================ //
// PHYSICS UPDATE - Updates both players                                        //
// ============================================================================ //
void game_update_physics(float deltaTime) {
    if (gameState.mode != PLAYING || gameState.winner != 0) {
        return;  // Don't update physics if not playing or game is over
    }
    
    // Physics constants
    const float ACCEL_SCALE = 30.0f;      // Scale accelerometer input
    const float MAX_VELOCITY = 50.0f;     // Max pixels per second
    const float FRICTION = 0.92f;         // Velocity decay per frame
    
    // ======== UPDATE PLAYER 1 ========
    if (!gameState.player1.hasFinished) {
        // Update velocity based on accelerometer
        gameState.player1.velX += gameState.accel1X * ACCEL_SCALE * deltaTime;
        gameState.player1.velY += gameState.accel1Y * ACCEL_SCALE * deltaTime;
        
        // Apply friction
        gameState.player1.velX *= FRICTION;
        gameState.player1.velY *= FRICTION;
        
        // Clamp velocity
        float speed1 = sqrt(gameState.player1.velX * gameState.player1.velX + 
                           gameState.player1.velY * gameState.player1.velY);
        if (speed1 > MAX_VELOCITY) {
            gameState.player1.velX = (gameState.player1.velX / speed1) * MAX_VELOCITY;
            gameState.player1.velY = (gameState.player1.velY / speed1) * MAX_VELOCITY;
        }
        
        // Calculate new position
        float newX1 = gameState.player1.x + gameState.player1.velX * deltaTime;
        float newY1 = gameState.player1.y - gameState.player1.velY * deltaTime;
        
        // Check wall collision before moving
        if (!check_wall_collision(newX1, gameState.player1.y, BALL_RADIUS)) {
            gameState.player1.x = newX1;
        } else {
            gameState.player1.velX = 0;  // Stop horizontal movement
        }
        
        if (!check_wall_collision(gameState.player1.x, newY1, BALL_RADIUS)) {
            gameState.player1.y = newY1;
        } else {
            gameState.player1.velY = 0;  // Stop vertical movement
        }
        
        // Check finish line
        if (check_finish_collision(gameState.player1.x, gameState.player1.y, BALL_RADIUS)) {
            gameState.player1.hasFinished = true;
            gameState.player1.finishTime = millis() - gameState.gameStartTime;
            gameState.winner = 1;
            Serial.printf("PLAYER 1 WINS! Time: %lu ms\n", gameState.player1.finishTime);
        }
    }
    
    // ======== UPDATE PLAYER 2 ========
    if (!gameState.player2.hasFinished) {
        // Update velocity based on accelerometer
        gameState.player2.velX += gameState.accel2X * ACCEL_SCALE * deltaTime;
        gameState.player2.velY += gameState.accel2Y * ACCEL_SCALE * deltaTime;
        
        // Apply friction
        gameState.player2.velX *= FRICTION;
        gameState.player2.velY *= FRICTION;
        
        // Clamp velocity
        float speed2 = sqrt(gameState.player2.velX * gameState.player2.velX + 
                           gameState.player2.velY * gameState.player2.velY);
        if (speed2 > MAX_VELOCITY) {
            gameState.player2.velX = (gameState.player2.velX / speed2) * MAX_VELOCITY;
            gameState.player2.velY = (gameState.player2.velY / speed2) * MAX_VELOCITY;
        }
        
        // Calculate new position
        float newX2 = gameState.player2.x + gameState.player2.velX * deltaTime;
        float newY2 = gameState.player2.y - gameState.player2.velY * deltaTime;
        
        // Check wall collision before moving
        if (!check_wall_collision(newX2, gameState.player2.y, BALL_RADIUS)) {
            gameState.player2.x = newX2;
        } else {
            gameState.player2.velX = 0;  // Stop horizontal movement
        }
        
        if (!check_wall_collision(gameState.player2.x, newY2, BALL_RADIUS)) {
            gameState.player2.y = newY2;
        } else {
            gameState.player2.velY = 0;  // Stop vertical movement
        }
        
        // Check finish line (only if player 1 hasn't won yet)
        if (gameState.winner == 0 && 
            check_finish_collision(gameState.player2.x, gameState.player2.y, BALL_RADIUS)) {
            gameState.player2.hasFinished = true;
            gameState.player2.finishTime = millis() - gameState.gameStartTime;
            gameState.winner = 2;
            Serial.printf("PLAYER 2 WINS! Time: %lu ms\n", gameState.player2.finishTime);
        }
    }
}

// ============================================================================
// GAME LOGIC UPDATE
// ============================================================================
void game_update_logic(bool selectPressed, bool cyclePressed) {
    switch (gameState.mode) {
        case MENU:
            game_update_menu(selectPressed, cyclePressed);
            break;
            
        case PLAYING:
            if (selectPressed) {
                gameState.mode = PAUSED;
                Serial.println("game_update_logic: Game paused");
            }
            
            // Check if someone won
            if (gameState.winner != 0) {
                gameState.mode = GAMEOVER;
            }
            break;
            
        case PAUSED:
            if (selectPressed) {
                gameState.mode = PLAYING;
                Serial.println("game_update_logic: Game resumed");
            }
            if (cyclePressed) {
                gameState.mode = MENU;
                Serial.println("game_update_logic: Returning to menu");
            }
            break;
            
        case GAMEOVER:
            if (selectPressed) {
                gameState.mode = MENU;
                Serial.println("game_update_logic: Returning to menu from game over");
            }
            break;
    }
}

// ============================================================================
// COLLISION DETECTION
// ============================================================================

// Check if a circle at (x, y) with given radius collides with any wall
bool check_wall_collision(float x, float y, float radius) {
    // Check bounds first
    if (x - radius < 0 || x + radius >= LCD_WIDTH ||
        y - radius < 0 || y + radius >= LCD_HEIGHT) {
        return true;
    }
    
    // Check the cells around the ball
    int minCol = (int)((x - radius) / MAZE_CELL_SIZE);
    int maxCol = (int)((x + radius) / MAZE_CELL_SIZE);
    int minRow = (int)((y - radius) / MAZE_CELL_SIZE);
    int maxRow = (int)((y + radius) / MAZE_CELL_SIZE);
    
    // Clamp to maze bounds
    minCol = max(0, min(MAZE_COLS - 1, minCol));
    maxCol = max(0, min(MAZE_COLS - 1, maxCol));
    minRow = max(0, min(MAZE_ROWS - 1, minRow));
    maxRow = max(0, min(MAZE_ROWS - 1, maxRow));
    
    // Check each cell
    for (int row = minRow; row <= maxRow; row++) {
        for (int col = minCol; col <= maxCol; col++) {
            if (gameState.maze.walls[row][col]) {
                // This cell is a wall - check if ball overlaps with it
                int cellX = col * MAZE_CELL_SIZE;
                int cellY = row * MAZE_CELL_SIZE;
                
                // Find closest point in cell to ball center
                float closestX = max((float)cellX, min(x, (float)(cellX + MAZE_CELL_SIZE)));
                float closestY = max((float)cellY, min(y, (float)(cellY + MAZE_CELL_SIZE)));
                
                // Check if closest point is within ball radius
                float distSq = (x - closestX) * (x - closestX) + (y - closestY) * (y - closestY);
                if (distSq < radius * radius) {
                    return true;  // Collision detected
                }
            }
        }
    }
    
    return false;  // No collision
}

// Check if ball reached the finish zone
bool check_finish_collision(float x, float y, float radius) {
    return (x >= gameState.maze.finishX && 
            x <= gameState.maze.finishX + gameState.maze.finishWidth &&
            y >= gameState.maze.finishY && 
            y <= gameState.maze.finishY + gameState.maze.finishHeight);
}