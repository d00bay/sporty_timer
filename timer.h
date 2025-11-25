// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file timer.h
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
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

// Function pointer type for updating display
typedef void (*display_callback_t)(uint8_t minutes, uint8_t seconds);

typedef enum {
  TIMER_STOPPED,
  TIMER_RUNNING,
  TIMER_PAUSED,
  TIMER_FINISHED
} timer_state_t;

typedef enum {
  PHASE_ACTIVE, 
  PHASE_REST
} timer_phase_t;

typedef struct {
  uint16_t active_duration;  // In seconds
  uint16_t rest_duration;    // In seconds
  uint8_t total_rounds;
  uint8_t current_round;
  uint16_t time_remaining;   // In seconds
  timer_phase_t phase;
  timer_state_t state;
  uint8_t is_round_mode;     // 1 = Round Mode, 0 = Simple Countdown

  display_callback_t display_fn; 
} timer_t;

// Initialize the timer with a specific preset
void timer_setup(timer_t *t, uint16_t active, uint16_t rest, uint8_t rounds, uint8_t round_mode, display_callback_t cb);

// Start/Resume
void timer_start(timer_t *t);

// Pause
void timer_pause(timer_t *t);

// Call this every 1 second
void timer_tick(timer_t *t);

#endif
