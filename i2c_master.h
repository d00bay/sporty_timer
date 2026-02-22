// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file i2c_master.h
 * @brief ATtiny85 Workout Timer
 * @author Matt Dube https://github.com/d00bay
 *
 * Hardware: ATtiny85 (8MHz Internal), SSD1306 OLED (I2C)
 *
 * Style Guidelines:
 *  - Identation: 2 spaces
 *  - Naming: snake_case
 *  - Structure: One statement per line
 */
    
#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <avr/io.h>
#include <util/delay.h>

// --- CONFIGURATION ---
#define I2C_SDA PB0
#define I2C_SCL PB2
#define I2C_PORT PORTB
#define I2C_DDR  DDRB

// --- PROTOTYPES ---
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
bool i2c_write(uint8_t data);
// maybe needed this for sensors later:
uint8_t i2c_read(uint8_t ack); 

#endif
