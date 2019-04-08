/*
 * The MIT License
 *
 * Copyright 2017 Schuemi.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   GameController.cpp
 * Author: Schuemi
 * 
 * Created on 10. Dezember 2017, 18:16
 */

#include "GameController.h"
#include <Arduino.h>
#include "Levels.h"
#include "PocketDisplay.h"
#include "GameControl.h"
#include "Utils.h"
#include <PocketStar.h>
#include "FakeEEPROM.h"


#define GAME_DATA_MAGIC 55556


const uint8_t PROGMEM GameController::spriteShot[] = {
0x00,0x8c,0x00,0x8c,0xa0,0x8c,0x00,0x8c,0x00
};

const uint8_t PROGMEM GameController::brick[] = {
0xfa,0xfa,0xfa,0xf0,0xf0,0xf0,0xa4,0xa4,0xa4
};
const uint8_t PROGMEM GameController::brickDissApp[] = {
0xfa,0xfa,0xc8,0xfa,0xf0,0xa4,0xc8,0xa4,0xa4
};

const uint8_t PROGMEM GameController::crosshair[] = {
0xda,0x00,0xda,0x00,0x00,0x00,0xda,0x00,0xda
};
const uint8_t PROGMEM GameController::brickSpezial[] = {
0xda,0xda,0xda,0xda,0xc4,0xda,0xda,0xda,0xda
};
const uint8_t PROGMEM GameController::border[] = { //9x18
0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x00,0x00,0x00,0x24,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x20,0x20,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x20,0x20,0x24,0x24,0x24,0x24,0x24,0x24,0x20,0x20,0x20,0x24,0x24,0x24,0x24,0x24,0x24,0x20,0x20,0x20,0x24,0x24,0x24,0x24,0x24,0x24,0x20,0x20,0x20,0x24,0x24,0x24,0x20,0x20,0x20,0x20,0x20,0x20,0x24,0x24,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x24,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
};

GameController::GameController() {
  
   
  
   m_spaceShip = new SpaceShip(m_gameData.m_shipXPosition);
   m_starField = new StarField(GAMEFIELD_X, 0, 57, 64, 1);
   
   reset(1, GAME_MODE_NONE);
   
    
    
}


GameController::~GameController() {
}

void GameController::drawBorder() {
    if (m_gameData.m_bgScroll > 0) {
        for (int x= 0; x<4; x++){
            pocketDisplay.drawBitmap(x*9,0,border + ((18-m_gameData.m_bgScroll)*9), 9, m_gameData.m_bgScroll);
        }
    }
    for (int y= 0; y<4; y++){
        for (int x= 0; x<4; x++){
            pocketDisplay.drawBitmap(x*9,(y*18)+m_gameData.m_bgScroll,border, 9, 18);
        }
    }
    m_gameData.m_bgScroll++;
    if (m_gameData.m_bgScroll == 18) m_gameData.m_bgScroll = 0;
}

void GameController::reset(uint8_t startLevel, uint8_t gameMode) {
    
    
    //if (! haveSavedGame()) {
        
        
        
        uint8_t* d = (uint8_t*)&m_gameData; for (uint16_t i = 0; i < sizeof(gameData); i++) d[i] = 0;
        m_gameData.m_newObjectNumber = 1;
        m_gameData.m_shipXPosition = 8;
        m_gameLoaded = false;
        m_gameData.m_currentLevel = startLevel;
      
        m_gameData.m_gameMode = gameMode;
        
              
        m_gameData.m_dataMagic = GAME_DATA_MAGIC;
    //} else {
       // EEPROM.get(EEPROM_GAMEDATA_BYTE + EEPROM_STORAGE_SPACE_START, m_gameData);
    //    m_gameLoaded = true;
    //}
    m_spaceShip->reset();
    m_noCollision = 0;
    m_spaceShip->setyPos((0 - (m_gameData.m_scrollPosition - 234)));
    getLcharacteristics(m_gameData.m_currentLevel, m_gameData.m_currentLevelStage, m_gameData.m_gameMode, &m_currentLevelCharacteristics);
    
}



