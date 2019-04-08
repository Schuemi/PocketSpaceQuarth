/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FakeEEPROM.h
 * Author: Schuemi
 *
 * Created on 8. April 2019, 13:24
 */

#ifndef FAKEEEPROM_H
#define FAKEEEPROM_H
#include <PocketStar.h>
#include <SdFat.h>
#define _EEPROM_FAKE__PATH "games/spacequarth/SPACE_QUARTH.settings"
#define _EEPROM_FAKE__FOLDER "games/spacequarth"

class FakeEEPROM {
public:
    FakeEEPROM();
    FakeEEPROM(const FakeEEPROM& orig);
    virtual ~FakeEEPROM();
    
    template< typename T > T &get( int idx, T &t ){
    uint8_t *ptr = (uint8_t*) &t;
    
    if (!sdAviable || ! pocketstar.isSDConnected() || ! SD->exists(_EEPROM_FAKE__PATH)) {
        memset(ptr, 0xff, sizeof(T));
        return t;
    }
    
    File file = SD->open(_EEPROM_FAKE__PATH, FILE_READ);
    file.seek(idx);
    file.read(ptr, sizeof(T));
    file.close();
    
    return t;
}
    
template< typename T > const T &put( int idx, const T &t ){
    
    uint8_t *ptr = (uint8_t*) &t;
    
    if (!sdAviable || ! pocketstar.isSDConnected() || ! SD->exists(_EEPROM_FAKE__PATH)) {
        memset(ptr, 0xff, sizeof(T));
        return t;
    }
    
    File file = SD->open(_EEPROM_FAKE__PATH, FILE_WRITE);
    file.seek(idx);
    file.write(ptr, sizeof(T));
    file.close();
    
    return t;
}
    
    
private:
    SdFat* SD;
    boolean sdAviable;
};


#endif /* FAKEEEPROM_H */

