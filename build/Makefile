VPATH = ../

# The flags passed to the C compiler.
CFLAGS= \
		-mcpu=arm920t \
		-O2 \
		-Wall \
		-Wextra \
		-Werror	\
		-Wno-format	\
		-nostdlib \
		-ffreestanding \
		-Wfloat-equal \
		-Wundef \
		-Wshadow \
		-Wpointer-arith \
		-Wcast-align \
		-Wstrict-prototypes \
		-Wwrite-strings \
		-Wconversion \
		-Wunreachable-code \
		-Wswitch-enum \
		-Wswitch-default \
		-std=c11 \
		-g

LSCRIPT = kernel.ld
OBJ = kernel.o start.o ivt.o string.o stdio.o
CROSS = arm-none-eabi-
CC = $(CROSS)gcc
LD = $(CROSS)ld
OBJCOPY =$(CROSS)objcopy

LIBGCC := $(shell $(CC) -print-libgcc-file-name)

%.o: %.S
	$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

kernel: $(LSCRIPT) $(OBJ)
	$(LD) -T $(LSCRIPT) -o $@ $(OBJ) $(LIBGCC)

kernel.bin: kernel
	$(OBJCOPY) -Obinary --set-section-flags \
		.bss=contents,alloc,load,data $< $@

kernel.img: kernel.bin
	mkimage -A arm -T standalone -C none -a 0x20000000 -d $< $@

.PHONY: all
all: kernel

.PHONY: clean
clean:
	rm -f kernel kernel.bin kernel.img rm -f $(OBJ)
	rm -f *.d

# these targets assume you symlinked our custom qemu binary as ./qemu
.PHONY: run
run: kernel
	./qemu -M portux920t -m 64M -nographic -kernel kernel

.PHONY: debug
debug: kernel
	./qemu -M portux920t -m 64M -nographic -kernel kernel -S -gdb tcp::2000

.PHONY: gdb
gdb: kernel
	$(CROSS)gdb -ex 'target remote :2000' kernel
