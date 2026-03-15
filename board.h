// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file board.h
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

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void board_init(void);
uint8_t board_read_buttons(void);
void board_buzzer_on(void);
void board_buzzer_off(void);

#endif

