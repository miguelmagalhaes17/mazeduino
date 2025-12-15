#pragma once
#include <Arduino.h>

#define MAZE_WIDTH 16 // trocar depois
#define MAZE_HEIGHT 16 // trocar depois

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