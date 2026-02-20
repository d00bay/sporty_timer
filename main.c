// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file main.c
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
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>:
#include "ssd1306.h"
#include "timer.h"

// --- PRESET DEFINITIONS (Easily Redefined) ---

// Preset 1: Simple Countdown
#define P1_WORK_SEC    300  // 5 Minutes
#define P1_REST_SEC    0    // Not used
#define P1_ROUNDS      0    // Not used
#define P1_MODE        0    // 0 = Simple, 1 = Round/Tabata

// Preset 2: Tabata (2m Work, 30s Rest)
#define P2_WORK_SEC    120  // 2 Minutes
#define P2_REST_SEC    30   // 30 Seconds
#define P2_ROUNDS      8    // Standard Tabata sets (adjust as needed)
#define P2_MODE        1

// Preset 3: Tabata (3m Work, 1m Rest)
#define P3_WORK_SEC    180  // 3 Minutes
#define P3_REST_SEC    60   // 1 Minute
#define P3_ROUNDS      5    // Adjust rounds as needed
#define P3_MODE        1

// --- HARDWARE CONFIG ---
// PB1, PB3, PB4 are the only safe pins left after I2C
#define BTN_1_PIN PB1 
#define BTN_2_PIN PB3
#define BTN_3_PIN PB4
#define BTN_PORT PINB

// Helper enum to track which preset is loaded
typedef enum {
  PRESET_NONE,
  PRESET_1,
  PRESET_2,
  PRESET_3
} current_preset_t;

timer_t my_timer;
current_preset_t active_preset = PRESET_NONE;

// Debounce helper
uint8_t is_btn_pressed(uint8_t pin) {
  if (!(BTN_PORT & (1 << pin))) {
    _delay_ms(50); 
    if (!(BTN_PORT & (1 << pin))) {
      return 1;
    }
  }
  return 0;
}

void update_oled(uint8_t m, uint8_t s) {
  char status_str[128] = {0};

  ssd1306_draw_time(m, s);

  // Status Bar Logic
  if (my_timer.state == TIMER_FINISHED) {
    ssd1306_print(50, 0, "DONE! ");
  } 
  else if (my_timer.state == TIMER_PAUSED) {
    ssd1306_print(50, 0, "PAUSE ");
  }
  else if (my_timer.is_round_mode) {
    if (my_timer.phase == PHASE_ACTIVE) {
      ssd1306_print(50, 0, "WORK ");
    } else {
      ssd1306_print(50, 0, "REST ");
    }
    sprintf(status_str, "%d/%d", my_timer.current_round, my_timer.total_rounds);
    ssd1306_print(54, 7, status_str);
  } 
  else {
    ssd1306_print(20, 0, "TIMER ");
  }
}

// Logic to load OR toggle a preset
void handle_preset_btn(current_preset_t preset_id, uint16_t work, 
                          uint16_t rest, uint8_t rounds, uint8_t mode) {
  // If we are already in this preset, treat button as Pause/Resume
  if (active_preset == preset_id) {
    if (my_timer.state == TIMER_RUNNING) {
      timer_pause(&my_timer);
    } else {
      timer_start(&my_timer);
    }
  } 
  else {
    // New preset selected: Load it and Start immediately
    ssd1306_clear();
    active_preset = preset_id;
    timer_setup(&my_timer, work, rest, rounds, mode, update_oled);
    timer_start(&my_timer);
  }

  // Wait for button release to prevent double-trigger
  // (We need to check which pin corresponds to the preset_id logic, 
  //  but for simplicity in this helper, we'll handle the wait in the main loop)
}

int main(void) {
  // Setup Buttons (Inputs with Pullups)
  DDRB &= ~((1 << BTN_1_PIN) | (1 << BTN_2_PIN) | (1 << BTN_3_PIN));
  PORTB |= (1 << BTN_1_PIN) | (1 << BTN_2_PIN) | (1 << BTN_3_PIN);
  
  _delay_ms(100);
  ssd1306_init();
  
  // Start with a blank screen or specific text
  ssd1306_print(30, 0, "SELECT");
  ssd1306_print(40, 3, "MODE");
  
  uint16_t tick_counter = 0;
  
  while (1) {
    // --- BUTTON 1: Preset 1 ---
    if (is_btn_pressed(BTN_1_PIN)) {
      handle_preset_btn(PRESET_1, P1_WORK_SEC, P1_REST_SEC, P1_ROUNDS, 
                                                              P1_MODE);
      while (is_btn_pressed(BTN_1_PIN)) {} // Wait for release
    }
    
    // --- BUTTON 2: Preset 2 ---
    if (is_btn_pressed(BTN_2_PIN)) {
      handle_preset_btn(PRESET_2, P2_WORK_SEC, P2_REST_SEC, P2_ROUNDS, 
                                                              P2_MODE);
      while (is_btn_pressed(BTN_2_PIN)) {} 
    }
    
    // --- BUTTON 3: Preset 3 ---
    if (is_btn_pressed(BTN_3_PIN)) {
      handle_preset_btn(PRESET_3, P3_WORK_SEC, P3_REST_SEC, P3_ROUNDS, 
                                                              P3_MODE);
      while (is_btn_pressed(BTN_3_PIN)) {}
    }

    // --- TIMING LOOP ---
    _delay_ms(10);
    tick_counter++;
    
    if (tick_counter >= 100) {
      tick_counter = 0;
      
      timer_state_t prev_state = my_timer.state;
      timer_tick(&my_timer); 
      
      if (prev_state == TIMER_RUNNING && my_timer.state == TIMER_FINISHED) {
        ssd1306_flash_screen(5);
        active_preset = PRESET_NONE; // Reset logic so next press reloads
      }
    }
  }
  return 0;
}

