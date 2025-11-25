# --- CONFIGURATION ---
MCU          = attiny85
F_CPU        = 8000000UL  # 8 MHz (Make sure your fuses are set for this!)
PROGRAMMER   = usbtiny    # Options: usbtiny, avrispmkII, arduino, etc.
BAUD         = 19200      # Only needed for some programmers (like Arduino as ISP)

# --- OUTPUT SETTINGS ---
TARGET       = main
OBJDIR       = build

# --- SOURCE FILES ---
# Automatically find all .c files in the current directory
SRCS         = $(wildcard *.c)
OBJS         = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

# --- TOOLCHAIN ---
CC           = avr-gcc
OBJCOPY      = avr-objcopy
SIZE         = avr-size
AVRDUDE      = avrdude

# --- FLAGS ---
# -Os: Optimize for size (Crucial for ATtiny85)
# -Wall: Enable all warnings
CFLAGS       = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
LDFLAGS      = -mmcu=$(MCU)

# --- BUILD RULES ---

# Default target: build the hex file
all: $(OBJDIR) $(TARGET).hex size

# Create build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile .c to .o
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to .elf
$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

# Convert .elf to .hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flash the chip
flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U flash:w:$(TARGET).hex:i

# Check memory usage
size: $(TARGET).elf
	@echo
	@$(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(TARGET).elf $(TARGET).hex

.PHONY: all flash clean size
