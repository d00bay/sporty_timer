# Project: ATtiny85 Timer
**Hardware:** ATtiny85, SSD1306 OLED (I2C), 3 Buttons.
**Current State:**
- Bit-banged I2C library (`i2c_master.c`) is separated.
- OLED driver (`ssd1306.c`) supports text and large numbers.
- Timer logic (`timer.c`) handles Countdown and Tabata modes.
- Makefile is set up for `avr-gcc` and `usbtiny`.

# Coding Style / Preferences (STRICT)
1. **Naming:** snake_case ONLY. No camelCase.
2. **Indentation:** 2 spaces. No tabs.
3. **Formatting:** ONE statement per line. No `cmd(); cmd();`.
4. **Language:** C (AVR-GCC).

# Next Tasks
- Refactor `main.c` to remove blocking `_delay_ms` logic.
- Implement **Timer0 Interrupts (ISR)** for accurate 10ms timing.
- Use the "Mailbox/Flag" pattern to ensure thread-safety with I2C.
