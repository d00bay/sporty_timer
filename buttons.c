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
#include "buttons.h"
#include "pcf8574.h"
#include <avr/io.h>

/*
 * Configuration
 */

#define USE_PCF8574     0
#define PCF8574_ADDR    0x20
#define PCF_BTN_MASK    0x07 // P0..P2
#define PCF_BUZZER_BIT  3    // P3

static pcf8574_t pcf;

/*
 * GPIO implementation
 */

static void buttons_gpio_init(void)
{
  DDRB &= ~(1 << PB1);
  DDRB &= ~(1 << PB3);
  DDRB &= ~(1 << PB4);

  PORTB |= (1 << PB1);
  PORTB |= (1 << PB3);
  PORTB |= (1 << PB4);
}

static uint8_t buttons_gpio_read(void)
{
  uint8_t buttons;

  buttons = 0;

  if (!(PINB & (1 << PB1)))
    buttons |= BTN1_MASK;

  if (!(PINB & (1 << PB3)))
    buttons |= BTN2_MASK;

  if (!(PINB & (1 << PB4)))
    buttons |= BTN3_MASK;

  return buttons;
}

/*
 * PCF8574 implementation
 */

static void buttons_pcf_init(void)
{
  pcf8574_init(&pcf, PCF8574_ADDR, 0xff);
}

static uint8_t buttons_pcf_read(void)
{
  uint8_t value;

  if (!pcf8574_read(&pcf, &value)) {
    return 0;
  }

  return (uint8_t)(~value) & PCF_BTN_MASK; 
}

/*
 * Public API
 */

void buttons_init(void)
{
#if USE_PCF8574
  buttons_pcf_init();
#else
  buttons_gpio_init();
#endif
}

uint8_t buttons_read(void)
{
#if USE_PCF8574
  return buttons_pcf_read();
#else
  return buttons_gpio_read();
#endif
}

