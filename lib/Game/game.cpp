#include "game.hpp"

GameState currentGameState = MENU;
ball gameBall;


//inicializar jogo
    // estado = MENU
    // posiçoes reset 
    // velocidades reset
    // raio default

void game_init(){
    currentGameState = MENU;
    gameBall.x = 0.0;
    gameBall.y = 0.0;
    gameBall.x_velocity = 0.0;
    gameBall.y_velocity = 0.0;
    gameBall.radius = 2;
}


void game_render(GameState state, Select selection, float ballX, float ballY) {
    Serial.println("game_render: Rendering game state...");
    switch(currentGameState) {
        case MENU:

            lcd1.clearDisplay();
            lcd1.setCursor(0,0);
            lcd1.setTextSize(2);
            // PLAY option
            if (selection == PLAY) {
                lcd1.setTextColor(WHITE, BLACK);  // Highlighted
            } else {
                lcd1.setTextColor(BLACK);         // Normal
            }
            lcd1.println("PLAY");
            
            // CREDITS option
            if (selection == CREDITS) {
                lcd1.setTextColor(WHITE, BLACK);  // Highlighted
            } else {
                lcd1.setTextColor(BLACK);         // Normal
            }
            lcd1.println("CREDITS");
            
            // METRICS option
            if (selection == METRICS) {
                lcd1.setTextColor(WHITE, BLACK);  // Highlighted
            } else {
                lcd1.setTextColor(BLACK);         // Normal
            }
            lcd1.println("METRICS");
            
            lcd1.display();

        break;

        case PLAYING:
            // code block
            // se calhar meter numa função separada e simplesmente chamar aqui start_game();
            // aqui pode ficar lógica para meter em pausa ou sair simplesmente com um if ou assim mas não sei como fazer por "evento"
        break;
        case PAUSED:
            // code block
            // dar só opção de continuar ou bazar
        break;
        case GAMEOVER:
            // code block
            // mostrar score, dar opção de reiniciar ou bazar
            // (spoiler não ha score vai ser tipo math.random() ou assim só para o troll?)
        break;
}
}

void game_update(bool selectButton, bool cycleButton) {
    Serial.println("game_update: Updating game state...");
    switch(currentGameState){
        case MENU:

        break;
    }
}