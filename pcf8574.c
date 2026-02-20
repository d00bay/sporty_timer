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

void pcf8574_init(uint8_t addr_7bit)
{
  pcf8574_write(addr_7bit, 0xff);
}

void pcf8574_write(uint8_t addr_7bit, uint8_t value)
{
  i2c_start();
  i2c_write((addr_7bit << 1) | 0);
  i2c_write(value);
  i2c_stop();
}

uint8_t pcf8574_read(uint8_t addr_7bit)
{
  uint8_t v;

  i2c_start();
  i2c_write((addr_7bit << 1) | 1);
  v = i2c_read(0);
  i2c_stop();

  return v;
}

