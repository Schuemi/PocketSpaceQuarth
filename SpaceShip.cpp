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
 * File:   SpaceShip.cpp
 * Author: Schuemi
 * 
 * Created on 10. Dezember 2017, 17:47
 */
#include  <stdlib.h>

#include "SpaceShip.h"
#include "constants.h"
#include <avr/pgmspace.h>
#include "PocketDisplay.h"
#include <PocketStar.h>

// spaceShip2.png  width: 12 height: 11

const uint8_t PROGMEM SpaceShip::m_sprite[] = {
0x00,0x00,0x00,0x00,0x00,0x72,0x2d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4d,0x76,0x2e,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9a,0x52,0x2e,0x0a,0x04,0x00,0x00,0x00,0x00,0x00,0x29,0x2a,0x9a,0x52,0x4e,0x0e,0x09,0x05,0x00,0x00,0x00,0x29,0x52,0x76,0x7a,0x32,0x4e,0x2e,0x0a,0x0a,0x05,0x00,0x29,0x7a,0x56,0x52,0x76,0x76,0x2e,0x2e,0x09,0x2e,0x2e,0x09,0x4d,0x29,0x2e,0x72,0x72,0x8d,0x84,0x24,0x05,0x09,0x05,0x09,0x00,0x29,0x56,0x9a,0x96,0xb6,0xa8,0x24,0x05,0x09,0x05,0x00,0x00,0x52,0x52,0x7a,0x76,0x69,0x44,0x05,0x09,0x09,0x09,0x00,0x00,0x00,0x4e,0x9a,0x56,0x52,0x29,0x2e,0x0e,0x09,0x00,0x00,0x00,0x00,0x29,0x49,0x49,0x29,0x25,0x05,0x04,0x04,0x00,0x00
};

// spaceShip2open.png  width: 12 height: 3
const uint8_t PROGMEM SpaceShip::m_spriteOpen[] = {
0x00,0x00,0x00,0x72,0x00,0x00,0x00,0x00,0x2d,0x00,0x00,0x00,0x00,0x00,0x4d,0x76,0x00,0x00,0x00,0x00,0x2e,0x09,0x00,0x00,0x00,0x00,0x9a,0x52,0x00,0x00,0x00,0x00,0x2e,0x0a,0x04,0x00,
};



SpaceShip::SpaceShip(uint8_t x) {
  
    reset();
    
}


SpaceShip::~SpaceShip() {
}

void SpaceShip::doExplode() {
   
    if (! m_exploding) m_exploding = 1;
}

void SpaceShip::reset() {
    m_exploding = 0;
    m_ypos = 0;
    m_redLed = 0;
    m_shooting = false;
    
}
 void SpaceShip::shooting(bool s) {
    m_shooting = s; 
 }
void SpaceShip::setyPos(int8_t y) {
    m_ypos = y;
}
void SpaceShip::draw(uint8_t x) {
    int16_t xd = GAMEFIELD_X + (x*3);
    int16_t yd = 53 + m_ypos;
    
    if (m_exploding) {
        
        yd = 53;
        if (m_exploding < 25){ m_exploding++; m_redLed+=3; pocketDisplay.drawBitmap(xd,yd,m_sprite, 12, 11); }
        if (m_exploding == 25 && m_redLed > 0) m_redLed--;
        
        for (int i = 0; i < m_redLed / 10; i++){
            uint8_t color = random(8) << 5;
            pocketDisplay.drawCircle(xd + 6, yd + 5, i, color);
        }
        
        
        
        for (int xx = xd; xx < xd + 12; xx++){
            for (int yy = yd; yy < yd + 11; yy++){
                if (!random(3))
                    pocketDisplay.drawPixel(xx,yy, 0);
                
            }
        }
        
        return;
    }
    
    pocketDisplay.drawBitmap(xd,yd,m_sprite, 12, 11);
    if (m_shooting)pocketDisplay.drawBitmap(xd,yd,m_spriteOpen, 12, 3);
}

