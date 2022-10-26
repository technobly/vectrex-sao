/* 
 * File:   sprite.h
 * Author: Brett Walach
 *
 * Created on October 9, 2022, 11:01 AM
 */

#include "sprite.h"
#include "IS31FL3731.h"

/*
 * uint8_t sprite1[] = {
 *     3,                   // SIZE
 *     0,                   // DX
 *     0,                   // DY
 *     0b00000100,          // sprite1[SPRITE_DATA_OFFSET + 0]
 *     0b00000011,          // sprite1[SPRITE_DATA_OFFSET + 1]
 *     0b00000100           // sprite1[SPRITE_DATA_OFFSET + 2]
 * };
 */
void drawSprite(uint8_t* s, uint8_t bright) {
    for (uint8_t i = 0; i < s[SPRITE_SIZE_OFFSET]; i++) {
        for (uint8_t b = 0; b < 7; b++) {
            if (s[i + SPRITE_DATA_OFFSET] & (1 << b)) {
                uint8_t x = b + s[SPRITE_DX_OFFSET];
                if (x > 6) {
                    x -= 7;
                }
                ledDrawPixel(i + s[SPRITE_DY_OFFSET], x, bright);
            }
        }
    }
}

void drawObject(uint8_t* s, uint8_t index, uint8_t size, uint8_t dx, uint8_t dy, uint8_t bright) {
    for (uint8_t i = 0; i < size; i++) {
        for (uint8_t b = 0; b < 7; b++) {
            if (s[i+index] & (1 << b)) {
                uint8_t x = b + dx;
                if (x > 6) {
                    x -= 7;
                }
                ledDrawPixel(i + dy, x, bright);
            }
        }
    }
}

void blinkSprite(uint8_t* s, uint8_t rate, uint8_t bright_min, uint8_t bright_max) {
    static uint8_t blink = 0;
    for (uint8_t i = 0; i < s[SPRITE_SIZE_OFFSET]; i++) {
        for (uint8_t b = 0; b < 7; b++) {
            if (s[i + SPRITE_DATA_OFFSET] & (1 << b)) {
                uint8_t x = b + s[SPRITE_DX_OFFSET];
                if (x > 6) {
                    x -= 7;
                }
                if (blink < rate) {
                    ledDrawPixel(i + s[SPRITE_DY_OFFSET], x, bright_max);
                } else if (blink < (rate * 2)) {
                    ledDrawPixel(i + s[SPRITE_DY_OFFSET], x, bright_min);
                } else {
                    blink = 0;
                }
            }
        }
    }
    blink++;
}

void eraseSprite(uint8_t* s) {
    for (uint8_t i = 0; i < s[SPRITE_SIZE_OFFSET]; i++) {
        for (uint8_t b = 0; b < 7; b++) {
            if (s[i + SPRITE_DATA_OFFSET] & (1 << b)) {
                uint8_t x = b + s[SPRITE_DX_OFFSET];
                if (x > 6) {
                    x -= 7;
                }
                ledDrawPixel(i + s[SPRITE_DY_OFFSET], x, 0);
            }
        }
    }
}

// SHIP CENTER, MOUNTAIN PEAK, MOUNTAIN PEAK INDEX
// 6, 3, 3
// 5, 2, 3
// 4, 1, 3
// 3, 0, 3
// 2, 6, 3 => -1+7=6
uint8_t isHitX(uint8_t orig_dx, uint8_t dest_dx, int8_t offset) {
    uint8_t hit = 0;
    int8_t test = orig_dx - offset;
    if (test > 6) {
        test -= 7;
    }
    if (test < 0) {
        test += 7;
    }
    if (dest_dx == (uint8_t)test) {
        hit = 1;
    }
    
//    if (sprite1_dx >= (uint8_t)test) {
//        if (sprite1_dx - test <= range) {
//            hit = 1;
//        }
//    } else {
//        if ((uint8_t)test - sprite1_dx <= range) {
//            hit = 1;
//        }
//    }

//    int8_t min = sprite2_dx + offset - range;
//    int8_t max = sprite2_dx + offset + range;
//    if (min > 6) {
//        min -= 7;
//    }
//    if (min < 0) {
//        min += 7;
//    }
//    if (max > 6) {
//        max -= 7;
//    }
//    if (min < 0) {
//        min += 7;
//    }
//    if (sprite1_dx >= (uint8_t)min && sprite1_dx <= (uint8_t)max) {
//        hit = 1;
//    }
    return hit;
}
