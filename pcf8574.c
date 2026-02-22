// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file pcf8574.c
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
#include "pcf8574.h"
#include "i2c_master.h"

void pcf8574_init(pcf8574_t *d, uint8_t addr_7bit, uint8_t initial_out){
  d->addr_7bit = addr_7bit;
  d->out = initial_out;
  (void)pcf8574_write(d);
}

bool pcf8574_write(pcf8574_t *d){
  i2c_start();
  if (!i2c_write(((uint8_t)((d->addr_7bit << 1) | 0)))) {
    i2c_stop();
    return false;
  }
  if(!i2c_write(d->out)) {
    i2c_stop();
    return false;
  }
  i2c_stop();
}

bool pcf8574_read(pcf8574_t *d, uint8_t *value) {
  i2c_start();
  if (!i2c_write((d->addr_7bit << 1) | 1)){
    i2c_stop;
    return false;
  }
  *value = i2c_read(0);
  i2c_stop();
  return true;
}

bool pcf8574_set_bit(pcf8574_t *d, uint8_t bit, bool level) {
  if (bit > 7) {
    return false;
  }

  if (level) {
    d->out |= (1u << bit);
  }
  else {
    d->out &= ~(1u << bit);
  }

  return pcf8574_write(d);
}

