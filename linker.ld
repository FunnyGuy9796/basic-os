OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS {
    . = 0x10000;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    end_of_kernel = .;

    . = ALIGN(4);
}