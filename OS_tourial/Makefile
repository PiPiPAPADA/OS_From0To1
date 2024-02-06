C_SOURCES = $(wildcard kernel/*.c drives/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drives/*.h cpu/*.h libc/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}
# -g: Use debugging symbols in gcc
CFLAGS = -g  -m32 -fno-pic  -ffreestanding   -Wall -Wextra -fno-exceptions


all: os-image.bin
#build kernel binary $^ substituted all dependency file
kernel.bin : boot/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf : boot/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

os-image.bin : boot/bootsect.bin kernel.bin 
	cat $^ > os-image.bin


run: all
	qemu-system-i386 -drive format=raw,file=os-image.bin

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	gdb -ex "target remote :1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	gcc ${CFLAGS}  -c $< -o $@

%.o: %.asm
	nasm $< -f elf -I boot -o $@

%.bin: %.asm
	nasm $< -f bin -I boot -o $@

clean:	
	rm -rf *.bin *.o *.elf
	rm -rf boot/*.o boot/*.bin  kernel/*.o drives/*.o libc/*.o

