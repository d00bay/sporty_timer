// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file ssd1306.h
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
#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

void ssd1306_init(void);
void ssd1306_clear(void);

// Draws MM:SS scaled 4x (occupies pages 2-5)
void ssd1306_draw_time(uint8_t minutes, uint8_t seconds);

// NEW: Prints a string of text (small 5x7 font)
// x: 0-127 (column)
// page: 0-7 (row) -> Use 0 for top title, 6 or 7 for bottom status
void ssd1306_print(uint8_t x, uint8_t page, const char *str);

// NEW: Flashes the screen (White/Black invert)
// count: how many times to flash
void ssd1306_flash_screen(uint8_t count);

#endif
