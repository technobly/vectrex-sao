/*
 * File:   main.c
 * Author: Brett Walach
 *
 * Created on September 23, 2022, 4:42 PM
 */

#include <xc.h>
#include "config.h"
#include "IS31FL3731.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "notes.h"
#include "sound.h"
#include "sprite.h"

// This would be the best octave, but we can't go that low
//uint16_t song_vec_startup[] = { 0,1500, A3,232, 0,10, A3,116, G4,1200, 0,2000, 1,1 }; // 1,1 is end of song
uint16_t song_vec_startup[] = { 0,1500, A4,232, 0,10, A4,116, G5,1200, 0,2000, 1,1 }; // 1,1 is end of song

// 3x4 number font
const uint8_t num3by4[] = {
    0b00000111, // 0
    0b00000101,
    0b00000101,
    0b00000111,
    0b00000001, // 1
    0b00000001,
    0b00000001,
    0b00000001,
    0b00000111, // 2
    0b00000010,
    0b00000100,
    0b00000111,
    0b00000111, // 3
    0b00000010,
    0b00000001,
    0b00000111,
    0b00000101, // 4
    0b00000111,
    0b00000001,
    0b00000001,
    0b00000111, // 5
    0b00000110,
    0b00000001,
    0b00000111,
    0b00000011, // 6
    0b00000110,
    0b00000101,
    0b00000111,
    0b00000111, // 7
    0b00000001,
    0b00000010,
    0b00000100,
    0b00000111, // 8
    0b00000010,
    0b00000101,
    0b00000111,
    0b00000111, // 9
    0b00000101,
    0b00000011,
    0b00000110,
};

// 3x4 number font (blank))
const uint8_t num3by4_blank[] = {
    0b01110111, // BLANK
    0b01110111,
    0b01110111,
    0b01110111,
};

// ship
uint8_t sprite1[] = {
    3,
    6,
    0,
    0b00000001, // 1000000 (shifted down 0)
    0b01100000, // 0110000
    0b00000001  // 1000000
};
// mountain base
uint8_t sprite2[] = {
    3,
    0,
    7,
    0b00000100, // 0000100 (shifted down 7)
    0b00001010, // 0001010
    0b01110001  // 1110001
};
// ship thrust
uint8_t sprite3[] = {
    1,
    6,
    1,
    0b00000001, // 1000000 (shifted down 1)
};
// ship bomb
uint8_t sprite4[] = {
    1,
    6,
    2,
    0b01000000, // 0100000 (shifted down 1)
};
// ship explosion animation (copy offsets from ship)
const uint8_t shipExplosion[] = {
//  0b00000001, // 1000000 (shifted down 0)
//  0b01100000, // 0110000
//  0b00000001  // 1000000
    
    0b00000001, // frame 1
    0b01000000, 
    0b00000000,
    0b00100000,
    0b00000001,
    
    0b01000000, // frame 1
    0b00000000, 
    0b00000000,
    0b00000000,
    0b00100000,
    
    0b00000000, // frame 1
    0b00000000, 
    0b00000000,
    0b00000000,
    0b00000000,
};



uint8_t temp;

// The lookup table to make the brightness changes be more visible
uint8_t sweep[] = { 1, 2, 3, 4, 6, 8, 10, 15, 20, 30, 40, 60, 60, 40, 30, 20, 15, 10, 8, 6, 4, 3, 2, 1 };

uint8_t touchPressed = 1;
uint8_t toggleLed = 0;
uint8_t portbTemp = 0;  // variable to use as PORTB dump
uint8_t startupOnce = 0;