void GameController::calcGameLogic() {
    
    gameControl.pollButtons();
    /*if (gameControl.justPressed(ButtonB)){  // z on the emu
       // if (m_gameData.m_stopScrolling) m_gameData.m_stopScrolling = false; else m_gameData.m_stopScrolling = true; // test cheat
       if (m_gamePaused) {
        m_gamePaused = false; 
          deleteSavedGame();
       }else {
           m_gamePaused = true; 
           // if (EEPROM.read(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START))
           //     EEPROM.put(EEPROM_GAMEDATA_BYTE + EEPROM_STORAGE_SPACE_START, m_gameData);
           
       } 
    }*/
    if (m_gamePaused) return;
    
    
    m_starField->move();
    
    if (m_gameData.m_showLevelCleared) return;
    
    
    if (m_gameData.m_highestBrick > 21) {
        m_spaceShip->setyPos(3 * (m_gameData.m_highestBrick - 21));
    } else { m_spaceShip->setyPos(0);}
    if (! m_noCollision) {
        if (m_gameData.m_exploding || m_gameData.m_highestBrick == 25) {
            m_gameData.m_speedUp = 0;
            m_gameData.m_exploding++;
            m_gameData.m_stopScrolling = true;
            if (m_gameData.m_exploding == 1) {
                // m_tones->tone(400, 300, 200, 300, 150, 600);
                
                pocketstar.vibrate(true);
            }
            if (m_gameData.m_exploding == 30){
                pocketstar.vibrate(false);
            }
            if (m_gameData.m_exploding == 100){
               // g_arduboy->setRGBled(0,0,0);
                m_gameData.m_gameOver = true;
                
            }
            
            m_spaceShip->doExplode();
            return;
        }
    }
    
    
   
    if (m_gameData.m_shipXPosition > 0){
        if (gameControl.justPressed(ButtonLeft)){
            m_gameData.m_shipXPosition--;
            m_gameData.m_keyHoldCounter = 0;
        } else {
            if (gameControl.pressed(ButtonLeft) && m_gameData.m_keyHoldCounter++ > 3){
                m_gameData.m_keyHoldCounter = 0;
                m_gameData.m_shipXPosition--;
            }
        }
    }
    if ( m_gameData.m_shipXPosition < 15){
        if (gameControl.justPressed(ButtonRight)){
            m_gameData.m_shipXPosition++;
            m_gameData.m_keyHoldCounter = 0;
        } else {
            if (gameControl.pressed(ButtonRight) && m_gameData.m_keyHoldCounter++ > 3){
                m_gameData.m_keyHoldCounter = 0;
                m_gameData.m_shipXPosition++;
            }
        }
    }
    
    m_gameData.m_speedUp =gameControl.pressed(ButtonUp);
    
    
    
    
    
    
    if (gameControl.justPressed(ButtonA)){ // x on the emu
        for(uint8_t i = 0; i < MAX_SHOOTS_AT_ONCE; i++) {
          
            
            if (! bitRead(m_gameData.m_shotActive, i)){
               
               // m_tones->tone(900, 35, 800, 35, 400, 35);
                m_gameData.m_shotPositionX[i] = GAMEFIELD_X + 5 + (m_gameData.m_shipXPosition*3);
                m_gameData.m_shotPositionY[i] = 53;
                if (m_gameData.m_highestBrick > 21) {
                    m_gameData.m_shotPositionY[i] += 3 * (m_gameData.m_highestBrick - 21);
                }
                bitSet(m_gameData.m_shotActive, i);
                
                break;
            }
        }
    }
    
    
    
    moveShots(false);
    moveShots(false);
    
    
    
}

