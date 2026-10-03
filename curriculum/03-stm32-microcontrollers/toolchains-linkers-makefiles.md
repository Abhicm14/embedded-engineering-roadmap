# 🛠️ Toolchains, Linker Scripts & Makefiles

> Building firmware without vendor IDEs using `arm-none-eabi-gcc`, custom `.ld` scripts, and Make.

---

## 1. Anatomy of an Embedded Linker Script (`.ld`)

The linker script instructs the GNU Linker (`ld`) where to place code and data in physical memory:

```ld
ENTRY(Reset_Handler)

/* Define hardware memory regions from chip datasheet */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM); /* Stack starts at end of RAM */

SECTIONS
{
    /* Vector table MUST be at start of Flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code and constants */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Used by startup.c to copy initialized globals into RAM */
    _sidata = LOADADDR(.data);

    /* Initialized data placed in RAM, but loaded in Flash */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /* Zero-initialized variables */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}
```

---

## 2. Load Memory Address (LMA) vs Virtual Memory Address (VMA)

- **VMA (Virtual Memory Address):** The runtime memory address where code expects to read/write the variable (e.g. `0x20000000` in SRAM).
- **LMA (Load Memory Address):** The physical Flash address where the initial values are permanently burned (e.g. `0x08004500` in Flash).
- `startup.c` bridges this gap by copying bytes from `_sidata` (LMA) to `_sdata` (VMA) on every reset!
