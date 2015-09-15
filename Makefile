PREFIX = arm-linux-gnueabi
AS = $(PREFIX)-as
CC = $(PREFIX)-gcc
LD = $(PREFIX)-ld
OBJCOPY = $(PREFIX)-objcopy
FLAGS = -mcpu=arm926ej-s

RAM_START ?= 0x00010000

all:
	$(AS) $(FLAGS) -g startup.s -o startup.o
	$(CC) $(FLAGS) -c -g bshell.c -o bshell.o
	$(CC) $(FLAGS) -c -g console.c -o console.o
	$(CC) $(FLAGS) -c -g cmd.c -o cmd.o
	$(LD)  -Map bshell.map -T bshell.ld cmd.o console.o bshell.o startup.o --defsym SYM_RAM_START=$(RAM_START) -o bshell.elf
	$(OBJCOPY) -O binary bshell.elf bshell.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin
	rm *.map


