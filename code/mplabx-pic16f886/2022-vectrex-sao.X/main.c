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
#include "eeprom.h"

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

// Pattern to blank out 3 lines
const uint8_t blank_3[] = {
    0b01111111, // BLANK
    0b01111111,
    0b01111111,
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
    
    0b01000000, // frame 2
    0b00000000, 
    0b00000000,
    0b00000000,
    0b00100000,
    
    0b00000000, // frame 3
    0b00000000, 
    0b00000000,
    0b00000000,
    0b00000000,
};
// next
uint8_t nextSprite[] = {
    5,
    0,
    0,
    0b01000100, // 010100 (shifted down 3)
    0b00100010, // 001010
    0b00010001, // 000101
    0b00100010, // 001010
    0b01000100, // 010100
};

// high score "HI"
const uint8_t highScoreText[] = {
    0b01010111, // 1010111 (shifted down 7)
    0b01110010, // 1110010
    0b01010010, // 1010010
    0b01010111, // 1010111
};

uint8_t temp;
uint8_t touchPressed = 1;
uint8_t toggleLed = 0;
uint8_t portbTemp = 0;  // variable to use as PORTB dump
uint8_t startupOnce = 0;
uint8_t highscoreOnBoot = 1;

void main(void) {
    ledBegin(ISSI_ADDR_DEFAULT << 1);
    
    srand((int)0x55);   // Seed the random number generator
 
    TRISB0 = 1;         // RB0 as Input PIN
    ANSEL = 0;          // DISABLE ANALOG FUNCTION ON PORTB
    ANSELH = 0;         // DISABLE ANALOG FUNCTION ON PORTB
    C1ON = 0;
    C2ON = 0;
    nRBPU = 0;          // Enable Global WPUB
    WPUB0 = 1;          // Enable WPUB for RB0
    
    // SUPER LOOP
    while (1) {
        
    // STARTUP SCREEN
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
    uint8_t highScoreHi = 0;
    uint8_t highScoreLo = 0;
    uint8_t highScoreCount = 0;
    uint8_t highScoreToggle = 1;
    uint8_t scoreCount = 0;
    uint8_t scoreCountGameOver = 0;
    uint8_t fuel = 255;
    uint8_t gameOver = 0;
    uint8_t userInput = 0;

    ledClear();
    highScoreHi = readEEPROM(2);
    highScoreLo = readEEPROM(1);
    if (highScoreHi >= 9 && highScoreLo >= 9) {
        highScoreHi = 0;
        highScoreLo = 0;
        writeEEPROM(1, highScoreLo);
        writeEEPROM(2, highScoreHi);
    }
    if (highscoreOnBoot && (highScoreHi > 0 || highScoreLo > 0)) { // Show high score first boot if there is one
        scoreCountGameOver = 80;
    } else {
        highscoreOnBoot = 0;
    }
    while (1) {
        if (scoreCount) {
            drawObject(num3by4, scoreLo * 4, 4, 0, 3, SHIP_BRIGHTNESS_MIN);
            drawObject(num3by4, scoreHi * 4, 4, 4, 3, SHIP_BRIGHTNESS_MIN);
        } else if (scoreCountGameOver) {
            if (!highscoreOnBoot) {
                drawObject(num3by4, scoreLo * 4, 4, 0, 0, SHIP_BRIGHTNESS_MIN);
                drawObject(num3by4, scoreHi * 4, 4, 4, 0, SHIP_BRIGHTNESS_MIN);
            }
            if (highScoreHi > scoreHi || (highScoreHi == scoreHi && highScoreLo > scoreLo)) {
                if (highscoreOnBoot) {
                    drawObject(num3by4, highScoreLo * 4, 4, 0, 6, SHIP_BRIGHTNESS_MIN);
                    drawObject(num3by4, highScoreHi * 4, 4, 4, 6, SHIP_BRIGHTNESS_MIN);
                    drawObject(highScoreText, 0, 4, 0, 0, SHIP_BRIGHTNESS_MIN);
                } else {
                    if (highScoreToggle & 1) {
                        drawObject(num3by4, highScoreLo * 4, 4, 0, 6, SHIP_BRIGHTNESS_MIN);
                        drawObject(num3by4, highScoreHi * 4, 4, 4, 6, SHIP_BRIGHTNESS_MIN);
                    } else {
                        drawObject(highScoreText, 0, 4, 0, 6, SHIP_BRIGHTNESS_MIN);
                    }
                    if (++highScoreCount > 15) {
                        highScoreCount = 0;
                        highScoreToggle++;
                        if (highScoreToggle & 1) {
                            drawObject(highScoreText, 0, 4, 0, 6, 0);
                        } else {
                            drawObject(num3by4, highScoreLo * 4, 4, 0, 6, 0);
                            drawObject(num3by4, highScoreHi * 4, 4, 4, 6, 0);
                        }
                    }
                }
            }
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
            } else { // GAME OVER!!!!
                uint8_t shipExplodeFrame = 0;
                uint8_t drawOrErase = 0;
                drawSprite(sprite2, BG_BRIGHTNESS_MIN);
                bombNote = 16;
                for (uint16_t x = 0; x < 250; x++) {
                    if (x & 1) {
                        if (userInput) {
                            playNote(bombNote);
                        } else {
                            playNote(0);
                        }
                    } else {
                        playNote(0);
                    }
                    if ((x & 0x0f) == 0x0f) {   
                        if (shipExplodeFrame > 0) {
                            drawObject(shipExplosion, (shipExplodeFrame-1) * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
                        } else {
                            drawObject(shipExplosion, 2 * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, 0);
                        }
                        drawObject(shipExplosion, shipExplodeFrame * 5, 5, sprite1[SPRITE_DX_OFFSET], sprite1[SPRITE_DY_OFFSET]-1, SHIP_BRIGHTNESS_MAX);
                        drawSprite(sprite2, BG_BRIGHTNESS_MIN);
                        if (shipExplodeFrame < 2) {
                            shipExplodeFrame++;
                        } else {
                            shipExplodeFrame = 0;
                        }
                    }
                }
                ledClear(); // blank screen
                if (userInput) {
                    scoreCountGameOver = 80; // display final score
                    writeEEPROM(1, highScoreLo);
                    writeEEPROM(2, highScoreHi);
                } else {
                    playNote(0);
                    break; // EXIT GAME
                }
            }
        }
        
        delay(20); // Give each frame of visual data a bit of time to be displayed
        
        // Now erase objects to allow for them to be moved on the next frame
        if (scoreCount == 1) { // erase on last frame
            drawObject(num3by4, scoreLo * 4, 4, 0, 3, 0);
            drawObject(num3by4, scoreHi * 4, 4, 4, 3, 0);
        } else if (scoreCountGameOver) {
            // Don't erase this final score screen now, it just makes it flickery
        } else {
            eraseSprite(sprite1);
            eraseSprite(sprite2);
            if (!dxBg) eraseSprite(sprite3);
            if (bombActive || bombHit) eraseSprite(sprite4);
        }
        if (scoreCount) {
            scoreCount--;
        }
        if (scoreCountGameOver) {
            scoreCountGameOver--;
            if (scoreCountGameOver == 0) {
                if (gameOver) {
                    playNote(0);
                    break; // EXIT GAME
                } else {
                    if (highscoreOnBoot) {
                        highscoreOnBoot = 0;
                    }
                    ledClear(); // clear screen and start game
                }
            }
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
            userInput = 1;

//            // Switch to next game if button held??
//            touchCount++;
//            if (touchCount > 100) {
//                ledClear(); // blank screen
//                drawSprite(nextSprite, BOMB_BRIGHTNESS_MIN);
//                delay(2000);
//                playNote(0);
//                break;
//            }

            if (++touchCount < 50) { // only allow rapid fire for about 3 shots
                // drop sprite4 (bomb)
                if (!bombHit && !bombActive) {
                    bombActive = 1;
                    bombCount = 0;
                    bombNote = G6; // fly like a G6!
                    playNote(bombNote);
                    sprite4[SPRITE_DX_OFFSET] = sprite3[SPRITE_DX_OFFSET];
                }
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
                        if (scoreHi > highScoreHi || (scoreHi == highScoreHi && scoreLo > highScoreLo)) {
                            highScoreHi = scoreHi;
                            highScoreLo = scoreLo;
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
                if (fuel < 175) {
                    fuel += 80; // refuel after score displayed
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
            }
        }
    } // END SCRAMBLE
    
    } // END SUPER LOOP
} // main()
