.PHONY: all run clean

MBR_SRC := bootloader/mbr.asm
SECOND_SRC := bootloader/second.asm
THIRD_SRC := $(wildcard bootloader/*.c)

BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj

MBR_BIN := $(BIN_DIR)/mbr.bin
SECOND_BIN := $(BIN_DIR)/second.bin
THIRD_OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(THIRD_SRC:.c=.o)))
THIRD_ELF := $(BIN_DIR)/third.elf
THIRD_BIN := $(BIN_DIR)/third.bin

BASIC_IMG := basic.img

NASM_FLAGS := -f bin

GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
GCC_FLAGS := -m32 -ffreestanding

LD := $(HOME)/opt/cross/bin/i686-elf-ld
BOOT_LDFLAGS := -T bootloader/boot_linker.ld

all: $(BASIC_IMG)

$(MBR_BIN): $(MBR_SRC)
	nasm $(NASM_FLAGS) $(MBR_SRC) -o $(MBR_BIN)

$(SECOND_BIN): $(SECOND_SRC)
	nasm $(NASM_FLAGS) $(SECOND_SRC) -o $(SECOND_BIN)

$(OBJ_DIR)/%.o: bootloader/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@

$(THIRD_ELF): $(THIRD_OBJ)
	$(LD) $(BOOT_LDFLAGS) -o $(THIRD_ELF) $^

$(THIRD_BIN): $(THIRD_ELF)
	objcopy -O binary $(THIRD_ELF) $(THIRD_BIN)

$(BASIC_IMG): $(MBR_BIN) $(SECOND_BIN) $(THIRD_BIN)
	qemu-img create -f raw $(BASIC_IMG) 12M
	dd if=$(MBR_BIN) of=$(BASIC_IMG) bs=512 count=1 conv=notrunc
	dd if=$(SECOND_BIN) of=$(BASIC_IMG) bs=512 count=2 seek=1 conv=notrunc
	dd if=$(THIRD_BIN) of=$(BASIC_IMG) bs=512 count=2 seek=3 conv=notrunc

run: $(BASIC_IMG)
	qemu-system-x86_64 -m 64M -drive format=raw,file=$(BASIC_IMG)

debug: $(BASIC_IMG)
	qemu-system-x86_64 -m 64M -drive format=raw,file=$(BASIC_IMG) -no-reboot -no-shutdown -monitor stdio

clean:
	rm -f $(BIN_DIR)/*.bin $(OBJ_DIR)/*.o $(THIRD_ELF) $(THIRD_BIN) $(BASIC_IMG)