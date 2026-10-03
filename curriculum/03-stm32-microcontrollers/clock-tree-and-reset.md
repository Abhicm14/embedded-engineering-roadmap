# ⏱️ Microcontroller Clock Trees & Reset Sequences

> Tracing the Phase-Locked Loop (PLL), AHB/APB bus prescalers, and the power-on boot sequence.

---

## 1. The Boot & Reset Sequence

When power is applied to an STM32 microcontroller and the external reset line (`NRST`) is pulled high:

```
                  Power-On / Reset Released
                             │
                             ▼
 ┌────────────────────────────────────────────────────────┐
 │ Hardware loads address 0x00000000 into MSP             │
 │ (Top of Stack address from linker script _estack)      │
 └───────────────────────────┬────────────────────────────┘
                             │
                             ▼
 ┌────────────────────────────────────────────────────────┐
 │ Hardware loads address 0x00000004 into PC              │
 │ (Entry point address of Reset_Handler)                 │
 └───────────────────────────┬────────────────────────────┘
                             │
                             ▼
 ┌────────────────────────────────────────────────────────┐
 │ Reset_Handler() executes:                              │
 │ 1. Copy initialized .data from Flash (LMA) to RAM (VMA)│
 │ 2. Zero-out .bss section in RAM                        │
 │ 3. Call SystemInit() (Configure RCC / PLL / FPU)       │
 │ 4. Call main()                                         │
 └────────────────────────────────────────────────────────┘
```

---

## 2. STM32 Reset & Clock Control (RCC) Architecture

A microcontroller silicon die contains multiple clock sources distributed via a hierarchical clock tree:

```
[ HSI: Internal 16MHz RC ] ──┐
                             ├─► [ PLL (Phase-Locked Loop) ] ──► SYSCLK (Up to 84-100MHz)
[ HSE: External 25MHz Quartz]─┘         Multipliers & Dividers         │
                                                                       ▼
                                                             [ AHB Prescaler (/1) ]
                                                                       │
                                                       ┌───────────────┴───────────────┐
                                                       ▼                               ▼
                                            [ APB1 Prescaler (/2) ]         [ APB2 Prescaler (/1) ]
                                            Low-Speed Periphs               High-Speed Periphs
                                            (TIM2-5, USART2, I2C1-3)        (TIM1, USART1, SPI1, ADC1)
```

---

## 3. Flash Memory Latency (Wait-States)

Silicon Flash memory is significantly slower than the CPU core:
- At 16 MHz, Flash can deliver data in 0 wait-states.
- At 84 MHz or 100 MHz, Flash access requires **2 to 3 wait-states (latency cycles)**.
- If you boost the CPU clock before configuring `FLASH->ACR` latency wait-states, the CPU attempts to fetch instructions faster than Flash can deliver, immediately crashing into an unrecoverable `HardFault`!

```c
// Mandatory order:
// 1. Enable Flash Pre-fetch, Instruction Cache, and 2 Wait-States
FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_2WS;

// 2. Configure PLL and switch SYSCLK to PLL
RCC->CFGR |= RCC_CFGR_SW_PLL;
```
