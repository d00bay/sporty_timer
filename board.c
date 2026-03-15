// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file board.c 
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
#include "board.h"
#include "pcf8574.h"
// temporary for debug, remove later
#include "ssd1306.h"
#include <stdio.h>
#include <avr/io.h>


#define PCF8574_ADDR    0x20
#define PCF_BTN_MASK    0x0E /* P0 to P2 */
#define PCF_BUZZER_BIT  0

static pcf8574_t pcf;

void board_init(void) 
{
  pcf8574_init(&pcf, PCF8574_ADDR, 0xff);
  (void)pcf8574_set_bit(&pcf, PCF_BUZZER_BIT, false);
}

uint8_t board_read_buttons(void) 
{
  uint8_t value;

  pcf.out |= PCF_BTN_MASK;
  pcf.out |= (1 << PCF_BUZZER_BIT);
  (void)pcf8574_write(&pcf);

  if (!pcf8574_read(&pcf, &value)) {
    return 0;
  }

  return (uint8_t)(~value) & PCF_BTN_MASK;
}

void board_buzzer_on(void) 
{
  //  (void)pcf8574_set_bit(&pcf, 0, true);  
  pcf.out = 0xff;
  pcf8574_write(&pcf);
}

void board_buzzer_off(void) 
{
//  (void)pcf8574_set_bit(&pcf, 0, false);  
  pcf.out = 0xfe;
  pcf8574_write(&pcf);
}