void GameController::drawCrosshair() {
    int16_t bmpx = (m_gameData.m_shipXPosition*3)+GAMEFIELD_X + 5;
    for (int i = GAMEFIELD_BRICKS_HEIGHT- 1; i > 2; i--) {
        if (getBrick(m_gameData.m_shipXPosition, i)){
            int16_t bmpy = ((i + 1)*3) - 15; // hide the first 5 bricks
            if (bmpy > 0)
                pocketDisplay.drawBitmap(bmpx,bmpy,crosshair, 3, 3);
            
            return;
        }
        
    }
    pocketDisplay.drawBitmap(bmpx,0,crosshair, 3, 3);
}
void GameController::moveShots(bool collisionCheckOnly) {
    
    for(uint8_t i = 0; i < MAX_SHOOTS_AT_ONCE; i++) {
        if (bitRead(m_gameData.m_shotActive, i) &&  m_gameData.m_shotPositionY[i] > 0) {
            if (!collisionCheckOnly) m_gameData.m_shotPositionY[i]--;
            // is there a brick we are hitting?
            uint8_t py = (m_gameData.m_shotPositionY[i] / 3) + 5;
            uint8_t px = (m_gameData.m_shotPositionX[i] - GAMEFIELD_X - 5) / 3;
            if (py < GAMEFIELD_BRICKS_HEIGHT && px < GAMEFIELD_BRICKS_WIDTH){
                uint8_t b = getBrick(px, py-1);
                if (b > 0 && b <= 127){
                    setBrick(px, py, b);
                    bitClear(m_gameData.m_shotActive, i);
                    checkForQuarth(px, py);
                }
                if (b > 127){
                    bitClear(m_gameData.m_shotActive, i);
                    
                }
            }
        }
        else 
           bitClear(m_gameData.m_shotActive, i); 
    }
    
}
//int c = 0; // to test a object
void GameController::createLine() {
    
     // to test a object
    /*c++;
    if (c == 1){
        insertObject(4, 0, 3, false);
        insertObject(4, 3, 9, true);
        insertObject(4, 4, 10, false);
    }*/
    
    uint8_t obj[5];
    
    for (uint8_t i = 0; i < 5; i++) obj[i] = 0;
    
    uint8_t objCount = 0;
    uint8_t width = m_gameData.m_drawingObject;
    if (width > m_currentLevelCharacteristics.objectMaxWidth) width = m_currentLevelCharacteristics.objectMaxWidth;
    
    int8_t xFrom = (8 - (m_currentLevelCharacteristics.objectMaxWidth / 2));// - (m_gameData.m_drawingObject / 2);
    int8_t xTo = (8 + (m_currentLevelCharacteristics.objectMaxWidth / 2) - 1);// + (m_gameData.m_drawingObject / 2);
    bool spezial = false;
    if (m_gameData.m_currentLevelStage > 4 && ! random(30)) 
        spezial = true;
        
    if (m_currentLevelCharacteristics.tightLevel < 10) if (random(2) == 1) return;
    
    
    
    if (xFrom < 0) xFrom =0;
    if (xTo > 15) xTo = 15;
    int mid = xFrom + ((xTo - xFrom) / 2) - 3;
        
        for (uint8_t i = xFrom; i < xTo; i+=random(3) + 1) {
            if (m_currentLevelCharacteristics.tightLevel > 10 || random(m_currentLevelCharacteristics.tightLevel + 1) != 1){
                for (int o = 0; o < m_currentLevelCharacteristics.tightLevel; o++){
                    uint8_t objectNumber = random(10) + 1;
                    uint8_t pos = mid + i;
                    if (pos > xTo) pos = (xFrom + (pos - xTo)) - 1;
                    if (!isValueInArray(obj, objCount, objectNumber)) {
                    if (insertObject(pos, 0, objectNumber, spezial)){
                        obj[objCount++] = objectNumber;
                        m_gameData.m_drawingObjectCount++;
                      }
                    }
                if (m_gameData.m_drawingObjectCount >= m_currentLevelCharacteristics.objectMaxCount) return;
                }
               
            }
        }
    
}

uint32_t GameController::getPoints(){ return m_gameData.m_points; }
bool GameController::isGameOver() { return m_gameData.m_gameOver; }
bool GameController::isSpeedUp(){ return m_gameData.m_speedUp; }
    
void GameController::deleteSavedGame() {
   // if (EEPROM.read(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START))
   //     EEPROM.update(EEPROM_GAMEDATA_BYTE + EEPROM_STORAGE_SPACE_START + ((uint16_t)&m_gameData.m_dataMagic - (uint16_t)&m_gameData), (uint16_t)0);
        
    
}
bool GameController::haveSavedGame() {
   // if (! EEPROM.read(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START)) return false;
   // EEPROM.get(EEPROM_GAMEDATA_BYTE + EEPROM_STORAGE_SPACE_START + ((uint16_t)&m_gameData.m_dataMagic - (uint16_t)&m_gameData), m_gameData.m_dataMagic);
   // if (m_gameData.m_dataMagic == GAME_DATA_MAGIC) return true;
    return false;
}


