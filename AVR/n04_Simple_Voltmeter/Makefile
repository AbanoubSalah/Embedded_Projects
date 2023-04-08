#---------------------------------------------------------------------------
# Make file for bulding AVR
#
# Use: make [TARGET]
#
# Build Targets:
#	   build: Build the whole project
#			  Define DEBUG=1 for extra debuging flags
#	   clean: Removes all make generated files
#
#---------------------------------------------------------------------------

BUILDDIR = build/
TARGET = SVoltmeter
SOURCES = src/main.c src/lcd.c src/simplevmeter.c
INCLUDES = -Iinclude

CC = avr-gcc
LD = avr-ld
SIZE = avr-size
OBJDUMP = avr-objdump
OBJCOPY = avr-objcopy
MCU = atmega169p
OBJS := $(SOURCES:.c=.o)
CFLAGS = -O3 $(INCLUDES) -std=c99 -mmcu=$(MCU)
LDFLAGS += -Xlinker -Map=$(addprefix $(BUILDDIR),$(TARGET).map)

ifeq ($(DEBUG), 1)
CFLAGS += -gdwarf-2 -Wa,-adhlns=$(addprefix $(BUILDDIR),$(TARGET).lst)
endif

.PHONY: build clean
build : $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(SOURCES) -o $(addprefix $(BUILDDIR),$(TARGET).elf)
	$(OBJCOPY) -R .fuse -O ihex $(addprefix $(BUILDDIR),$(TARGET).elf) $(addprefix $(BUILDDIR),$(TARGET).hex)
	$(SIZE) -Btd $(addprefix $(BUILDDIR),$(TARGET).elf) $(addprefix $(BUILDDIR),$(OBJS))

%.o : %.c
	@mkdir -p $(dir $(addprefix $(BUILDDIR),$@))
	$(CC) -c $< $(CFLAGS) -o $(addprefix $(BUILDDIR),$@)

clean:
	@rm -rf $(BUILDDIR)
