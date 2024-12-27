.PHONY: all run clean

MBR_SRC := bootloader/mbr.asm
SECOND_SRC := bootloader/second.asm
KERNEL_SRC := $(shell find kernel -type f -name '*.c')
KERNEL_ASM_SRC := $(shell find kernel -type f -name '*.asm')

BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj

MBR_BIN := $(BIN_DIR)/mbr.bin
SECOND_BIN := $(BIN_DIR)/second.bin
KERNEL_OBJ := $(patsubst kernel/%.c,$(OBJ_DIR)/%.o,$(KERNEL_SRC))
KERNEL_ASM_OBJ := $(patsubst kernel/%.asm,$(OBJ_DIR)/%.o,$(KERNEL_ASM_SRC))
KERNEL_ELF := $(BIN_DIR)/kernel.elf
KERNEL_BIN := $(BIN_DIR)/kernel.bin

BASIC_IMG := basic.img

NASM := nasm
NASM_FLAGS := -f bin

GCC := $(HOME)/opt/cross/bin/i686-elf-gcc
GCC_FLAGS := -m32 -ffreestanding -O0

LD := $(HOME)/opt/cross/bin/i686-elf-ld
LD_FLAGS := -T linker.ld

all: $(BASIC_IMG)

$(MBR_BIN): $(MBR_SRC) | $(BIN_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $@

$(SECOND_BIN): $(SECOND_SRC) | $(BIN_DIR)
	$(NASM) $(NASM_FLAGS) $< -o $@

$(OBJ_DIR)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(GCC) $(GCC_FLAGS) $(DEP_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: kernel/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf $< -o $@

$(KERNEL_ELF): $(KERNEL_OBJ) $(KERNEL_ASM_OBJ) | $(BIN_DIR)
	$(LD) $(LD_FLAGS) -o $@ $^ $(HOME)/opt/cross/lib/gcc/i686-elf/12.2.0/libgcc.a

$(KERNEL_BIN): $(KERNEL_ELF) | $(BIN_DIR)
	objcopy -O binary $< $@

$(BASIC_IMG): $(MBR_BIN) $(SECOND_BIN) $(KERNEL_BIN)
	qemu-img create -f raw $@ 12M
	dd if=$(MBR_BIN) of=$@ bs=512 count=1 conv=notrunc
	dd if=$(SECOND_BIN) of=$@ bs=512 count=2 seek=1 conv=notrunc
	dd if=$(KERNEL_BIN) of=$@ bs=512 count=14 seek=3 conv=notrunc

run: $(BASIC_IMG)
	qemu-system-x86_64 -m 4G -drive format=raw,file=$(BASIC_IMG)

debug: $(BASIC_IMG)
	qemu-system-x86_64 -m 4G -drive format=raw,file=$(BASIC_IMG) -no-reboot -no-shutdown -monitor stdio

clean:
	rm -rf $(BIN_DIR) $(BASIC_IMG)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

-include $(KERNEL_OBJ:.o=.d)