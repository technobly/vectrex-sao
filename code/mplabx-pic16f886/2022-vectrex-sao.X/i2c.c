/* 
 * File:   i2c.c
 * Author: Brett Walach
 *
 * Created on October 3, 2022, 2:46 PM
 */

#include "i2c.h"
#include "config.h"

// Begin I2C as master
void i2c_init(const unsigned long feq_K) {
    TRISC3 = 1; //Set SDA and SCL pins as input pins
    TRISC4 = 1; 
    SSPCON  = 0b00101000;
    SSPCON2 = 0b00000000;
    SSPADD = (_XTAL_FREQ/(4 * feq_K)) - 1;
//    SSPADD = 1;
//    SSPSTAT = 0b10000000; // 400kHz
    SSPSTAT = 0b00000000; // 100kHz
}

// check the this on registers to make sure the I2C is not in progress
void i2c_wait() {
    while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100));
}

void i2c_start() {
    i2c_wait();  // Wait while I2C is busy 
    SEN = 1;     // Start I2C
}
void i2c_stop() {
    i2c_wait(); // Wait while I2C is busy 
    PEN = 1;    // Stop I2C
}

void i2c_write(unsigned data) {
    i2c_wait();
    SSPBUF = data;
}

uint8_t i2c_read(uint8_t ack) {
  uint8_t incoming;
  i2c_wait();
  RCEN = 1;

  i2c_wait();
  incoming = SSPBUF;     // get the data saved in SSPBUF

  i2c_wait();
  ACKDT = (ack)?0:1;     // check if ack bit received 
  ACKEN = 1;

  return incoming;
}
