/* 
 * File:   IS31FL3731.c
 * Author: Brett Walach
 *
 * Created on October 3, 2022, 2:53 PM
 */

#include "IS31FL3731.h"
#include "i2c.h"
#include "config.h"

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

uint8_t _i2caddr;
uint8_t _frame;

uint8_t ledBegin(uint8_t addr) {
    i2c_init(200000);   // 100kHz Master Mode
    
    _i2caddr = addr;
    _frame = 0;

    // shutdown
    ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);
    delay(10);

    // out of shutdown
    ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);

    // picture mode
    ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, ISSI_REG_CONFIG_PICTUREMODE);

    ledDisplayFrame(_frame);

    // all LEDs on & 0 PWM
    ledClear(); // set each led to 0 PWM

    for (uint8_t f = 0; f < 8; f++) {
        for (uint8_t i = 0; i <= 0x11; i++)
            ledWriteRegister8(f, i, 0xff);     // each 8 LEDs on
    }

    ledAudioSync(0);

    return 1;
}

void ledClear(void) {
    // all LEDs on & 0 PWM

    ledSelectBank(_frame);

    for (uint8_t i = 0; i < 6; i++) {
        i2c_start();
        i2c_write(_i2caddr);
        i2c_write((uint8_t) 0x24 + i * 24);
        // write 24 bytes at once
        for (uint8_t j = 0; j < 24; j++) {
            i2c_write(0);
        }
        i2c_stop();
    }
}

void ledSetPWM(uint8_t lednum, uint8_t pwm, uint8_t bank) {
    if (lednum >= 144) {
        return;
    }
    ledWriteRegister8(bank, 0x24 + lednum, pwm);
}

void ledDrawPixel(int16_t x, int16_t y, uint16_t color) {
//    switch (getRotation()) {
//    case 1:
//        _swap_int16_t(x, y);
//        x = 15 - x - 1;
//        break;
//    case 2:
//        x = 15 - x - 1;
//        y = 7 - y - 1;
//        break;
//    case 3:
//        _swap_int16_t(x, y);
//        y = 9 - y - 1;
//        break;
//    }

//    // charlie wing is smaller:
//    if ((x < 0) || (x >= 16) || (y < 0) || (y >= 7)) return;

    if (x > 7) {
        x = 15 - x;
        y += 8;
    } else {
        y = 7 - y;
    }

//    _swap_int16_t(x, y);

//    if (color > 255) color = 255; // PWM 8bit max

    ledSetPWM((uint8_t)(y + x * 16), (uint8_t)color, _frame);
    return;
}

void ledSetFrame(uint8_t f) {
    _frame = f;
}

void ledDisplayFrame(uint8_t f) {
    if (f > 7) {
        f = 0;
    }
    ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, f);
}


void ledSelectBank(uint8_t b) {
    i2c_start();
    i2c_write(_i2caddr);
    i2c_write((uint8_t)ISSI_COMMANDREGISTER);
    i2c_write(b);
    i2c_stop();
}

void ledAudioSync(uint8_t sync) {
    if (sync) {
        ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x1);
    } else {
        ledWriteRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, 0x0);
    }
}

void ledWriteRegister8(uint8_t b, uint8_t reg, uint8_t data) {
    ledSelectBank(b);

    i2c_start();
    i2c_write(_i2caddr);
    i2c_write((uint8_t)reg);
    i2c_write((uint8_t)data);
    i2c_stop();
    //Serial.print("$"); Serial.print(reg, HEX);
    //Serial.print(" = 0x"); Serial.println(data, HEX);
}

uint8_t ledReadRegister8(uint8_t bank, uint8_t reg) {
    uint8_t x;

    ledSelectBank(bank);

    i2c_start();
    i2c_write(_i2caddr);
    i2c_write((uint8_t)reg);
    i2c_stop();

    i2c_start();
    i2c_write(_i2caddr | 0x01); // read
//    x = i2c_read(_i2caddr, (uint8_t)1);
    x = i2c_read((uint8_t)1); // ACK (1)
    i2c_stop();

// Serial.print("$"); Serial.print(reg, HEX);
//  Serial.print(": 0x"); Serial.println(x, HEX);

    return x;
}