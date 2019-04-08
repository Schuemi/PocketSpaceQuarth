/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameConfig.cpp
 * Author: Schuemi
 * 
 * Created on 4. April 2019, 20:31
 */

#include "GameControl.h"
#include <PocketStar.h>
#include "constants.h"
GameControl gameControl;

GameControl::GameControl() {
    audio = true;
    
    gameMode = GAME_MODE_EASY;
    pressedBtns = 0xff;
}

GameControl::GameControl(const GameControl& orig) {
}

GameControl::~GameControl() {
}
void GameControl::pollButtons() {
    previousButtonState = pressedBtns;
    pressedBtns = pocketstar.getButtons();
    
}
uint8_t GameControl::justPressed(uint8_t btn) {
    return (!(previousButtonState & btn) && (pressedBtns & btn));
}
uint8_t GameControl::justReleased(uint8_t btn)
{
  return ((previousButtonState & btn) && !(pressedBtns & btn));
}

uint8_t GameControl::pressed(uint8_t btn) {
    return pocketstar.getButtons(btn);
}

uint8_t GameControl::audioToggle() {
    if (audio) audio = false; else audio = true;
}
uint8_t GameControl::vibrationToggle() {
    if (pocketstar.vibrationEnabled) pocketstar.setVibrationEnabled(false); else pocketstar.setVibrationEnabled(true);
}

