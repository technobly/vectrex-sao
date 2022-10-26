/* 
 * File:   i2c.h
 * Author: Brett Walach
 *
 * Created on October 3, 2022, 2:46 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

void i2c_init(const unsigned long feq_K);
void i2c_wait();
void i2c_start();
void i2c_stop();
void i2c_write(unsigned data);
uint8_t i2c_read(uint8_t ack);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

