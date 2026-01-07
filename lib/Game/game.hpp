#pragma once
#include <Arduino.h>
//#include <Adafruit_PCD8544.h>
//#include <Adafruit_GFX.h>
#include <Tasks.hpp>

#define LCD_WIDTH 84 // trocar depois
#define LCD_HEIGHT 48 // trocar depois

#define BALL_RADIUS 2

#define MAZE_CELL_SIZE 2
#define MAZE_COLS (LCD_WIDTH / MAZE_CELL_SIZE)
#define MAZE_ROWS (LCD_HEIGHT / MAZE_CELL_SIZE)

#define NUMBER_OF_MAZES 3

extern Adafruit_PCD8544 lcd1;
extern Adafruit_PCD8544 lcd2;

enum GameMode {
    MENU = 0, 
    PLAYING = 1, 
    PAUSED = 2, 
    GAMEOVER = 3
};

enum MenuSelect {
    PLAY = 0,
    CREDITS = 1,
    SELECT_MAX = 2
};

struct Player {
    float x;
    float y;
    float velX;
    float velY;
    bool hasFinished;
    uint32_t finishTime;

};

struct Maze {
    bool walls[MAZE_ROWS][MAZE_COLS];
    int startX;
    int startY;
    int finishX;
    int finishY;
    int finishWidth;
    int finishHeight;
};

struct GameState {
    GameMode mode;
    MenuSelect menuSelection;
    
    Player player1;
    Player player2;
    
    Maze maze;
    
    int winner;  // 0 = no winner, 1 = player1, 2 = player2
    uint32_t gameStartTime;  // When game started (in ms)
    
    // Accelerometer readings (copied from sensor tasks)
    float accel1X, accel1Y;
    float accel2X, accel2Y;
};

extern GameState gameState;
extern Maze allMazes[NUMBER_OF_MAZES];
extern Maze maze1;
extern Maze maze2;
extern Maze maze3;

void game_init();
void game_generate_maze();
void game_update_menu(bool selectPressed, bool cyclePressed);
void game_update_physics(float deltaTime);
void game_update_logic(bool selectPressed, bool cyclePressed);
void game_render_player1();
void game_render_player2();

bool check_wall_collision(float x, float y, float radius);
bool check_finish_collision(float x, float y, float radius);

//void render_maze(Adafruit_PCD8544& lcd);
//void render_menu(Adafruit_PCD8544& lcd, MenuSelect selection);
//void render_gameover(Adafruit_PCD8544& lcd, int winner);
