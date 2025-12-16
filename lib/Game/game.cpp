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


void game_render() {
    switch(currentGameState) {
        case MENU:
            // code block
            lcd1.clearDisplay();
            lcd2.clearDisplay();
            lcd1.setCursor(0,0);
            lcd1.setTextSize(1);
            //fazer display de jogar
            lcd1.setTextColor(WHITE, BLACK); // 'inverted' text
            lcd1.println("PLAY");
            lcd1.println("outra merda qualquer");
            lcd1.display();
            // acho que agora a lógica deve ser tipo: o botão de select 
            //faz com que entre no texto - que corresponde a um estado - 
            //que está invertido,e o outro botão deve inverter as cores 
            //como tem no exemplo com as cores invertidas
            

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



