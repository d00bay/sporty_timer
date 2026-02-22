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

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t addr_7bit;
  uint8_t out;
} pcf8574_t;

void pcf8574_init(pcf8574_t *d, uint8_t addr_7bit, uint8_t initial_out);
bool pcf8574_write(pcf8574_t *d);
bool pcf8574_read(pcf8574_t *d, uint8_t *value);
bool pcf8574_set_bit(pcf8574_t *d, uint8_t bit, bool level);

#endif
