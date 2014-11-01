
default: all

h: help
help:
	@echo "Targets:"
	@echo "  all   - build .elf and .hex (default target)"
	@echo "  bpr   - build, program, and run"
	@echo "  s     - stop (actually loads and runs the blink program)"
	@echo "  r     - run (same as bpr)
	@echo "  reboot- reboot teensy
	@echo "  tags  - make tags file"
	@echo "  clean - remove generated files"
	@echo "  help  - show this help"

# The name of your project (used to name the compiled .hex file)
TARGET = dragon

# Path to your arduino installation
#ARDUINOPATH ?= ../../../..
#ARDUINOPATH ?= ../../../Software/arduino/arduino-1.0.6
ARDUINOPATH ?= arduino

# configurable options
OPTIONS = -DF_CPU=72000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH

# options needed by many Arduino libraries to configure for Teensy 3.0
#OPTIONS += -D__MK20DX128__ -DARDUIO=105 -DTEENSYDUINO=118
OPTIONS += -D__MK20DX256__ -DARDUIO=106 -DTEENSYDUINO=120


# Other Makefiles and project templates for Teensy 3.x:
#
# https://github.com/apmorton/teensy-template
# https://github.com/xxxajk/Arduino_Makefile_master
# https://github.com/JonHylands/uCee


#************************************************************************
# Location of Teensyduino utilities, Toolchain, and Arduino Libraries.
# To use this makefile without Arduino, copy the resources from these
# locations and edit the pathnames.  The rest of Arduino is not needed.
#************************************************************************

# path location for Teensy Loader, teensy_post_compile and teensy_reboot
TOOLSPATH = $(ARDUINOPATH)/hardware/tools   # on Linux
#TOOLSPATH = $(ARDUINOPATH)/hardware/tools/tools/avr/bin   # on Mac or Windows

# path location for Arduino libraries (currently not used)
LIBRARYPATH = $(ARDUINOPATH)/libraries

# path location for the arm-none-eabi compiler
COMPILERPATH = $(ARDUINOPATH)/hardware/tools/arm-none-eabi/bin

TEENSY3 = $(ARDUINOPATH)/hardware/teensy/cores/teensy3

#************************************************************************
# Settings below this point usually do not need to be edited
#************************************************************************

INCLUDES :=
INCLUDES += -I.
INCLUDES += -Iarduino/hardware/teensy/cores/teensy3

# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -g -Os -mcpu=cortex-m4 -mthumb -nostdlib -MMD $(OPTIONS) $(INCLUDES) -Werror

# compiler options for C++ only
CXXFLAGS = -std=gnu++0x -felide-constructors -fno-exceptions -fno-rtti

# compiler options for C only
CFLAGS =

LINKSCRIPT := $(ARDUINOPATH)/hardware/teensy/cores/teensy3/mk20dx256.ld

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -T$(LINKSCRIPT)

# additional libraries to link
LIBS = -lm


# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size
AR = $(abspath $(COMPILERPATH))/arm-none-eabi-ar

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
#C_FILES := $(wildcard *.c)
#CPP_FILES := $(wildcard *.cpp)

C_FILES :=
CPP_FILES :=
#CPP_FILES += dragon_main.cpp

C_FILES += $(TEENSY3)/usb_keyboard.c
C_FILES += $(TEENSY3)/mk20dx128.c
C_FILES += $(TEENSY3)/math_helper.c
C_FILES += $(TEENSY3)/usb_mouse.c
C_FILES += $(TEENSY3)/nonstd.c
C_FILES += $(TEENSY3)/usb_joystick.c
C_FILES += $(TEENSY3)/keylayouts.c
C_FILES += $(TEENSY3)/serial3.c
C_FILES += $(TEENSY3)/pins_teensy.c
C_FILES += $(TEENSY3)/usb_desc.c
C_FILES += $(TEENSY3)/usb_rawhid.c
C_FILES += $(TEENSY3)/usb_mem.c
C_FILES += $(TEENSY3)/serial1.c
C_FILES += $(TEENSY3)/touch.c
C_FILES += $(TEENSY3)/usb_midi.c
C_FILES += $(TEENSY3)/eeprom.c
C_FILES += $(TEENSY3)/usb_seremu.c
C_FILES += $(TEENSY3)/serial2.c
C_FILES += $(TEENSY3)/usb_serial.c
C_FILES += $(TEENSY3)/usb_dev.c
C_FILES += $(TEENSY3)/analog.c

