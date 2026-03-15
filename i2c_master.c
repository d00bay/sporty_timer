// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file i2c_master.c
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
#include <stdbool.h>

#include "i2c_master.h"

static void sda_low(void) {
  I2C_PORT &= ~(1 << I2C_SDA);
  I2C_DDR |= (1 << I2C_SDA);
}

static void sda_release(void) {
  I2C_DDR &= ~(1 << I2C_SDA);
}

static void scl_low(void) {
  I2C_PORT &= ~(1 << I2C_SCL);
  I2C_DDR |= (1 << I2C_SCL);
}

static void scl_release(void) {
  I2C_DDR &= ~(1 << I2C_SCL);
}

void i2c_init(void) {
  // Optional: Set default states, though often done in start()
  I2C_PORT |= (1<<I2C_SDA) | (1<<I2C_SCL);
  I2C_DDR &= ~((1 << I2C_SDA) | (1 << I2C_SCL));
}

void i2c_start(void) {
  sda_release();
  scl_release();
  _delay_us(4);

  sda_low();
  _delay_us(4);

  scl_low();
  _delay_us(4);
}

void i2c_stop(void) {
  sda_low();
  _delay_us(4);

  scl_release();
  _delay_us(4);

  sda_release();
  _delay_us(4);
}

bool i2c_write(uint8_t data) {
  I2C_DDR |= (1 << I2C_SDA);

  for(uint8_t i=0; i<8; i++) {
    if(data & 0x80) 
      I2C_PORT |= (1<<I2C_SDA);
    else 
      I2C_PORT &= ~(1<<I2C_SDA);
    I2C_PORT |= (1<<I2C_SCL);
    _delay_us(4);
    I2C_PORT &= ~(1<<I2C_SCL);
    data <<= 1;
  }
  // Handle ACK/NACK pulse (we ignore the actual bit for the OLED)
  I2C_DDR &= ~(1<<I2C_SDA);
  I2C_PORT |= (1<<I2C_SDA);
  I2C_PORT |= (1<<I2C_SCL);
  _delay_us(2);

  bool ack = !(I2C_PIN & (1 << I2C_SDA)); // Ack == SDA low

  I2C_PORT &= ~(1<<I2C_SCL);
  return ack;
}

uint8_t i2c_read(uint8_t ack) {
  uint8_t data = 0;
  I2C_DDR &= ~(1<<I2C_SDA); // Set SDA as input
  
  for(uint8_t i=0; i<8; i++) {
    I2C_PORT |= (1<<I2C_SCL);
    _delay_us(4);
    data <<= 1;
    if(PINB & (1<<I2C_SDA)) data |= 1;
    I2C_PORT &= ~(1<<I2C_SCL);
    _delay_us(4);
  }
  
  // Send ACK or NACK
  I2C_DDR |= (1<<I2C_SDA);
  if(ack) I2C_PORT &= ~(1<<I2C_SDA); // ACK
  else I2C_PORT |= (1<<I2C_SDA);     // NACK
  
  I2C_PORT |= (1<<I2C_SCL);
  _delay_us(4);
  I2C_PORT &= ~(1<<I2C_SCL);
  
  return data;
}
