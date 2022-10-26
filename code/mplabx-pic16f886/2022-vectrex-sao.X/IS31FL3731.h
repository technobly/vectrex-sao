/* 
 * File:   IS31FL3731.h
 * Author: Brett Walach
 *
 * Created on October 3, 2022, 2:53 PM
 */

#ifndef IS31FL3731_H
#define	IS31FL3731_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define ISSI_ADDR_DEFAULT 0x74

#define ISSI_REG_CONFIG  0x00
#define ISSI_REG_CONFIG_PICTUREMODE 0x00
#define ISSI_REG_CONFIG_AUTOPLAYMODE 0x08
#define ISSI_REG_CONFIG_AUDIOPLAYMODE 0x18

#define ISSI_CONF_PICTUREMODE 0x00
#define ISSI_CONF_AUTOFRAMEMODE 0x04
#define ISSI_CONF_AUDIOMODE 0x08

#define ISSI_REG_PICTUREFRAME  0x01

#define ISSI_REG_SHUTDOWN 0x0A
#define ISSI_REG_AUDIOSYNC 0x06

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_BANK_FUNCTIONREG 0x0B    // helpfully called 'page nine'

uint8_t ledBegin(uint8_t addr);
void ledDrawPixel(int16_t x, int16_t y, uint16_t color);
void ledClear(void);

void ledSetPWM(uint8_t lednum, uint8_t pwm, uint8_t bank);
void ledAudioSync(uint8_t sync);
void setFrame(uint8_t b);
void ledDisplayFrame(uint8_t frame);

void ledSelectBank(uint8_t bank);
void ledWriteRegister8(uint8_t bank, uint8_t reg, uint8_t data);
uint8_t ledReadRegister8(uint8_t bank, uint8_t reg);

#ifdef	__cplusplus
}
#endif

#endif	/* IS31FL3731_H */

