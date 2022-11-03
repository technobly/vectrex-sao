/* 
 * File:   eeprom.h
 * Author: Brett Walach
 *
 * Created on November 1st, 2022
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char readEEPROM(unsigned char address);
void writeEEPROM(unsigned char address, unsigned char data);

#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

