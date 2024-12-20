.PHONY: all run clean

GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
GCCFLAGS := -m32 -ffreestanding

LD := $(HOME)/opt/cross/bin/i686-elf-ld
LDFLAGS := -T linker.ld

all: basic.img

mbr.bin: mbr.asm
	nasm -f bin mbr.asm -o mbr.bin

second.bin: second.asm
	nasm -f bin second.asm -o second.bin

third.bin: third.c
	$(GCC) $(GCCFLAGS) -c third.c -o third.o
	$(LD) $(LDFLAGS) -o third.elf third.o
	objcopy -O binary third.elf third.bin

basic.img: mbr.bin second.bin third.bin
	qemu-img create -f raw basic.img 12M
	dd if=mbr.bin of=basic.img bs=512 count=1 conv=notrunc
	dd if=second.bin of=basic.img bs=512 count=2 seek=1 conv=notrunc
	dd if=third.bin of=basic.img bs=512 count=1 seek=3 conv=notrunc

run: basic.img
	qemu-system-x86_64 -m 64M -drive format=raw,file=basic.img

debug: basic.img
	qemu-system-x86_64 -m 64M -drive format=raw,file=basic.img -monitor stdio

clean:
	rm -f *.bin basic.img