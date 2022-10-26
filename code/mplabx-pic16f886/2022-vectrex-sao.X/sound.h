/* 
 * File:   sound.h
 * Author: Brett Walach
 *
 * Created on October 9, 2022, 10:54 AM
 */

#ifndef SOUND_H
#define	SOUND_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

uint16_t maxDutyCycle(void);
void playNote(uint16_t freq);
uint8_t playSong(uint16_t* song, uint8_t ms, uint8_t quiet);


#ifdef	__cplusplus
}
#endif

#endif	/* SOUND_H */

