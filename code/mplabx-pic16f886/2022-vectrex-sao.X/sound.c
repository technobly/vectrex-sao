/*
 * File:   sound.c
 * Author: Brett Walach
 *
 * Created on September 23, 2022, 4:42 PM
 */

#include <xc.h>
#include "config.h"
#include "notes.h"

uint16_t pwm_freq = 0;
uint8_t tmr2_prescale = 16;

uint16_t maxDutyCycle(void) {
    return (_XTAL_FREQ / (pwm_freq * tmr2_prescale));
}

/*
 * playNote(C4 ~ B6); or playNote(262 ~ 1976)
 */

void playNote(uint16_t freq) {
    pwm_freq = freq;
    if (pwm_freq > A5) {
        tmr2_prescale = 4;
    } else {
        tmr2_prescale = 16;
    }
    PR2 = (_XTAL_FREQ/(pwm_freq * 4 * tmr2_prescale)) - 1;
    TMR2IF = 0; // Clear Timer2 interrupt
    CCP1M3 = 1; // Configure the CCP1 module
    CCP1M2 = 1;
    // Configure the Timer module
    if (tmr2_prescale == 1) {
        T2CKPS0 = 0;
        T2CKPS1 = 0;
    } else if (tmr2_prescale == 4) {
        T2CKPS0 = 1;
        T2CKPS1 = 0;
    } else { // tmr2_prescale == 16
        T2CKPS0 = 1;
        T2CKPS1 = 1;
    }
    TMR2ON = 1; // Enable Timer2
    TRISC2 = 0; // RC2 is PWM output

    uint16_t duty = maxDutyCycle() / 2;
//    uint16_t duty = maxDutyCycle() / 32; // quieter, but actually louder
    CCP1X = duty & 1;   // Store the 1st bit
    CCP1Y = duty & 2;   // Store the 0th bit
    CCPR1L = duty >> 2; // Store the remaining 8 bits
}

/*
 * uint16_t song_vec_startup[] = { 0,1500, A4,232, 0,10, A4,116, A5,1200, 0,2000, 1,1 }; // 1,1 is end of song
 */
uint8_t playSong(uint16_t* song, uint8_t ms, uint8_t silent) {
    static uint16_t lastMs = 0;
    static uint8_t lastNote = 0;

    if (lastMs >= song[lastNote+1]) {
        lastNote += 2;
        lastMs = 0;
    }

    if (song[lastNote] == 1 && song[lastNote+1] == 1) {
        lastMs = 0;
        return 0;
    }

    if (silent) {
        playNote(0); // Song can be used for timing, and maybe you want it to be silent, but still have the same effect on delays
    } else {
        playNote(song[lastNote]);
    }

    lastMs += ms;

    return 1;
}
