SECTIONS {
    .text : {
        *(.text._start)
        *(.text)
        *(.text.*)
        PROVIDE(__stop_text = .);
    }
    . = ALIGN(CONSTANT(COMMONPAGESIZE));

    .rodata : { ASSERT(SIZEOF(.rodata) == 0, "No global data allowed!"); }
    .data   : { ASSERT(SIZEOF(.data) == 0,   "No global data allowed!"); }
    .bss    : { ASSERT(SIZEOF(.bss) == 0,    "No global data allowed!"); }
}