void GameController::draw() {
    
    if (m_gamePaused) {
        
        pocketDisplay.setCursor(48, 20);
        pocketDisplay.print(F("PAUSED"));
        
        
       
        return;
    }
    
    
  //  if (m_gameData.m_currentLevel == 1) g_arduboy->drawBitmap(92,25,l1rPic, 36, 39);
  //  if (m_gameData.m_currentLevel == 2) g_arduboy->drawBitmap(92,22,l3rPic, 36, 42);
  //  if (m_gameData.m_currentLevel == 3) g_arduboy->drawBitmap(92,21,l4rPic, 36, 43);
//    if (m_gameData.m_currentLevel == 4) g_arduboy->drawBitmap(98,20,l5rPic, 26, 45);
//    if (m_gameData.m_currentLevel == 5) g_arduboy->drawBitmap(94,22,l6rPic, 36, 42);
//    if (m_gameData.m_currentLevel == 6) g_arduboy->drawBitmap(94,21,l7rPic, 36, 44);
//    if (m_gameData.m_currentLevel == 7) g_arduboy->drawBitmap(94,24,l8rPic, 36, 40);
//    if (m_gameData.m_currentLevel == 8) g_arduboy->drawBitmap(94,19,l9rPic, 36, 46);
    
    
    
    if (! m_gameData.m_bricksAreDisappearing && ! m_gameData.m_stopScrolling) m_gameData.m_scrollMinPosition++;
   
    if (! m_gameData.m_bricksOnField && m_gameData.m_currentLevelStage >= 10) {
        m_gameData.m_sezialDrawn = false;
        m_gameData.m_currentLevelStage = 0;
        m_gameData.m_showLevelCleared = true;
        m_gameData.m_scrollPosition = 0;
        m_gameData.m_speedUp = false;
    }
    
    if (m_gameData.m_showLevelCleared) {
        m_gameData.m_scrollPosition++;
        m_starField->draw(m_gameData.m_speedUp);
        
        if (m_gameData.m_scrollPosition < 234) {
            if (m_gameData.m_scrollPosition < 40)  pocketDisplay.setCursor(44, 64 - m_gameData.m_scrollPosition);
            if (m_gameData.m_scrollPosition >= 40 && m_gameData.m_scrollPosition < 100)   pocketDisplay.setCursor(44, 24);
            if (m_gameData.m_scrollPosition >= 100)   pocketDisplay.setCursor(44, 24 - (m_gameData.m_scrollPosition - 100));
            pocketDisplay.print(F("CONGRATS!"));
        } else if (m_gameData.m_scrollPosition < 300)
        {
            int8_t wait = (40 - (m_gameData.m_scrollPosition - 234)) / 2;
            
            m_gameData.m_speedUp = true;
            
            if (wait > 0) delay(wait);
            m_spaceShip->setyPos(0 - (m_gameData.m_scrollPosition - 234));
        } else if (m_gameData.m_scrollPosition < 350 && m_gameData.m_currentLevel < 8){
           
            pocketDisplay.setCursor(54, 20);
            pocketDisplay.print(F("NEXT"));
            pocketDisplay.setCursor(51, 28);
            pocketDisplay.print(F("LEVEL"));
          
            
           m_gameData.m_speedUp = false;
        } else if (m_gameData.m_scrollPosition < 360 && m_gameData.m_currentLevel < 8){
           if (m_gameData.m_currentLevel < 8) m_spaceShip->setyPos(360 - m_gameData.m_scrollPosition);
        } else if (m_gameData.m_scrollPosition < 900 && m_gameData.m_currentLevel == 8){
           
             pocketDisplay.setCursor(37, 20);
                pocketDisplay.print(F("YOU SAVED"));
                pocketDisplay.setCursor(54, 28);
                pocketDisplay.print(F("THE"));
                 pocketDisplay.setCursor(46, 36);
                pocketDisplay.print(F("GALAXY!"));
        } else if (m_gameData.m_scrollPosition < 1500 && m_gameData.m_currentLevel == 8){
            m_gameData.m_speedUp = true;
            m_spaceShip->setyPos(10 - ((m_gameData.m_scrollPosition - 900)) / 5);
            
        } else if (m_gameData.m_scrollPosition == 1500 && m_gameData.m_currentLevel == 8){
            // EEPROM.update(EEPROM_GAME_CLEARED_BYTE + EEPROM_STORAGE_SPACE_START, 0x32); 
            m_gameData.m_gameOver = true;
        }
        
        
        
        if (m_gameData.m_scrollPosition == 360 && m_gameData.m_currentLevel < 8){
            m_gameData.m_showLevelCleared = 0;
            m_gameData.m_scrollPosition = 0;
            m_gameData.m_currentLevel++;
            if (m_gameData.m_currentLevel == 10) m_gameData.m_gameOver = true;
            m_gameData.m_currentLevelStage = 0;
            getLcharacteristics(m_gameData.m_currentLevel, m_gameData.m_currentLevelStage, m_gameData.m_gameMode, &m_currentLevelCharacteristics);
           
        }
    } else {
        uint8_t scrollspeed = m_currentLevelCharacteristics.scrollSpeed;
        if (m_gameData.m_speedUp) scrollspeed = scrollspeed / 2;
        if (m_gameData.m_scrollMinPosition >= scrollspeed){
            m_gameData.m_scrollMinPosition = 0;
            m_gameData.m_scrollPosition++;


            m_gameData.m_drawingObject++;
            if (m_gameData.m_currentLevelStage < 10 && m_gameData.m_drawingObject < m_currentLevelCharacteristics.objectMaxHeight && m_gameData.m_drawingObjectCount < m_currentLevelCharacteristics.objectMaxCount) {
                createLine();
            } else {
                
                
                if (m_gameData.m_drawingObjectCount >= m_currentLevelCharacteristics.objectMaxCount){ 
                    m_gameData.m_drawingObjectCount = 0; m_gameData.m_drawingObject = m_currentLevelCharacteristics.objectMaxHeight;
                }

                if (m_gameData.m_drawingObject > m_currentLevelCharacteristics.objectMaxHeight + m_currentLevelCharacteristics.minObjectSpaceBetween){
                    m_gameData.m_drawingObject = 0;
                    m_gameData.m_currentLevelMircoStage++;
                    if (m_gameData.m_currentLevelMircoStage >= m_currentLevelCharacteristics.objectCountPerStage){
                        m_gameData.m_currentLevelStage++;
                        m_gameData.m_currentLevelMircoStage = 0;
                        m_gameData.m_drawingObjectCount = 0;
                        getLcharacteristics(m_gameData.m_currentLevel, m_gameData.m_currentLevelStage, m_gameData.m_gameMode, &m_currentLevelCharacteristics);
                    }
                    
                }
            }

            scrollDown();
        }

        m_starField->draw(m_gameData.m_speedUp);
        drawBorder();
        drawShots();
        drawBricks();
        
        drawCrosshair();
        
     
    
    
    } 
   
    pocketDisplay.setCursor(4, 28);
    pocketDisplay.print(F("POINTS"));
    
    pocketDisplay.setCursor(16 - (2*Utils::countdigits(m_gameData.m_points)), 41);
    pocketDisplay.print(m_gameData.m_points);
    
    pocketDisplay.setCursor(5, 2);
    pocketDisplay.print(F("LEVEL"));
   
    if (m_gameData.m_currentLevelStage > 0 && m_gameData.m_currentLevelStage < 10){
        pocketDisplay.setCursor(11, 15);
        pocketDisplay.print(m_gameData.m_currentLevel);
          
        pocketDisplay.print('/');
        pocketDisplay.print(m_gameData.m_currentLevelStage);
    } else {
        pocketDisplay.setCursor(14, 15);
        pocketDisplay.print(m_gameData.m_currentLevel);
    }
    
    bool shipShooting = false;
    for(uint8_t i = 0; i < 8; i++) {
        if (bitRead(m_gameData.m_shotActive, i) && m_gameData.m_shotPositionY[i] > 40){
            shipShooting = true;
        }
    }
    m_spaceShip->shooting(shipShooting);
    m_spaceShip->draw(m_gameData.m_shipXPosition);
    
    if (m_gameLoaded) { m_gamePaused = true; m_gameLoaded = false; }
    
}

