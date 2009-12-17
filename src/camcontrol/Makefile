RM=rm
AVRDUDE=avrdude

# Adjust the Target MCU to your project
TARGET_MCU=atmega128

# avrdude settings
AVRDUDE_TARGET=m128
AVRDUDE_PROGRAMMER=stk500v2
AVRDUDE_DEV=/dev/ttyACM0

# Adjust the Application Name to your project
APP_NAME = camcontrol

# The gcc compiler and linker
CC      = avr-gcc
LINK    = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

QP_INCDIR = ../qpn/include
QP_SRCDIR = ../qpn/source
BLDDIR    = .
AVR_INCDIR = ../avr

CCINC     := -I$(QP_INCDIR) -I$(BLDDIR) #-I$(AVR_INCDIR)
QP_DEP    := $(QP_INCDIR)/qassert.h \
	$(QP_INCDIR)/qepn.h \
	$(QP_INCDIR)/qfn.h \
	$(BLDDIR)/qpn_port.h

APP_DEP   := $(BLDDIR)/bsp.h

# dbg (default target) .......................................................

BINDIR   = dbg

# gcc options
# -c    : compile only
# -g    : source level debugging
CCFLAGS  = -c -gdwarf-2 -std=gnu99 -Os -fsigned-char -fshort-enums \
 -mmcu=$(TARGET_MCU) -Wall -o$@

LINKFLAGS = -gdwarf-2 -Os -mmcu=$(TARGET_MCU)


# release ....................................................................
ifeq ($(MAKECMDGOALS), rel)

BINDIR   = rel

# gcc options
# -c    : compile only
# -O    : optimize
CCFLAGS  = -c -DNDEBUG -std=gnu99 -Os -fsigned-char -fshort-enums \
 -mmcu=$(TARGET_MCU) -Wall -o$@

LINKFLAGS =	-O2 -mmcu=$(TARGET_MCU)

endif
ifeq ($(MAKECMDGOALS), rel_clean)

BINDIR   = rel

endif

#.............................................................................

all: $(BINDIR)/$(APP_NAME).hex

$(BINDIR)/$(APP_NAME).hex : $(BINDIR)/$(APP_NAME).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(BINDIR)/$(APP_NAME).elf : \
	$(BINDIR)/bsp.o \
	$(BINDIR)/globals.o \
	$(BINDIR)/hd44780.o \
	$(BINDIR)/key.o \
	$(BINDIR)/lcd.o \
	$(BINDIR)/main.o \
	$(BINDIR)/menu.o \
	$(BINDIR)/mmi.o \
	$(BINDIR)/uart.o \
	$(BINDIR)/qepn.o \
	$(BINDIR)/qfn.o
	$(LINK) $(LINKFLAGS) \
	-o $(BINDIR)/$(APP_NAME).elf \
	-Wl,-Map,$(BINDIR)/$(APP_NAME).map,--cref \
	$(BINDIR)/bsp.o \
	$(BINDIR)/globals.o \
	$(BINDIR)/hd44780.o \
	$(BINDIR)/key.o \
	$(BINDIR)/lcd.o \
	$(BINDIR)/main.o \
	$(BINDIR)/menu.o \
	$(BINDIR)/mmi.o \
	$(BINDIR)/uart.o \
	$(BINDIR)/qepn.o \
	$(BINDIR)/qfn.o



$(BINDIR)/bsp.o: $(BLDDIR)/bsp.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/globals.o: $(BLDDIR)/globals.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/hd44780.o: $(BLDDIR)/hd44780.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/lcd.o: $(BLDDIR)/lcd.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/key.o: $(BLDDIR)/key.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/main.o: $(BLDDIR)/main.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/menu.o: $(BLDDIR)/menu.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/mmi.o: $(BLDDIR)/mmi.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/uart.o: $(BLDDIR)/uart.c $(APP_DEP) $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qepn.o: $(QP_SRCDIR)/qepn.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<

$(BINDIR)/qfn.o: $(QP_SRCDIR)/qfn.c $(QP_DEP)
	$(CC) $(CCFLAGS) $(CCINC) $<



dbg: all

rel: all


# clean targets...

.PHONY: clean rel_clean spy_clean

rel_clean: clean

dbg_clean: clean

clean:
	-$(RM) $(BINDIR)/*.o
	-$(RM) $(BINDIR)/*.elf
	-$(RM) $(BINDIR)/*.hex
	-$(RM) $(BINDIR)/*.map

	
# download

.PHONY: download

download: all
	$(AVRDUDE) -B 1 -p $(AVRDUDE_TARGET) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_DEV) -e -U flash:w:$(BINDIR)/$(APP_NAME).hex
	
