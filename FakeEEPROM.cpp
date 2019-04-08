/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FakeEEPROM.cpp
 * Author: Schuemi
 * 
 * Created on 8. April 2019, 13:24
 */

#include "FakeEEPROM.h"







FakeEEPROM::FakeEEPROM() {
    SD = &_SD;
    /*if (!pocketstar.isSDConnected()) {
        sdAviable = false;
        return;
    }*/
    
    sdAviable = true;
    // create directory if it doesn't exist
    if (!SD->exists(_EEPROM_FAKE__FOLDER))
        SD->mkdir(_EEPROM_FAKE__FOLDER);
    if (!SD->exists(_EEPROM_FAKE__PATH)) {
        File file = SD->open(_EEPROM_FAKE__PATH, FILE_WRITE);
        file.seek(0);
        uint8_t* data = new uint8_t[512];
        memset(data, 0xff, 512);
        file.write(data, 512);
        delete[] data;
        file.close();
        
    }
}

FakeEEPROM::FakeEEPROM(const FakeEEPROM& orig) {
}

FakeEEPROM::~FakeEEPROM() {
}