void GameController::scrollDown() {
    int16_t until = (GAMEFIELD_BRICKS_WIDTH*GAMEFIELD_BRICKS_HEIGHT) - GAMEFIELD_BRICKS_WIDTH;
    for (int16_t i = until - 1; i >= 0; i--) {
        m_gameData.gameFieldBricks[i+GAMEFIELD_BRICKS_WIDTH] = m_gameData.gameFieldBricks[i];
    }
    for (uint8_t i = 0; i < GAMEFIELD_BRICKS_WIDTH; i++) m_gameData.gameFieldBricks[i] = 0;
    
    moveShots(true);
}

void GameController::drawShots() {
    for(uint8_t i = 0; i < 8; i++) {
        if (bitRead(m_gameData.m_shotActive, i)){
            drawShot(i);
        }
    }
}
void GameController::drawShot(uint8_t shotNumber) {
    pocketDisplay.drawBitmap(m_gameData.m_shotPositionX[shotNumber],m_gameData.m_shotPositionY[shotNumber],brickDissApp, 3, 3);
    
}
void GameController::drawBricks() {
    int16_t bmpx;
    int16_t bmpy;
    bool blockDis = false;
    bool bricksOnField = false;
    for (uint8_t y = 0; y < GAMEFIELD_BRICKS_HEIGHT; y++){
        for (uint8_t x = 0; x < GAMEFIELD_BRICKS_WIDTH; x++){
            uint8_t b = getBrick(x,y);
            if (b){
                bricksOnField = true;
                bmpx = (x*3)+GAMEFIELD_X + 5;
                bmpy = (y*3) - 15; // hide the first 5 bricks
                m_gameData.m_highestBrick = y;
            }
            if (b && b <127){
                // draw nomal brick
               
                
                pocketDisplay.drawBitmap(bmpx,bmpy,brick, 3, 3);
                if (x>0 && getBrick(x-1,y) == b) {
                    pocketDisplay.drawPixel(bmpx, bmpy+1,0xf0);
                   
                } 
                               
                if (x<GAMEFIELD_BRICKS_WIDTH -1 && getBrick(x+1,y) == b) {
                   pocketDisplay.drawPixel(bmpx + 2, bmpy+1,0xf0);
                  
                }
               
                if (y>0 && getBrick(x,y-1) == b) {
                    // above is this object, too
                    pocketDisplay.drawPixel(bmpx, bmpy,0xf0);
                    pocketDisplay.drawPixel(bmpx+1, bmpy,0xf0);
                    pocketDisplay.drawPixel(bmpx+2, bmpy,0xf0);
                   
                }
                if (y<GAMEFIELD_BRICKS_HEIGHT - 1 && getBrick(x,y+1) == b) {
                    // under is this object, too
                    pocketDisplay.drawPixel(bmpx, bmpy + 2,0xf0);
                    pocketDisplay.drawPixel(bmpx+1, bmpy + 2,0xf0);
                    pocketDisplay.drawPixel(bmpx+2, bmpy + 2,0xf0);
                    
                }
                
                if (x>0 && getBrick(x-1,y) != b) {
                    pocketDisplay.drawPixel(bmpx, bmpy,0xfa);
                    pocketDisplay.drawPixel(bmpx, bmpy+1,0xfa);
                    pocketDisplay.drawPixel(bmpx, bmpy+2,0xfa);
                }
                
              
                
                
                
                
            }
            if (b && b == 127){
                // spezial brick!
                pocketDisplay.drawBitmap(bmpx,bmpy,brickSpezial, 3, 3);
            }
            
                                
           
            
            if (b > 127) {
                // these brick is disappearing 
                
                pocketDisplay.drawBitmap(bmpx ,bmpy,brickDissApp, 3, 3);
               
                
                if (m_gameData.m_destroyBrickWaitingCounter == 0) if (b <= 254) setBrick(x,y,b+1);
                blockDis = true;
                
            }
           
            if (b > 254) {
               setBrick(x,y,0);
                //m_tones->tone(400, 25, 800, 25);
                m_gameData.m_points += m_gameData.m_lastFoundObjectsCount;
              
                
            }
        }
    }
    if (m_gameData.m_destroyBrickWaitingCounter > 0){
        m_gameData.m_destroyBrickWaitingCounter--;
    } 
    m_gameData.m_bricksOnField = bricksOnField;
    if (m_gameData.m_bricksAreDisappearing && ! blockDis) { m_gameData.m_bricksAreDisappearing = false; m_gameData.m_destroyBrickWaitingCounter; }
   
}

