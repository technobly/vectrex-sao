/* 
 * File:   eeprom.c
 * Author: Brett Walach
 *
 * Created on November 1st, 2022
 */

#include "eeprom.h"
#include "config.h"

unsigned char readEEPROM(unsigned char address) {
    uint8_t data;
    EEADR = address;   // address to be read
    EEPGD = 0;         // select EEPROM Data Memory
    RD = 1;            // initialize read cycle
    data = EEDATA;
    return data;
}

void writeEEPROM(unsigned char address, unsigned char data) {
    while (WR == 1);   // waits until Last attempt To write Is finished
    EEADR = address;   // address to write
    EEDATA = data;     // data to write
    EEPGD = 0;         // select EEPROM Data Memory
    WREN = 1;          // enable writing of EEPROM
    GIE = 0;           // disable global interrupts
    EECON2 = 0x55;     // required sequence for write to internal EEPROM
    EECON2 = 0xAA;     //  |
    WR = 1;            // init write cycle
    GIE = 1;           // enable global interrupts
    WREN = 0;          // to disable write
    WR = 0;            // clear WR register
    while (EEIF == 0); // wait for completion of write operation
    EEIF = 0;          // clear EEIF bit that was just set
}
