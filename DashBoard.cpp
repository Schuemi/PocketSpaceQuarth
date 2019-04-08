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
 * File:   DashBoard.cpp
 * Author: Schuemi
 * 
 * Created on 12. Dezember 2017, 11:28
 */

#include "DashBoard.h"
#include "StarField.h"
#include "math.h"
#include <avr/pgmspace.h>
#include <PocketStar.h>
#include "PocketDisplay.h"
#include "GameControl.h"
#include "constants.h"
#include "FakeEEPROM.h"

#define MENU_X 18
#define MENU_Y 38
#define MENU_X_TXT MENU_X + 5


enum MENU_ITEMS {
   // MENU_ITEM_SOUND = 0,
    MENU_VIBRATION = 0,
    MENU_ITEM_MODE,
    MENU_ITEM_LEVEL
    
};




const uint8_t PROGMEM DashBoard::title[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x68,0x00,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0x00,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0x68,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0xac,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x8c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x88,0xac,0xac,0xac,0xac,0xac,0x00,0x00,0x00,0x88,0xac,0xac,0xac,0xac,0x00,0x00,0xac,0xac,0xac,0xac,0x68,0x00,0x00,0x68,0xac,0xac,0xac,0xac,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x44,0xac,0x88,0x00,0x00,0xac,0xac,0x00,0x00,0x44,0xac,0xac,0xac,0xac,0x00,0x44,0xac,0xac,0x88,0xac,0x88,0x00,0x88,0xd0,0xd0,0x20,0x00,0x00,0xd0,0xd0,0xcc,0xac,0xac,0xac,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x20,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0x68,0x44,0xd0,0xd0,0xac,0xd0,0xd0,0x00,0xcc,0xd0,0xd0,0x24,0x44,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x20,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x24,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xd0,0xac,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0x20,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x88,0x88,0x88,0x00,0x00,0x00,0x00,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0x8c,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x88,0x88,0x00,0x00,0x00,0xac,0x88,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xd0,0xac,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0x20,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x88,0xd0,0xd0,0x44,0x00,0x00,0x00,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xd0,0xac,0xd0,0xd0,0x8c,0xd0,0xd0,0xd0,0x20,0xd0,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x44,0xd0,0xd0,0x88,0x00,0x00,0x00,0x44,0x44,0x44,0x44,0x64,0xd0,0xd0,0x00,0x44,0x44,0x44,0x44,0x44,0x44,0x00,0x20,0x44,0x44,0x44,0x44,0x44,0x44,0x00,0xd0,0xd0,0x44,0x44,0x44,0x44,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x64,0xd0,0xd0,0x20,0x44,0x44,0x44,0x44,0xd0,0xd0,0x00,0x44,0x20,0x44,0x44,0x44,0x44,0x88,0xd0,0xac,0x00,0x00,0xd0,0xd0,0xd0,0x00,0x00,0x00,0x44,0x64,0x20,0x00,0x00,0xd0,0xd0,0x00,0x00,0x64,0x44,0x00,0x44,0x64,0x20,0x24,0x64,0x20,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x20,0x64,0x64,0x64,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0x00,0x68,0x00,0x00,0x20,0x64,0x44,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0x44,0x64,0x20,0x00,0x44,0xd0,0xac,0x00,0x00,0x88,0xd0,0xd0,0x68,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0xd0,0xd0,0x00,0x88,0xd0,0xcc,0x00,0xac,0xd0,0x44,0x88,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xac,0x00,0x64,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0xd0,0xd0,0xd0,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x00,0xac,0xd0,0x44,0x88,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x00,0x64,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0xac,0xd0,0xd0,0x64,0x00,0xcc,0xd0,0x64,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x44,0xac,0xd0,0x44,0x88,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x88,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x00,0x64,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0x00,0xd0,0xd0,0xcc,0x00,0xcc,0xd0,0x64,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0x44,0xac,0xd0,0x44,0x88,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x64,0xd0,0xd0,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0x00,0xac,0xd0,0xd0,0x00,0xcc,0xd0,0x64,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xac,0x68,0xac,0xd0,0x44,0x88,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x20,0xac,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xcc,0x00,0x00,0x00,0xd0,0xd0,0x00,0x68,0xd0,0xcc,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0xac,0x88,0x44,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0x00,0x68,0xd0,0xd0,0x44,0xcc,0xd0,0x68,0x00,0x20,0xd0,0xd0,0x00,0xd0,0xd0,0x00,0x00,0xac,0xd0,0x44,0x88,0xd0,0x68,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0x00,0x00,0x20,0xd0,0xd0,0x00,0x68,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0x00,0xd0,0xd0,0x20,0x00,0x68,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x68,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x20,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x68,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x88,0xd0,0xd0,0xd0,0xd0,0xd0,0x20,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x68,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x44,0x44,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x68,0xd0,0xd0,0xd0,0xd0,0xd0,0x20,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0x88,0x00,0x64,0xd0,0xd0,0xd0,0xd0,0xd0,0x64,0x00,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x00,0xac,0xd0,0xd0,0xd0,0xd0,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x00,0x44,0xd0,0xd0,0xd0,0xd0,0xd0,0xd0,0x00,0x8c,0xd0,0xd0,0xd0,0xd0,0xd0,0xac,0x44,0xd0,0xcc,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0xd0,0xd0,0x44,0x00,0x44,0xd0,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xac,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0xd0,0xac,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xd0,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};





DashBoard::DashBoard() {
   m_starField = new StarField(0, 0, 96, 64, 4);
  
   m_startLevel = 1;
   
}