bool GameController::insertObject(uint8_t x, uint8_t y, uint8_t opjectType, bool spezial)
{
    if (m_gameData.m_sezialDrawn) spezial = false;
    uint8_t objNumber = m_gameData.m_newObjectNumber;
    if (spezial) objNumber = 127;
    
    bool overLapped = false;
    // check for overlapped / out of the box
    for (int yb = 0; yb < 5; yb++){
    uint8_t byteb = pgm_read_byte_near(bricks + yb +  ((opjectType - 1) * 5));
        for (int xb = 0; xb < 8; xb++){
            if (bitRead(byteb, 7 - xb)){
                if (xb + x > 15)  { overLapped = true; break; }
                if (getBrick(xb + x, yb + y)) { overLapped = true; break; }
            }
        }
        if (overLapped) break;
    }
       
   
    if (overLapped) return false;
    for (int yb = 0; yb < 5; yb++){
        uint8_t byteb = pgm_read_byte_near(bricks + yb +  ((opjectType - 1) * 5));
        for (int xb = 0; xb < 8; xb++){
            if (bitRead(byteb, 7 - xb)) setBrick(xb + x, yb + y, objNumber);
        }
    }
    
    m_gameData.m_newObjectNumber++;
    if (m_gameData.m_newObjectNumber == 127) m_gameData.m_newObjectNumber = 1;
    if (spezial) m_gameData.m_sezialDrawn = true;
    return true;
    
   
    
}
bool GameController::isBrickDestroying(uint8_t x, uint8_t y)
{
    if (getBrick(x,y) > 170) return true;
    return false;
}
uint8_t GameController::getBrick(uint8_t x, uint8_t y)
{
   
    return m_gameData.gameFieldBricks[x+(y*GAMEFIELD_BRICKS_WIDTH)];
}
void GameController::setBrickDestroy(uint8_t x, uint8_t y, uint8_t val)
{
    uint8_t v = 254-val;
    if (v < 171) v = 171;
    setBrick(x,y,v);
}
void GameController::setBrick(uint8_t x, uint8_t y, uint8_t val)
{
    m_gameData.gameFieldBricks[x+(y*GAMEFIELD_BRICKS_WIDTH)] = val;
   
}

