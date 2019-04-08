/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameConfig.h
 * Author: Schuemi
 *
 * Created on 4. April 2019, 20:31
 */

#ifndef GAMECONTROL_H
#define GAMECONTROL_H
#include <stdint.h>

class GameControl {
public:
    GameControl();
    GameControl(const GameControl& orig);
    virtual ~GameControl();
    
    bool audio;
   
    int  gameMode;
    void pollButtons();
    uint8_t justPressed(uint8_t btn);
    uint8_t justReleased(uint8_t btn);
    uint8_t pressed(uint8_t btn);
    uint8_t audioToggle();
    uint8_t vibrationToggle();
private:
    uint8_t previousButtonState;
    uint8_t pressedBtns;
};
extern GameControl gameControl;
#endif /* GAMECONTROL_H */

