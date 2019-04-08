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
 * File:   StarField.cpp
 * Author: Schuemi
 * 
 * Created on 12. Dezember 2017, 11:28
 */
#include <PocketStar.h>

#include "StarField.h"
#include "PocketDisplay.h"


StarField::StarField(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t direction) {
    
    m_x = x;
    m_width = width;
    m_y = y;
    m_height = height;
    m_direction = direction;
    for (int i = 0; i < MAX_STARS; i++) {
        uint8_t speed = random(3)+1;
        m_stars[i].starsX = m_x + random(m_width);
        m_stars[i].starsY = m_y + random(m_height);
        m_stars[i].speed = speed;
        m_stars[i].color = WHITE_8b;
        if (speed < 3) m_stars[i].color = GRAY_8b;
        if (speed < 2) m_stars[i].color = DARKGRAY_8b;
    }
    
}

void StarField::move() {
   for (int i = 0; i < MAX_STARS; i++) {
       bool newStar = false;
       uint8_t y = m_stars[i].starsY;
       uint8_t speed = m_stars[i].speed;
       if (m_direction == 1) {
           y += speed;
            if (y > m_y + m_height){
                m_stars[i].starsX = m_x + random(m_width);
                y = m_y;
                speed = random(3)+1;
                newStar = true;
            }
       }
      
       if (m_direction == 4) {
            if (m_stars[i].starsX < 2){
                y = m_y + random(m_height);
                m_stars[i].starsX = m_x + m_width;
                speed = random(3)+1;
                newStar = true;
            } else m_stars[i].starsX -=speed;
       }
       if (newStar){
            m_stars[i].color = WHITE_8b;
            if (speed < 3) m_stars[i].color = GRAY_8b;
            if (speed < 2) { if(random(10) == 1) m_stars[i].color = DARKBLUE_8b; else if(random(10) == 1) m_stars[i].color = DARKRED_8b; else m_stars[i].color = DARKGRAY_8b; }
       }
       
       m_stars[i].starsY = y;
       m_stars[i].speed = speed;
   } 
}
void StarField::draw(bool warp) {
    for (int i = 0; i < MAX_STARS; i++) {
        uint8_t y = m_stars[i].starsY;
        
         pocketDisplay.drawPixel(m_stars[i].starsX, y,m_stars[i].color);
         if (warp || m_stars[i].speed > 2){
            if (m_direction == 1) pocketDisplay.drawPixel(m_stars[i].starsX, y + 1,m_stars[i].color);
            if (m_direction == 4) pocketDisplay.drawPixel(m_stars[i].starsX + 1, y,m_stars[i].color);
            
         }
    }
}
StarField::~StarField() {
}

