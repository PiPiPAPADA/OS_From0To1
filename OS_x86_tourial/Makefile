#!Makefile

C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc 
LD = ld 
ASM = nasm 


CFLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T tools/kernel.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
	@echo Edit code file $< ...
	$(CC) $(CFLAGS) $< -o $@

.s.o:
	@echo Edit ASM file $< ...
	$(ASM) $(ASM_FLAGS) $< 

link:
	@echo Link Kernel file 
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o hx_kernel 

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) hx_kernel

.PHONY: update_image
update_image:
	mount floppy.img floppy -o loop
	cp hx_kernel floppy
	sleep 1
	umount floppy

.PHONY: mount_image
mount_image:
	mount floppy.img floppy

.PHONY: umount_image
umount_image:
	umount floppy

.PHONY: qemu
qemu:
	qemu-system-i386 -fda floppy.img -boot a 

.PHONY: debug 
debug:
	qemu-system-i386 -s -S -fda floppy.img -boot a &
	sleep 1 
	gdb -x tools/gdbinit 