// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file pcf8574.h
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
#ifndef PCF8574_H_
#define PCF8574_H_

#include <stdint.h>

void pcf8574_init(uint8_t addr_7bit);
uint8_t pcf8574_read(uint8_t addr_7bit);
void pcf8574_write(uint8_t addr_7bit, uint8_t value);

#endif