//void interrupt(void) {
////    if (INTF) {
////        touchPressed = 1;
////        INTF = 0;
////    }
//     
//    if (RBIF) {
//        portbTemp = PORTB;
////        if ((portbTemp & 0x02) == 0) { // check if RB1 is shorted to GND
//            touchPressed = 1;
////        }
//        RBIF = 0;
//    }
//}
//
//void main() {
//    TRISB0 = 0;         // SET RB0 AS OUTPUT
//    TRISB1 = 1;         // SET RB1 AS INPUT
//    ANSEL = 0;      // DISABLE ANALOG FUNCTION ON PORTB
//    ANSELH = 0;      // DISABLE ANALOG FUNCTION ON PORTB
//    C1ON = 0;
//    C2ON = 0;
//    nRBPU = 0;          // Enable Global WPUB
//    WPUB1 = 1;         // Enable WPUB for RB1
////    INTEDG = 0;         // Interrupt from High TO Low
//    IOCB1 = 1;          // RB1 is used to create an interrupt
//    RBIF = 0;
//    RBIE = 1;           // Turn on IOCB
//    GIE = 1;            // Enable Global Interrupt
////    INTE = 1;           // Enable External Interrupt
//    
//    
////    RBIF = 0;           // Clear the interrupt flag
////    INTF = 0;           // Clear External Interrupt Flag
// 
//    while(1) {
//        if (touchPressed) {
//            if (++toggleLed & 1) {
//                RB0 = 1;
//            } else {
//                RB0 = 0;
//            }
//            delay(200);
//            touchPressed = 0;
//        }
//    }
//}     

