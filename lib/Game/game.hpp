#pragma once
#include <Arduino.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>

#define MAZE_WIDTH 16 // trocar depois
#define MAZE_HEIGHT 16 // trocar depois

extern Adafruit_PCD8544 lcd1;
extern Adafruit_PCD8544 lcd2;




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

void game_init();
void game_render();


extern GameState currentGameState;
extern ball gameBall;