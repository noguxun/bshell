PREFIX = arm-linux-gnueabi
AS = $(PREFIX)-as
CC = $(PREFIX)-gcc
LD = $(PREFIX)-ld
OBJCOPY = $(PREFIX)-objcopy
FLAGS = -mcpu=arm926ej-s

all:
	$(AS) $(FLAGS) -g startup.s -o startup.o
	$(CC) $(FLAGS) -c -g bshell.c -o bshell.o
	$(CC) $(FLAGS) -c -g console.c -o console.o
	$(CC) $(FLAGS) -c -g cmd.c -o cmd.o
	$(LD)  -Map zshell.map -T bshell.ld cmd.o console.o bshell.o startup.o -o bshell.elf
	$(OBJCOPY) -O binary bshell.elf bshell.bin

clean:
	rm *.o
	rm *.elf
	rm *.bin
	rm *.map


