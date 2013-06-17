ENTRY(_startup)
load_addr = 0x8000;
run_addr = 0x3c00000;
SECTIONS
{
    . = load_addr;
    .startup :
    {
        *(.startup)
    }
    . = run_addr;
    .text :
    {
        *(.text*)
        *(.rodata*)
    }
    .data :
    {
        *(.data*)
    }
    .bss :
    {
        *(.bss*)
    }
    loaded_text_start = load_addr + SIZEOF(.startup);
    loaded_end = loaded_text_start + SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.bss);
}
