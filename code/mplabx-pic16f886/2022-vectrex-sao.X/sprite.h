/* 
 * File:   sprite.h
 * Author: Brett Walach
 *
 * Created on October 9, 2022, 11:01 AM
 */

#ifndef SPRITE_H
#define	SPRITE_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define SPRITE_SIZE_OFFSET (0)
#define SPRITE_DX_OFFSET (1)
#define SPRITE_DY_OFFSET (2)
#define SPRITE_DATA_OFFSET (3)

void drawSprite(uint8_t* s, uint8_t bright);
void drawObject(uint8_t* s, uint8_t index, uint8_t size, uint8_t dx, uint8_t dy, uint8_t bright);
void blinkSprite(uint8_t* s, uint8_t rate, uint8_t bright_min, uint8_t bright_max);
void eraseSprite(uint8_t* s);
uint8_t isHitX(uint8_t sprite1_dx, uint8_t sprite2_dx, int8_t offset);

#ifdef	__cplusplus
}
#endif

#endif	/* SPRITE_H */

