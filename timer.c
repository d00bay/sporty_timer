// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file timer.c
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
#include "timer.h"

void timer_setup(timer_t *t, uint16_t active, uint16_t rest, 
              uint8_t rounds, uint8_t round_mode, display_callback_t cb) {
  t->active_duration = active;
  t->rest_duration = rest;
  t->total_rounds = rounds;
  t->is_round_mode = round_mode;
  t->display_fn = cb;
  
  // Reset state
  t->state = TIMER_STOPPED;
  t->phase = PHASE_ACTIVE;
  t->current_round = 1;
  t->time_remaining = active;
  
  // Update screen immediately
  if (t->display_fn) {
      t->display_fn(t->time_remaining / 60, t->time_remaining % 60);
  }
}

void timer_start(timer_t *t) {
  if (t->state == TIMER_FINISHED) {
    // Reset if we try to start a finished timer
    t->state = TIMER_RUNNING;
    t->phase = PHASE_ACTIVE;
    t->current_round = 1;
    t->time_remaining = t->active_duration;
  } else {
    t->state = TIMER_RUNNING;
  }
}

void timer_pause(timer_t *t) {
  if (t->state == TIMER_RUNNING) {
    t->state = TIMER_PAUSED;
  }
}

void timer_tick(timer_t *t) {
  if (t->state != TIMER_RUNNING) 
    return;
  
  if (t->time_remaining > 0) {
    t->time_remaining--;
  } else {
    // Time hit 0
    if (t->is_round_mode) {
      // Round Logic
      if (t->phase == PHASE_ACTIVE) {
        // Work done, switch to rest
        t->phase = PHASE_REST;
        t->time_remaining = t->rest_duration;
        // Optional: You could trigger a buzzer here
      } else {
         // Rest done, switch to next round
        t->current_round++;
        if (t->current_round > t->total_rounds) {
          t->state = TIMER_FINISHED;
                  // Timer done
        } else {
            t->phase = PHASE_ACTIVE;
            t->time_remaining = t->active_duration;
        }
      }
    } else {
      // Simple Countdown Logic
      t->state = TIMER_FINISHED;
    }
  }

  // Update Display
  if (t->display_fn) {
      t->display_fn(t->time_remaining / 60, t->time_remaining % 60);
  }
}