bool GameController::isValueInArray(uint8_t* array, uint8_t count, uint8_t searchValue) {
    for (int i = 0; i < count; i++){
        if (array[i] == searchValue) return true;
    }
    return false;
}


// this could definetly done better, i think
uint16_t GameController::checkOneQuarth(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t* foundSpezial)
{
    /// check the frame
    uint8_t foundObjects[12];
    uint8_t foundObjectsCount = 0;
    *foundSpezial = 0;
  
    
    
    for (uint8_t cx = x1; cx <= x2; cx++){
        for(uint8_t cy = y1; cy <= y2; cy++){
            uint8_t fb = getBrick(cx, cy);
            if (fb > 170) fb = 0;
            if (fb == 127) *foundSpezial = 1;
            if (! fb && (cx == x1 || cx == x2 || cy == y1 || cy == y2)) return 0;
            if (foundObjectsCount < 12){
                if (fb && ! isValueInArray(foundObjects, foundObjectsCount, fb)){
                    foundObjects[foundObjectsCount++] = fb;
                   
                } 
            } else return 0;// sorry too many objects here
            
        }
    }
    
    
    ///////// okay, now let's see if an object is not fully in this quarth
    if (x1 > 0){
        // left from the quarder
        for (int i = y1; i <= y2; i++) {
            uint8_t fb = getBrick(x1-1, i);
            if (fb && isValueInArray(foundObjects, foundObjectsCount, fb)){
                return 0;
            }                    
        }
    }
    if (x2 < GAMEFIELD_BRICKS_WIDTH - 1){
        // right from the quarder
        for (int i = y1; i <= y2; i++) {
            uint8_t fb = getBrick(x2+1, i);
            if (fb && isValueInArray(foundObjects, foundObjectsCount, fb)){
                return 0;
            }                    
        }
    }
    
    if (y2 < GAMEFIELD_BRICKS_HEIGHT - 1){
        // under it
       for (int i = x1; i <= x2; i++) {
            uint8_t fb = getBrick(i, y2+1);
            if (fb && isValueInArray(foundObjects, foundObjectsCount, fb)){
                return 0;
            }                    
        } 
    }
    if (y1 >0){
        // above it
       for (int i = x1; i <= x2; i++) {
            uint8_t fb = getBrick(i, y1-1);
            if (fb && isValueInArray(foundObjects, foundObjectsCount, fb)){
                return 0;
            }                    
        } 
    }
   m_gameData. m_lastFoundObjectsCount = foundObjectsCount;
    return (((x2-x1) + 1)*((y2-y1) + 1)) * foundObjectsCount;
}



