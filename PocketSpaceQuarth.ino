#include <Arduino.h>
#include <PocketStar.h>
#include "DashBoard.h"
#include "PocketDisplay.h"
#include "GameControl.h"
#include "GameController.h"
#include "Highscore.h"

enum MAINSTATE{
    MAINSTATE_DASHBOARD,
    MAINSTATE_GAME,
    MAINSTATE_HIGHSCORE
};
enum MAINSTATE mainState = MAINSTATE_DASHBOARD;


DashBoard dBoard;
GameController game;
Highscore highscore;


long timer = 0;
#define FRAMES  21
void initialize() {
    pocketDisplay.setFont(pocketStar6pt);
    pocketDisplay.setFontBold(true, 0xa4);
    pocketDisplay.setFontColor(0xf0);
    mainState = MAINSTATE_DASHBOARD;
    dBoard.reset();
    
   
}

void update(float tpf) {
    if (millis() - timer < FRAMES && ! game.isSpeedUp()) return;
    pocketDisplay.clearBuffer();
    
    if (mainState == MAINSTATE_DASHBOARD){
        dBoard.draw();
        if (dBoard.m_startGame){
            mainState = MAINSTATE_GAME;
            game.reset(dBoard.m_startLevel, gameControl.gameMode);
        }
        if (dBoard.m_startHighScore){
            mainState = MAINSTATE_HIGHSCORE;
        }
        
    }
    if (mainState == MAINSTATE_GAME){
       
       game.calcGameLogic();
       game.draw();
      
       
       
       if (game.isGameOver()) {
           if (game.getPoints() > 0){
               mainState = MAINSTATE_HIGHSCORE; 
               highscore.addNewRecord(game.getPoints());
           }else mainState = MAINSTATE_DASHBOARD;
           dBoard.reset();
           
       }
    }
    if (mainState == MAINSTATE_HIGHSCORE){
       highscore.draw();
       
        if (highscore.m_closeHighScore){
            highscore.reset();
            dBoard.reset();
            mainState = MAINSTATE_DASHBOARD;
        }
    }
    pocketDisplay.show();
    timer = millis();
    
}

boolean pause() {
    return true;
}

void resume() {}