void DashBoard::reset() {
    pocketstar.setBacklightColor(BacklightBlack);
    
    m_fontPosition = 128;
    m_bounce = 1;
    m_startGame = false;
    m_startHighScore = false;
    m_currSelected = 0;
    m_itemsCount = 2;
    FakeEEPROM EEPROM;
    EEPROM.get(EEPROM_GAME_MODE_BYTE + EEPROM_STORAGE_SPACE_START, gameControl.gameMode);
    if (gameControl.gameMode > GAME_MODE_HARD) gameControl.gameMode = GAME_MODE_EASY;
           
    //if (EEPROM.read(EEPROM_GAME_CLEARED_BYTE + EEPROM_STORAGE_SPACE_START) == 0x32) m_itemsCount = 4;
    
   
}

DashBoard::~DashBoard() {
}


void DashBoard::draw() {
    uint8_t itemsy = MENU_Y;
    m_starField->move();
    m_starField->draw(false);
    
    if (m_itemsCount > 3) itemsy = itemsy - (m_currSelected * 3);
    
    gameControl.pollButtons();
    
    //g_arduboy->clear();
    
    if (m_fontPosition > 35) 
        m_fontPosition-= 4;
    else if (m_bounce > -0.7) {
        m_bounce-= 0.03;
        m_fontPosition-= sin(m_bounce)*4;
    } 
    
   
    
    
    
    
    pocketDisplay.drawPixel(MENU_X + 1, itemsy + 3 + (m_currSelected * 8), WHITE_8b);
    pocketDisplay.drawPixel(MENU_X, itemsy + 2 + (m_currSelected * 8), WHITE_8b);
    pocketDisplay.drawPixel(MENU_X, itemsy + 3 + (m_currSelected * 8), WHITE_8b);
    pocketDisplay.drawPixel(MENU_X, itemsy + 4 + (m_currSelected * 8), WHITE_8b);
    pocketDisplay.setCursor(MENU_X_TXT, itemsy);
    /*if (gameControl.audio)
        pocketDisplay.print(F("SOUND  ON"));
    else
        pocketDisplay.print(F("SOUND  OFF"));
    
    itemsy += 8;pocketDisplay.setCursor(MENU_X_TXT, itemsy);*/
    
    //isSave = EEPROM.read(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START);
    if (pocketstar.vibrationEnabled) 
       pocketDisplay.print(F("VIBR.  ON"));
    else
       pocketDisplay.print(F("VIBR.  OFF"));
    
    
    itemsy += 8;pocketDisplay.setCursor(MENU_X_TXT, itemsy);
    
    
    
    pocketDisplay.print(F("MODE  "));
    
    switch(gameControl.gameMode) {
        case GAME_MODE_EASY: pocketDisplay.print(F("EASY")); break;
        case GAME_MODE_NORMAL: pocketDisplay.print(F("NORMAL")); break;
        case GAME_MODE_HARD: pocketDisplay.print(F("HARD")); break;
        
    }
   
    
    if (m_itemsCount == 4) {
        itemsy += 8;pocketDisplay.setCursor(MENU_X_TXT, itemsy);
        pocketDisplay.print(F("LEVEL  ")); pocketDisplay.print(m_startLevel);
    }
    

    
   
        
    if (gameControl.justPressed(ButtonB)){
       m_startHighScore = true;
       randomSeed(micros());
    }
    if (gameControl.justPressed(ButtonA)){
       m_startGame = true;
       randomSeed(micros());
    }
    if (gameControl.justPressed(ButtonDown)){
        m_currSelected++;
        if (m_currSelected == m_itemsCount) m_currSelected = 0;
        
    }
    
    
    
    if (gameControl.justPressed(ButtonUp)){
        if (m_currSelected == 0) m_currSelected = m_itemsCount - 1; else m_currSelected--;
        

    }
    if (gameControl.justPressed(ButtonLeft) || gameControl.justPressed(ButtonRight)){
/*        if (m_currSelected == MENU_ITEM_SOUND){
            gameControl.audioToggle();
            
        }*/
        if (m_currSelected == MENU_VIBRATION){
            gameControl.vibrationToggle();
           //if (isSave) EEPROM.update(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START, 0); else  EEPROM.update(EEPROM_AUTOSAVE_BYTE + EEPROM_STORAGE_SPACE_START, 1);
        }
    }
    if (gameControl.justPressed(ButtonLeft)){
        
        if (m_currSelected == MENU_ITEM_LEVEL){
            m_startLevel--;
            if (m_startLevel == 0) m_startLevel = 8;

        }
        if (m_currSelected == MENU_ITEM_MODE){
            if (gameControl.gameMode == GAME_MODE_EASY) gameControl.gameMode = GAME_MODE_HARD; else gameControl.gameMode--;
            FakeEEPROM EEPROM;
            EEPROM.put(EEPROM_GAME_MODE_BYTE + EEPROM_STORAGE_SPACE_START, gameControl.gameMode);
        }
        
    }
    if (gameControl.justPressed(ButtonRight)){
       
        if (m_currSelected == MENU_ITEM_LEVEL){
            m_startLevel++;
            if (m_startLevel == 9) m_startLevel = 1;
        }
        if (m_currSelected == MENU_ITEM_MODE){
            if (gameControl.gameMode == GAME_MODE_HARD) gameControl.gameMode = GAME_MODE_EASY; else gameControl.gameMode++;
            FakeEEPROM EEPROM;
            EEPROM.put(EEPROM_GAME_MODE_BYTE + EEPROM_STORAGE_SPACE_START, gameControl.gameMode);
        }
    }
    
   pocketDisplay.setCursor(0, 56);
   pocketDisplay.print(F("V"VERSION_MAJOR"."VERSION_MINOR));
    
    pocketDisplay.drawBitmap(m_fontPosition, 5, title, 90, 31, 0x00);
  
    
    
}