// this could definetly done better, i think
void GameController::checkForQuarth(uint8_t x, uint8_t y)
{
    uint16_t points = 0;
    uint8_t foundX1;
    uint8_t foundY1;
    uint8_t foundX2;
    uint8_t foundY2;
    uint8_t r = GAMEFIELD_BRICKS_WIDTH - x;
    uint8_t highestPoints = 0;
    uint8_t highestFoundObjectsCount = 0;
    uint8_t foundSpezial = 0;
    for (uint8_t rr = 0; rr < r; rr++) {
        for (uint8_t sx = 0; sx < x + rr; sx++) {
            for (uint8_t sy = 0; sy < y; sy++) {
                /// only, if x is in this
                if (x >= sx && x <= x+rr) {
                    uint8_t fs;
                    points = checkOneQuarth(sx,sy,x+rr,y, &fs);
                    if (points > highestPoints){
                        highestPoints = points;
                        highestFoundObjectsCount = m_gameData.m_lastFoundObjectsCount;
                        foundX1 = sx;
                        foundX2 = x+rr;
                        foundY1 = sy;
                        foundY2 = y;
                        foundSpezial = fs;
                    }
                }
            }
           
        }
        
    }
    
    int8_t counter = 0;
    points = highestPoints;
    m_gameData.m_lastFoundObjectsCount = highestFoundObjectsCount;
    if (points > 0 && foundSpezial) {
        m_gameData.m_bricksAreDisappearing = true;
        m_gameData.m_destroyBrickWaitingCounter = 50;
        for (int y = 0; y < GAMEFIELD_BRICKS_HEIGHT; y++) {
            counter++;
            for (int x = 0; x < GAMEFIELD_BRICKS_WIDTH; x++) {
                if (getBrick(x,y)) setBrickDestroy(x, y, counter);
            }
        }
    }
    else if (points > 0) {
        uint16_t disPoints = points/m_gameData.m_lastFoundObjectsCount;
        
        m_gameData.m_bricksAreDisappearing = true;
        if (m_gameData.m_gameMode == GAME_MODE_HARD)  { 
            m_gameData.m_destroyBrickWaitingCounter = 20; 
        } else {
            if (disPoints*2 < 250)
                m_gameData.m_destroyBrickWaitingCounter = (disPoints*2) + 25;
            else
                m_gameData.m_destroyBrickWaitingCounter = 250;
        }
        
        
        
        uint8_t currX1 = foundX1;
        uint8_t currX2 = foundX2;
        uint8_t currY1 = foundY1;
        uint8_t currY2 = foundY2;
        
        
        while(currX2 - currX1 > 1 || currY2 - currY1 > 1) {
            for (uint8_t ix = currX1; ix <= currX2; ix++) {
                if (!isBrickDestroying(ix, currY1)) {
                    counter++;
                    setBrickDestroy(ix, currY1, counter);
                }
            }
            
            for (uint8_t iy = currY1; iy <= currY2; iy++) {
                if (!isBrickDestroying(currX2, iy)) {
                    counter++;
                    setBrickDestroy(currX2, iy, counter);
                }
            }
            
            for (uint8_t ix = currX2; ix > currX1; ix--) {
                if (!isBrickDestroying(ix, currY2)) {
                    counter++;
                    setBrickDestroy(ix, currY2, counter);
                }
            }
            
            for (uint8_t iy = currY2; iy > currY1; iy--) {
                if (!isBrickDestroying(currX1,iy)) {
                    counter++;
                    setBrickDestroy(currX1, iy, counter);
                }
            }
           
            if (currX2 >= currX1 + 1) {
                currX1++;
                if (currX2 > currX1 + 1) currX2--;
            }
            
            if (currY2 >= currY1 + 1) {
                currY1++;
                if (currX2 > currX1 + 1) currY2--;
                
            }
            
            
        }
        
        {
            //if this is an exact quarth, mit middle could be missing
            for (uint8_t iy = foundY1; iy <= foundY2; iy++) {
                for (uint8_t ix = foundX1; ix <= foundX2; ix++) {
                    counter++;
                    if (!isBrickDestroying(ix,iy)) setBrickDestroy(ix, iy, counter);
                    
                }
            }
           
        }
        
 
      
    }
}