void main(void) {
//    delay(200);
   
//    i2c_init(100000);   // 100kHz Master Mode
    ledBegin(ISSI_ADDR_DEFAULT << 1);
    
    srand((int)0x55);
 
    int i = 0;
//    TRISB0 = 0;         // RB0 as Output PIN
//    TRISB1 = 1;         // RB1 as Input PIN
    TRISB0 = 1;         // RB0 as Input PIN
    ANSEL = 0;          // DISABLE ANALOG FUNCTION ON PORTB
    ANSELH = 0;         // DISABLE ANALOG FUNCTION ON PORTB
    C1ON = 0;
    C2ON = 0;
    nRBPU = 0;          // Enable Global WPUB
//    WPUB1 = 1;          // Enable WPUB for RB1
    WPUB0 = 1;          // Enable WPUB for RB0

    
//    uint8_t noteSize = sizeof(notes)/sizeof(notes[0]);
//    while (1) {
////        for (uint8_t note = 0; note < noteSize; note++) {
////            playNote(notes[note]);
////            delay(100);
////        }
////        for (uint8_t note = noteSize - 2; note > 0; note--) {
////            playNote(notes[note]);
////            delay(100);
////        }
//        delay(2000);
//        playNote(A4);
//        delay(232);
//        playNote(0);
//        delay(10);
//        playNote(A4);
//        delay(116);
//        playNote(A5);
//        delay(1200);
//        playNote(0);
//    }
    
    // SUPER LOOP
    while (1) {
        
//    // STARTUP SCREEN
    while(1) {
        static uint8_t counter = 0;
        static uint8_t dwell_count = 0;

        if (++dwell_count & 1) {
            // BOTTOM
            static uint8_t brightness = 30;
            for (uint8_t x = 0; x < 7; x++) {
                temp = (x + counter) % 4;
                if (temp == 0) {
                    ledDrawPixel(9, x, brightness);
                } else {
                    ledDrawPixel(9, x, 0);
                }
            }
            // RIGHT
            for (uint8_t y = 0; y < 10; y++) {
                temp = (y - counter) % 4;
                if (temp == 0) {
                    ledDrawPixel(y, 6, brightness);
                } else {
                    ledDrawPixel(y, 6, 0);
                }
            }
            // TOP
            for (uint8_t x = 0; x < 7; x++) {
                temp = (x - counter) % 4;
                if (temp == 0) {
                    ledDrawPixel(0, x, brightness);
                } else {
                    ledDrawPixel(0, x, 0);
                }
            }
            // LEFT
            for (uint8_t y = 0; y < 10; y++) {
                temp = (y + counter) % 4;
                if (temp == 0) {
                    ledDrawPixel(y, 0, brightness);
                } else {
                    ledDrawPixel(y, 0, 0);
                }
            }
            counter++;
            if (counter > 3) {
                counter = 0;
            }
        } else {
            delay(24); // alternate a delay to slow down the marquee
        }
        if (!startupOnce) {
            if (!playSong(song_vec_startup, 24, 0)) { // 24ms is how long this routine's loop time is
                startupOnce = 1;
                break;
            }
        } else {
            if (!playSong(song_vec_startup, 24, 1)) { // 24ms is how long this routine's loop time is
                break;
            }
        }
    }
    ledClear();
    
    //================
    // SCRAMBLE
    //================
    
    // DEBUG
//    #define SHIP_DWELL_SLOW 255
//    #define SHIP_DWELL_FAST 255
//    #define SHIP_ACCELERATE_DX 60
//    #define SHIP_DECELERATE_DX 60
//    #define BG_ACCELERATE_DX 60
//    #define BG_DECELERATE_DX 60
//    uint8_t bombRate[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    
    #define SHIP_DWELL_SLOW 60
    #define SHIP_DWELL_FAST 40
    #define SHIP_ACCELERATE_DX 3
    #define SHIP_DECELERATE_DX 1
    #define BG_ACCELERATE_DX 0
    #define BG_DECELERATE_DX 2
    uint8_t bombRate[] = { 1, 1, 4, 2, 1, 1, 1, 1, 1, 1 };

    #define BG_BRIGHTNESS_MAX 60
    #define BG_BRIGHTNESS_MIN 30
    #define THRUST_BRIGHTNESS_MAX 60
    #define THRUST_BRIGHTNESS_MIN 30
    #define BOMB_BRIGHTNESS_MAX 150
    #define BOMB_BRIGHTNESS_MIN 60
    #define SHIP_BRIGHTNESS_MAX 30
    #define SHIP_BRIGHTNESS_MIN 30
    uint8_t moveShip = 0;
    uint8_t dirShip = 1;
    uint8_t dxShip = 1;
    uint8_t dwellShip = 0;
    uint8_t moveBg = 0;
    uint8_t dxBg = 0;
    uint8_t bombActive = 0;
    uint16_t bombNote = 0;
    uint8_t bombCount = 0;
    uint8_t bombHit = 0;
    uint8_t touchCount = 0;
    uint8_t numIndex = 0;
    uint8_t scoreHi = 0;
    uint8_t scoreLo = 0;
    uint8_t scoreCount = 0;
    uint8_t fuel = 255;
    uint8_t gameOver = 0;
//    
//    while (1) {
//        sprite1[SPRITE_DX_OFFSET] = 4;
//        sprite1[SPRITE_DY_OFFSET] = 4;
//        drawSprite(sprite1, SHIP_BRIGHTNESS_MIN);
//        delay(1000);
//        drawSprite(sprite1, 0);
//        drawObject(shipExplosion, 0 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, SHIP_BRIGHTNESS_MAX);
//        delay(1000);
//        drawObject(shipExplosion, 0 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
//        drawObject(shipExplosion, 1 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, SHIP_BRIGHTNESS_MAX);
//        delay(1000);
//        drawObject(shipExplosion, 1 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
//        drawObject(shipExplosion, 2 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, SHIP_BRIGHTNESS_MAX);
//        delay(1000);
//        drawObject(shipExplosion, 2 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
//    }

    while (1) {
        if (scoreCount) {
            drawObject(num3by4, scoreLo * 4, 4, 0, 3, SHIP_BRIGHTNESS_MIN);
            drawObject(num3by4, scoreHi * 4, 4, 4, 3, SHIP_BRIGHTNESS_MIN);
        } else {
            if (!gameOver) {
                drawSprite(sprite1, SHIP_BRIGHTNESS_MIN);
                if (bombHit) {
                    blinkSprite(sprite2, 4, BG_BRIGHTNESS_MIN, BG_BRIGHTNESS_MAX);
                } else {
                    drawSprite(sprite2, BG_BRIGHTNESS_MIN);
                }
                if (!dxBg) blinkSprite(sprite3, 2, THRUST_BRIGHTNESS_MIN, THRUST_BRIGHTNESS_MAX);
                if (bombActive) drawSprite(sprite4, BOMB_BRIGHTNESS_MIN);
                if (bombHit) blinkSprite(sprite4, 2, BOMB_BRIGHTNESS_MIN, BOMB_BRIGHTNESS_MAX);
            }
            
            else {
                uint8_t shipExplodeFrame = 0;
                uint8_t drawOrErase = 0;
                drawSprite(sprite2, BG_BRIGHTNESS_MIN);
                bombNote = 16;
                for (uint16_t x = 0; x < 250; x++) {
                    if (x & 1) {
//                        playNote((uint8_t)(rand() & 0xf) + 40); // 50Hz ~ 113Hz);
                        playNote(bombNote);
                    } else {
                        playNote(0);
                    }
//                    if (bombNote >= 50) {
//                        bombNote = 0;
//                    }
//                    if (RB0 == 0) {
//                        bombNote++;
//                        delay(30);
//                        while (RB0 == 0);
//                    }
//                    delay(1);
                    if ((x & 0x0f) == 0x0f) {   
                        if (shipExplodeFrame > 0) {
                            drawObject(shipExplosion, (shipExplodeFrame-1) * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
                        } else {
                            drawObject(shipExplosion, 2 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
                        }
                        drawObject(shipExplosion, shipExplodeFrame * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, SHIP_BRIGHTNESS_MAX);
                        drawSprite(sprite2, BG_BRIGHTNESS_MIN);
                        if (shipExplodeFrame < 3) {
                            shipExplodeFrame++;
                        } else {
                            shipExplodeFrame = 0;
                        }
                    }
                }
                playNote(0);
                break;
            }
        }
        
        delay(20);
        
        if (scoreCount) {
            drawObject(num3by4, scoreLo * 4, 4, 0, 3, 0);
            drawObject(num3by4, scoreHi * 4, 4, 4, 3, 0);
        } else {
            eraseSprite(sprite1);
            eraseSprite(sprite2);
            if (!dxBg) eraseSprite(sprite3);
            if (bombActive || bombHit) eraseSprite(sprite4);
        }
        if (scoreCount) {
            scoreCount--;
        }
      
        // move sprite1 (ship)
        if (dwellShip == 0) {
            if (++moveShip > dxShip) {
                moveShip = 0;
                if (dirShip) {
                    dxBg = BG_ACCELERATE_DX;
                    if (sprite1[SPRITE_DX_OFFSET] > 2) {
                        sprite1[SPRITE_DX_OFFSET] -= 1;
                        sprite3[SPRITE_DX_OFFSET] -= 1;
//                        if (!bombActive) sprite4[SPRITE_DX_OFFSET] -= 1;
                    } else {
                        dirShip = 0;
                        dxShip = SHIP_ACCELERATE_DX;
                        dwellShip = SHIP_DWELL_FAST;
                        
                    }
                } else {
                    dxBg = BG_DECELERATE_DX;
                    if (sprite1[SPRITE_DX_OFFSET] < 6) {
                        sprite1[SPRITE_DX_OFFSET] += 1;
                        sprite3[SPRITE_DX_OFFSET] += 1;
//                        if (!bombActive) sprite4[SPRITE_DX_OFFSET] += 1;
                    } else {
                        dirShip = 1;
                        dxShip = SHIP_DECELERATE_DX;
                        dwellShip = SHIP_DWELL_SLOW;
                    }
                }
            }
        } else {
            dwellShip--;
        }
        
        // Read touch joystick
        if (RB0 == 0) {
            touchCount++;
            if (touchCount > 50) {
                playNote(0);
                break;
            }
            // drop sprite4 (bomb)
            if (!bombHit && !bombActive) {
                bombActive = 1;
                bombCount = 0;
                bombNote = G6; // fly like a G6!
                playNote(bombNote);
                sprite4[SPRITE_DX_OFFSET] = sprite3[SPRITE_DX_OFFSET];
            }
        } else {
            touchCount = 0;
        }
        // Check fuel level
        if (fuel < 80) {
            if (fuel > 60) {
                sprite1[SPRITE_DY_OFFSET] = 1;
                sprite3[SPRITE_DY_OFFSET] = 1;
            } else if (fuel > 40) {
                sprite1[SPRITE_DY_OFFSET] = 2;
                sprite3[SPRITE_DY_OFFSET] = 2;
            } else if (fuel > 20) {
                sprite1[SPRITE_DY_OFFSET] = 3;
                sprite3[SPRITE_DY_OFFSET] = 3;
            } else if (fuel > 10) {
                sprite1[SPRITE_DY_OFFSET] = 4;
                sprite3[SPRITE_DY_OFFSET] = 4;
            } else {
                gameOver = 1;
            }
        } else {
            sprite1[SPRITE_DY_OFFSET] = 0;
            sprite3[SPRITE_DY_OFFSET] = 0;
        }
        if (bombActive) {
            if (++bombCount > bombRate[sprite4[SPRITE_DY_OFFSET]]) {
                bombCount = 0;
                if (sprite4[SPRITE_DY_OFFSET] > 8) {
                    playNote(0);
                    bombActive = 0;
                    // if bomb is at max depth right in line
                    // with mountain base, bomb hit!
                    // Mountain base peak offset = 2
                    if (isHitX(sprite4[SPRITE_DX_OFFSET], sprite2[SPRITE_DX_OFFSET], 3)) {
                        bombHit = 1;
                        bombCount = 0;
                        if (!dxBg) { // if moving fast, more reward!
                            scoreLo += 3;
                        } else {
                            scoreLo++;
                        }
                        if (scoreLo > 9) {
                            scoreLo -= 10;
                            scoreHi++;
                        }
                    } else {
                        sprite4[SPRITE_DY_OFFSET] = 2;
                    }
                } else {
                    sprite4[SPRITE_DY_OFFSET] += 1;
                }
            }
            if (bombActive) {
                bombNote -= 25;
                playNote(bombNote);
            }
        }
        if (bombHit) {
            if (bombCount == 0) {
                bombNote = rand() & 0x3f + 50; // 50Hz ~ 113Hz
            }
            if (++bombCount & 1) {
                playNote(bombNote);
            } else {
                playNote(50);
            }
            if (bombCount > 30) {
                bombHit = 0;
                playNote(0);
                bombActive = 0;
                sprite4[SPRITE_DY_OFFSET] = 2;
                scoreCount = 20; // display score
                if (fuel < 235) {
                    fuel += 40; // refuel after score displayed
                }
            }
        }
        // move sprite2 (background)
        if (!bombHit && ++moveBg > dxBg) {
            moveBg = 0;
            if (sprite2[SPRITE_DX_OFFSET] < 6) {
                sprite2[SPRITE_DX_OFFSET] += 1;
                fuel--;
            } else {
                sprite2[SPRITE_DX_OFFSET] = 0;
//                fuel--;
            }
        }
    }
    
//    // RANDOM MOVEMENT and BLINK   
//    int8_t random_x = rand() & 0x7;
//    if (random_x > 6) random_x = 6;
//    int8_t random_y = rand() & 0xF;
//    if (random_y > 10) random_y = 9;
//    int8_t random_dx = rand() & 1;
//    if (random_dx == 0) {
//        random_dx = -1;
//    }
//    int8_t random_dy = rand() & 1;
//    if (random_dy == 0) {
//        random_dy = -1;
//    }
//    uint8_t moveCounter = 0;
//    uint8_t changeCounter = rand() & 0xf;
//    while(1) {
//        if (--changeCounter == 0) {
//            random_dx = rand() & 1;
//            if (random_dx == 0) {
//                random_dx = -1;
//            }
//            random_dy = rand() & 1;
//            if (random_dy == 0) {
//                random_dy = -1;
//            }
//            changeCounter = (rand() & 0xf) + 9;
//        }
////        RB0 = 1;
//        ledDrawPixel(random_y, random_x, 100);
//        playNote(16);
//        delay(10);
////        RB0 = 0;
//        ledDrawPixel(random_y, random_x, 10);
//        playNote(0);
//        delay(10);
//        ledDrawPixel(random_y, random_x, 0);
//        if (++moveCounter > 3) {
//            moveCounter = 0;
//            random_x += random_dx;
//            if (random_x < 0) {
//                random_x = 6;
//            } else if (random_x > 6) {
//                random_x = 0;
//            }
//            random_y += random_dy;
//            if (random_y < 0) {
//                random_y = 9;
//            } else if (random_y > 9) {
//                random_y = 0;
//            }
//        }
//    }
    
    
    } // SUPER LOOP
} // main()
