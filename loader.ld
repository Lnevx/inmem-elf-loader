SECTIONS {
    . = 0x401000;
    .text : {
        *(.text._start)
        *(.text)
        *(.text.*)
        PROVIDE(__stop_text = .);
    }
    .rodata : { ASSERT(SIZEOF(.rodata) == 0, "No global data allowed!"); }
    .data   : { ASSERT(SIZEOF(.data) == 0,   "No global data allowed!"); }
    .bss    : { ASSERT(SIZEOF(.bss) == 0,    "No global data allowed!"); }
}
