.PHONY: all run clean

all: basic.img

mbr.bin: mbr.asm
	nasm -f bin mbr.asm -o mbr.bin

second.bin: second.asm
	nasm -f bin second.asm -o second.bin

basic.img: mbr.bin second.bin
	qemu-img create -f raw basic.img 12M
	dd if=mbr.bin of=basic.img bs=512 count=1 conv=notrunc
	dd if=second.bin of=basic.img bs=512 count=7 seek=1 conv=notrunc

run: basic.img
	qemu-system-x86_64 -m 64M -drive format=raw,file=basic.img

debug: basic.img
	qemu-system-x86_64 -m 64M -drive format=raw,file=basic.img -d int

clean:
	rm -f *.bin basic.img