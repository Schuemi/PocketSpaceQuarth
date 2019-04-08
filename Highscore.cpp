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
 * File:   Highscore.cpp
 * Author: Schuemi
 * 
 * Created on 12. Dezember 2017, 21:05
 */

#include "constants.h"
#include "Highscore.h"

#include "Utils.h"
#include "PocketDisplay.h"
#include "PocketStar.h"
#include "GameControl.h"
#include "FakeEEPROM.h"

Highscore::Highscore() {
    // cannot load highscore here, because the SD is not ready, yet
    m_highscoreLoaded = false;
    m_starField = new StarField(0, 0, 96, 64, 4);
    reset();
    
    
}

void Highscore::saveListEEPROM() {
   loadListEEPROM();
    uint32_t eepromMagic = EEPROM_MAGIC;
   FakeEEPROM EEPROM;
   EEPROM.put(EEPROM_MAGIC_BYTE + EEPROM_STORAGE_SPACE_START, eepromMagic);
   EEPROM.put(EEPROM_HIGHSCORE_BYTE + EEPROM_STORAGE_SPACE_START, m_highscoreList);
}
void Highscore::loadListEEPROM() {
    if (! m_highscoreLoaded) {
        
        FakeEEPROM EEPROM;
        uint32_t eepromMagic;
        EEPROM.get(EEPROM_MAGIC_BYTE + EEPROM_STORAGE_SPACE_START, eepromMagic);
        if (eepromMagic == EEPROM_MAGIC) {
            EEPROM.get(EEPROM_HIGHSCORE_BYTE + EEPROM_STORAGE_SPACE_START, m_highscoreList);
        } else {
            pocketstar.setBacklightColor(PIN_LED_RED);
            for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
                m_highscoreList[i].name[0] = 0;
                m_highscoreList[i].score = 0;

            }
            m_highscoreLoaded = true;
            saveListEEPROM();
        }
        m_highscoreLoaded = true;
    }
}
Highscore::~Highscore() {
}

void Highscore::reset() {
   
    m_state = STATE_NORMAL;
    m_cusorBlink = 0;
    m_newNamePlace = HIGHSCORE_LIST_SIZE;
    m_currEditLocation = 0;
    m_currName[0] = '_';
    m_currName[1] = '_';
    m_currName[2] = '_';
    m_closeHighScore = false;
    
}


void Highscore::addNewRecord(uint32_t score) {
   
    m_newNamePlace = HIGHSCORE_LIST_SIZE;
    for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
        if (m_highscoreList[i].score < score){
            m_newNamePlace = i;
            break;
        }
    }
    if (m_newNamePlace < HIGHSCORE_LIST_SIZE){
        for (int i = HIGHSCORE_LIST_SIZE-2; i >= m_newNamePlace; i--) {
            m_highscoreList[i+1].score =  m_highscoreList[i].score;
            memcpy(m_highscoreList[i+1].name, m_highscoreList[i].name, 3);
        }
        m_state = STATE_INSERT_NAME;
        m_highscoreList[m_newNamePlace].score = score;
    }
    
}


void Highscore::draw() {
    
    
    loadListEEPROM();
    
    m_starField->move();
    m_starField->draw(false);
    
    pocketDisplay.setCursor(30, 0);
    pocketDisplay.print(F("HIGHSCORE"));
    pocketDisplay.setFont(pocketStar6pt, 5);
    
    
    
    if (m_state == STATE_INSERT_NAME) {
        gameControl.pollButtons();
        char nameStr[10];
        memcpy(nameStr, F(". ---    "), 10);
        memcpy(&nameStr[2], m_currName, 3);
       
        
        
        pocketDisplay.setCursor(2, 16);
        m_cusorBlink++; if (m_cusorBlink == 20) { m_cusorBlink = 0; }
        if (m_newNamePlace < 9) pocketDisplay.setCursor(7, 16);
        pocketDisplay.print(m_newNamePlace+1);
        
        if (m_cusorBlink < 10){
            pocketDisplay.setCursor(26 + 6*m_currEditLocation, 18);
            pocketDisplay.print('_');
            
        } 
        
        pocketDisplay.setCursor(14, 16);
        pocketDisplay.print(nameStr);    
        pocketDisplay.print(m_highscoreList[m_newNamePlace].score);        
        
        
       
        
        if (gameControl.justPressed(ButtonUp)){
            m_currName[m_currEditLocation]++;
            if (m_currName[m_currEditLocation] < 0x41 || m_currName[m_currEditLocation] > 0x5A) m_currName[m_currEditLocation] = 0x41;
            
        }
        if (gameControl.justPressed(ButtonDown)){
            m_currName[m_currEditLocation]--;
            if (m_currName[m_currEditLocation] < 0x41 || m_currName[m_currEditLocation] > 0x5A) m_currName[m_currEditLocation] = 0x5A;
            
        }
        if (gameControl.justPressed(ButtonB) || gameControl.justPressed(ButtonRight)){
            m_currEditLocation++;
            if (m_currEditLocation == 3) m_currEditLocation = 0;
            
        }
        if (gameControl.justPressed(ButtonLeft)){
            if (m_currEditLocation == 0) m_currEditLocation = 2; else m_currEditLocation--;
        }
        if (gameControl.justPressed(ButtonA)){
            if (m_currName[0] != '_' || m_currName[1] != '_' || m_currName[2] != '_') {
                memcpy(m_highscoreList[m_newNamePlace].name, m_currName, 3);
                for(int i = 0; i < 3; i++) if (m_highscoreList[m_newNamePlace].name[i] == '_') m_highscoreList[m_newNamePlace].name[i] = 0x20;
                saveListEEPROM();
                m_state = STATE_NORMAL;
                
                
            }
        }
        
        
    }
    
    if (m_state == STATE_NORMAL) {
        gameControl.pollButtons();
        
        for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
                int16_t cusory = (i*9 + 12);
                if (cusory > 128) break;
                if (cusory < -6 ) continue;
                
                pocketDisplay.setCursor(2, cusory);
                if (i < 9) pocketDisplay.setCursor(7, cusory);
                pocketDisplay.print(i+1);
                pocketDisplay.setCursor(14, cusory);
                if (m_highscoreList[i].score == 0){
                    pocketDisplay.print(F(".--- ---------"));
                } else {
                    char name[4]; 
                    memcpy(name, m_highscoreList[i].name, 3);
                    name[3] = 0;
                    pocketDisplay.print(".");
                    pocketDisplay.print(name);
                    pocketDisplay.setCursor(37 + ((10 - Utils::countdigits(m_highscoreList[i].score)) * 6), cusory);
                    
                    pocketDisplay.print(m_highscoreList[i].score);
                }
                        
        }
        
        if (gameControl.justPressed(ButtonA) || gameControl.justPressed(ButtonB)){
            m_closeHighScore = true;
        }
        
       
    }
    pocketDisplay.setFont(pocketStar6pt);
   
}
