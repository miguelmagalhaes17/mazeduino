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

enum Select {
    PLAY = 0,
    CREDITS = 1
};

Select selectMode;


void game_render() {
    Serial.println("game_render: Rendering game state...");
    switch(currentGameState) {
        case MENU:
            // code block
            lcd1.clearDisplay();
            //lcd2.clearDisplay();
            lcd1.setCursor(0,0);
            lcd1.setTextSize(2);
            while(selectButtonState == false){
                if (cycleButtonState == false) {
                    selectMode = PLAY;
                
                    lcd1.setTextColor(WHITE, BLACK);
                    lcd1.println("PLAY");
                    lcd1.setTextColor(BLACK); // 'inverted' text
                    lcd1.println("CREDITS");

                } else {
                    selectMode = CREDITS;
                    lcd1.setTextColor(BLACK); // 'inverted' text
                    lcd1.println("PLAY");
                    lcd1.setTextColor(WHITE, BLACK);
                    lcd1.println("CREDITS");
                }
            }
            if (selectMode == PLAY) {
                currentGameState = PLAYING;
            } else {
                lcd1.clearDisplay();
                lcd1.setCursor(0,0);
                lcd1.setTextSize(1);    
                lcd1.println("Game by:");
                lcd1.println("André Osório");
                lcd1.println("Miguel Magalhães");
                lcd1.println("Rodrigo Pereira");
                lcd1.println("SEMB, FEUP 2025");
                lcd1.display();
                delay(3000);
                currentGameState = MENU;
            }



            

            // fazer o mesmo para o display 2

            //dar display


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
    switch(currentGameState)
}