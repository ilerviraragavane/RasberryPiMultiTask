# ARMGNU ?= arm-none-eabi

CFLAGS = -Wall -g#-fomit-frame-pointer -mno-apcs-frame -g -march=armv6z -marm -mthumb-interwork
ASFLAGS = #-g -march=armv6z

C_FILES=elements.c mdutil.c midiread.c pmidi.c seqlib.c seqmidi.c glib.c
AS_FILES=

OBJS = $(patsubst %.s,%.o,$(AS_FILES))
OBJS += $(patsubst %.c,%.o,$(C_FILES))

DEFS = -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\" -DPACKAGE=\"pmidi\" -DVERSION=\"1.7.0\" -DHAVE_LIBASOUND=1 -DUSE_DRAIN=1 -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DSTDC_HEADERS=1 -DHAVE_UNISTD_H=1 -DHAVE_ALSA_ASOUNDLIB_H=1 -DHAVE_VPRINTF=1

.PHONY: gcc clean

gcc : pmidi

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

%.o : %.c
	gcc $(CFLAGS) -I/usr/include/glib-2.0 $(DEFS) -c $< -o $@

pmidi : $(OBJS)
	gcc $(OBJS) -lasound -o play_midi_file