#CPP_FILES += $(TEENSY3)/Tone.cpp
#CPP_FILES += $(TEENSY3)/AudioStream.cpp
CPP_FILES += $(TEENSY3)/HardwareSerial2.cpp
CPP_FILES += $(TEENSY3)/HardwareSerial1.cpp
#CPP_FILES += $(TEENSY3)/new.cpp
#CPP_FILES += $(TEENSY3)/DMAChannel.cpp
CPP_FILES += $(TEENSY3)/WMath.cpp
CPP_FILES += $(TEENSY3)/HardwareSerial3.cpp
CPP_FILES += $(TEENSY3)/usb_inst.cpp
#CPP_FILES += $(TEENSY3)/IntervalTimer.cpp
CPP_FILES += $(TEENSY3)/yield.cpp
#CPP_FILES += $(TEENSY3)/Print.cpp
#CPP_FILES += $(TEENSY3)/usb_flightsim.cpp
#CPP_FILES += $(TEENSY3)/Stream.cpp
#CPP_FILES += $(TEENSY3)/IPAddress.cpp
#CPP_FILES += $(TEENSY3)/avr_emulation.cpp
#CPP_FILES += $(TEENSY3)/WString.cpp

# Use my own main() function
#CPP_FILES += $(TEENSY3)/main.cpp

# Arduino libraries
CPP_FILES += $(LIBRARYPATH)/Servo/Servo.cpp
CPP_FILES += $(LIBRARYPATH)/Bounce/Bounce.cpp
INCLUDES += -Iarduino/libraries/Servo
INCLUDES += -Iarduino/libraries/Bounce

# My files (not including dragon_main.cpp)
CPP_FILES += ac_alive.cpp
CPP_FILES += ac_servo.cpp
CPP_FILES += ac_timer.cpp
CPP_FILES += ac_counter.cpp
CPP_FILES += ac_printf.cpp
CPP_FILES += ac_led_rgb.cpp
CPP_FILES += dragon_buttons.cpp
CPP_FILES += dragon_leds.cpp
CPP_FILES += dragon_emotions.cpp
CPP_FILES += dragon.cpp

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)


# the actual makefile rules (all .o files built by GNU make's default implicit rules)

all: $(TARGET).hex blink.hex

lib$(TARGET).a: $(OBJS)
	rm -f $@
	$(AR) rcs $@ $^
  

$(TARGET).elf: lib$(TARGET).a  $(LINKSCRIPT) dragon_main.o
	$(CC) $(LDFLAGS) -o $@ dragon_main.o lib$(TARGET).a -larm_cortexM4l_math -lm

blink.elf: lib$(TARGET).a  $(LINKSCRIPT) blink_main.o
	$(CC) $(LDFLAGS) -o $@ blink_main.o lib$(TARGET).a -larm_cortexM4l_math -lm

###%.hex: %.elf
###	$(SIZE) $<
###	$(OBJCOPY) -O ihex -R .eeprom $< $@
###	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(basename $@) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
###	-$(abspath $(TOOLSPATH))/teensy_reboot

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $(@:.elf=.eep)
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo ""
	@echo "Build succeeded.  Type 'make bpr' to program and run."

.PHONY: bpr
bpr: $(TARGET).hex
	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(basename $<) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
	-$(abspath $(TOOLSPATH))/teensy_reboot

.PHONY: run r
run:
r: bpr

.PHONY: reboot
reboot:
	-$(abspath $(TOOLSPATH))/teensy_reboot

.PHONY: stop s
stop: s
s: blink.hex
	$(abspath $(TOOLSPATH))/teensy_post_compile -file=$(basename $<) -path=$(shell pwd) -tools=$(abspath $(TOOLSPATH))
	-$(abspath $(TOOLSPATH))/teensy_reboot

.PHONY: tags
tags:
	rm tags
	ctags -R `pwd`

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	rm -f *.o *.d $(TEENSY3)/*.o $(TEENSY3)/*.d $(TARGET).elf $(TARGET).hex
	-find $(LIBRARYPATH) -name '*.o' -exec rm -f {} \;
	-find $(LIBRARYPATH) -name '*.d' -exec rm -f {} \;


