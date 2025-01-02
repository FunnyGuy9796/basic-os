OUTPUT_FORMAT(elf32-i386)
ENTRY(main)

SECTIONS {
    . = 0x10000;

    .text : {
        /* Ensure what is in .text.startup appears first */
        *(.text.startup)
        *(.text*)
    }

    .data : {
        *(.data)
    }

    .rodata : {
        *(.rodata*)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }

    end_of_kernel = .;

    . = ALIGN(4);
}
