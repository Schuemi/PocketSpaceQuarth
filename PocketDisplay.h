/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PocketDisplay.h
 * Author: Schuemi
 *
 * Created on 4. April 2019, 17:21
 */

#ifndef POCKETDISPLAY_H
#define POCKETDISPLAY_H
#include <Print.h>
#include <PS_FontDefinitions.h>

#define DISPLAY_HEIGHT 64
#define DISPLAY_WIDTH 96

class PocketDisplay : public Print {
public:
    PocketDisplay();
    PocketDisplay(const PocketDisplay& orig);
    virtual ~PocketDisplay();
    void drawPixel(uint8_t x, uint8_t y, uint8_t color);
    void show();
    void clearBuffer();
    void setCursor(uint8_t x, uint8_t y);
    size_t write(uint8_t);
    size_t writec(uint8_t);
    void setFont(const PSFont &font, uint8_t fixedWidth = 0);
    void setFontColor(uint8_t color, uint8_t colorBackground);
    void setFontColor(uint8_t color);
    void setFontBold(bool bold, uint8_t color = 0x00);
    void drawBitmap(uint8_t x, uint8_t y, const uint8_t* data, uint8_t w, uint8_t h, int16_t transparent = -1);
    void drawBitmap1bpp(uint8_t xpos, uint8_t ypos, const uint8_t* data, uint8_t w, uint8_t h);
    void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
    void drawCircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
private:
    uint8_t* displayBuffer;
    uint8_t cursorX;
    uint8_t cursorY;
    uint8_t fontColor;
    uint8_t fontColorBackground;
    bool fontTransparent;
    uint8_t fontFixedWidth;
    
    uint8_t       fontHeight;
    char          fontFirstChar;
    char          fontLastChar;
    const PSCharInfo    *fontDescriptor;
    const uint8_t       *fontBitmap;
    bool fontBold;
    uint8_t fontColorBold;
};
extern PocketDisplay pocketDisplay;
#endif /* POCKETDISPLAY_H */

