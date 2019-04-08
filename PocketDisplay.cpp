/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PocketDisplay.cpp
 * Author: Schuemi
 * 
 * Created on 4. April 2019, 17:21
 */

#include "PocketDisplay.h"
#include <PocketStar.h>
#include "Utils.h"

PocketDisplay pocketDisplay;

PocketDisplay::PocketDisplay() {
    displayBuffer = new uint8_t[DISPLAY_WIDTH*DISPLAY_HEIGHT];
    fontTransparent = true;
    fontBold = false;
}

PocketDisplay::PocketDisplay(const PocketDisplay& orig) {
}

PocketDisplay::~PocketDisplay() {
}
void PocketDisplay::drawPixel(uint8_t x, uint8_t y, uint8_t color){
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
    displayBuffer[x+(y*DISPLAY_WIDTH)] = color;
}
void PocketDisplay::show(){
    pocketstar.goTo(0,0);
    pocketstar.startTransfer();
    pocketstar.writeBuffer(displayBuffer, DISPLAY_WIDTH*DISPLAY_HEIGHT);
    pocketstar.endTransfer();
}
void PocketDisplay::clearBuffer(){
    memset(displayBuffer, 0x00, DISPLAY_WIDTH*DISPLAY_HEIGHT);
}
void PocketDisplay::setCursor(uint8_t x, uint8_t y) {
    cursorX = x;
    cursorY = y;
}  
void PocketDisplay::setFontBold(bool bold, uint8_t color) {
    fontBold = bold;
    fontColorBold = color;
}
void PocketDisplay::drawBitmap(uint8_t xpos, uint8_t ypos, const uint8_t* data, uint8_t w, uint8_t h, int16_t transparent) {
    for (uint8_t y = 0; y < h; y++){
        for (uint8_t x = 0; x < w; x++){
            if (transparent == -1 || transparent != data[x+(y*w)])
                drawPixel(x+xpos,y+ypos,data[x+(y*w)]);
        }
    }
}

//0x02,0x07,0x02 (3,3)
//00000010
//00000111
//00000010

void PocketDisplay::drawBitmap1bpp(uint8_t xpos, uint8_t ypos, const uint8_t* data, uint8_t w, uint8_t h) {
    uint8_t rh = h + h%8;
    for (uint8_t y = 0; y < h; y++){
        for (uint8_t x = 0; x < w; x++){
            uint8_t byte = data[y / 8 + x *(rh/8)];
            if (CHECK_BIT(byte, y%8))
                drawPixel(x+xpos,y+ypos,0xff);
            else
                drawPixel(x+xpos,y+ypos,0x00);
        }
    }
}
void PocketDisplay::drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void PocketDisplay::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
  // bresenham's algorithm
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    Utils::swap(x0, y0);
    Utils::swap(x1, y1);
  }

  if (x0 > x1) {
    Utils::swap(x0, x1);
    Utils::swap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++)
  {
    if (steep)
    {
      drawPixel(y0, x0, color);
    }
    else
    {
      drawPixel(x0, y0, color);
    }

    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}
size_t PocketDisplay::write(uint8_t c) {
    
    if (fontBold) {
        uint8_t ocx = cursorX;
        uint8_t f = fontColor;
        
        cursorX++;
        fontColor = fontColorBold;
        writec(c);
        fontColor = f;
        cursorX = ocx;
    }
    return writec(c);
}
size_t PocketDisplay::writec(uint8_t c) {
    if (!fontFirstChar)
        return 1;
    if (c < fontFirstChar || c > fontLastChar)
        return 1;
    if (cursorX >= DISPLAY_WIDTH || cursorY >= DISPLAY_HEIGHT)
        return 1;
    
    uint8_t charWidth = pgm_read_byte(&fontDescriptor[c - fontFirstChar].width);
   
    
    uint8_t bytesPerRow = charWidth / 8;
    if (charWidth > bytesPerRow * 8)
        bytesPerRow++;
    uint16_t offset = pgm_read_word(&fontDescriptor[c - fontFirstChar].offset) + (bytesPerRow * fontHeight) - 1;
    for (uint8_t y = 0; y < fontHeight && y + cursorY < DISPLAY_HEIGHT; y++) {
        if (! fontTransparent)  drawPixel(cursorX,cursorY+y,fontColorBackground);
        
        for (uint8_t byte = 0; byte < bytesPerRow; byte++) {
            uint8_t data = pgm_read_byte(fontBitmap + offset - y - ((bytesPerRow - byte - 1) * fontHeight));
            uint8_t bits = byte * 8;
            for (uint8_t i = 0; i < 8 && (bits + i) < charWidth && (bits + i + cursorX) < DISPLAY_WIDTH; i++) {
                if (data & (0x80 >> i)) {
                   drawPixel(cursorX+i,cursorY+y,fontColor);
                 } else {
                   if (! fontTransparent) drawPixel(cursorX+i,cursorY+y,fontColorBackground);
                 }
            }
        }
       if (! fontTransparent && (cursorX + charWidth) < DISPLAY_WIDTH) {
           drawPixel(cursorX+charWidth,cursorY+y,fontColorBackground);
       }
        
    }
    if (fontFixedWidth) cursorX += (fontFixedWidth + 1); else cursorX += (charWidth + 1);
    return 1;
}

void PocketDisplay::setFont(const PSFont &font, uint8_t fixedWidth) {
    fontHeight = font.height;
    fontFirstChar = font.startChar; 
    fontLastChar = font.endChar;
    fontDescriptor = font.charInfo;
    fontBitmap = font.bitmap;
    fontFixedWidth = fixedWidth;
   
    
}

void PocketDisplay::setFontColor(uint8_t color, uint8_t colorBackground) {
    fontColor = color;
    fontColorBackground = colorBackground;
    fontTransparent = false;
}
void PocketDisplay::setFontColor(uint8_t color) {
    fontColor = color;
    fontTransparent = true;
}
