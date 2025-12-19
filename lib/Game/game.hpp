#pragma once
#include <Arduino.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>
#include <Tasks.hpp>

#define MAZE_WIDTH 16 // trocar depois
#define MAZE_HEIGHT 16 // trocar depois

extern Adafruit_PCD8544 lcd1;
extern Adafruit_PCD8544 lcd2;

enum Select {
    PLAY = 0,
    CREDITS = 1,
    METRICS = 2,
    SELECT_MAX
};

extern Select selectMode;

enum GameState {
    MENU = 0, 
    PLAYING = 1, 
    PAUSED = 2, 
    GAMEOVER = 3
};

struct ball {
    float x;
    float y;
    float radius;
    float x_velocity;
    float y_velocity;
};

struct DisplayState {
    GameState currentState;
    Select menuSelection;
    float ballX, ballY;
    //mais cenas de display que possamos querer
};

extern DisplayState displayState;

void game_init();
void game_render(GameState state, Select selection, float ballX, float ballY);
void game_update(bool selectButton, bool cycleButton);



extern GameState currentGameState;
extern ball gameBall;