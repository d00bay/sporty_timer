// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file buttons.c 
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
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

/*
 * Button bitmask layout:
 * bit0 = BTN1
 * bit1 = BTN2
 * bit2 = BTN3
 */

#define BTN1_MASK (1 << 0)
#define BTN2_MASK (1 << 1)
#define BTN3_MASK (1 << 2)

void buttons_init(void);
uint8_t buttons_read(void);

#endif

