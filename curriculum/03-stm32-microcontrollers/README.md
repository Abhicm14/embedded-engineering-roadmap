# 🦾 Step 3: STM32 & Microcontroller Architecture

> **Pillar:** FIRMWARE (STM32 + Peripherals)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*  
> **Prerequisites:** Ensure you have mastered [**`PREREQUISITES.md`**](../../PREREQUISITES.md). If you're new to hardware registers, practice first in our [**🌱 True Beginner On-Ramp (`beginner-onramp/`)**](../../beginner-onramp/README.md). Look up registers and terms in our [**📖 Beginner Glossary (`cheatsheets/glossary.md`)**](../../cheatsheets/glossary.md). Every driver must follow the [**7-Step Code Construction Workflow**](../../code-examples/README.md#the-7-step-code-construction-methodology).

---

## 🎯 Learning Objectives

Transition from generic programming to bare-metal microcontroller engineering using the ARM Cortex-M4 and STM32 silicon. By the end of this module, you should be able to:
1. Explain the ARMv7-M core registers, stack pointer banking (`MSP` vs `PSP`), and operating modes (Thread vs Handler).
2. Trace the entire reset boot sequence from power-on to `main()`.
3. Construct a bare-metal C startup file and linker script (`.ld`) from scratch without vendor IDE code generators.
4. Configure the STM32 Reset and Clock Control (RCC) clock tree, prescalers, and Phase-Locked Loop (PLL).
5. Understand the layered firmware architectural progression: Polling $\rightarrow$ Interrupts $\rightarrow$ DMA $\rightarrow$ RTOS.
6. Complete **Project 1: Bare-Metal GPIO & SysTick FSM Driver**.

---

## 🧭 Topic Guides in This Module

| Topic Document | Type | Key Concepts |
| :--- | :---: | :--- |
| [**1. Cortex-M Core & Registers**](cortex-m-core-and-registers.md) | `[INTUITION]` | `R0-R15`, `xPSR`, `CONTROL`, Privileged vs Unprivileged, Thread vs Handler mode. |
| [**2. Clock Trees & Reset Sequences**](clock-tree-and-reset.md) | `[INTUITION]` | HSI, HSE, PLL multiplication, AHB/APB prescalers, Flash latency wait-states. |
| [**3. Vector Table & NVIC Architecture**](vector-table-and-nvic.md) | `[INTUITION]` | Exception vectors, VTOR relocation, NVIC priority grouping, tail-chaining. |
| [**4. Toolchains, Linker Scripts & Makefiles**](toolchains-linkers-makefiles.md) | `[DEEP DIVE]` | GNU Arm toolchain, `MEMORY`/`SECTIONS` directives, LMA vs VMA, build automation. |
| [**5. Layered Firmware Architecture**](firmware-architecture.md) | `[INTUITION]` | Hardware Abstraction Layer, Board Support Package (BSP), device drivers, app layers. |

---

## 🛠️ Associated Project

- **[Project 1: Bare-Metal GPIO & SysTick FSM Driver](../../projects/beginner.md#project-1-gpio-control-board-bare-metal-systick-fsm)**

---

## ✅ Step 3 Completion Checklist

- [ ] Can draw the Cortex-M boot sequence from reset pin release to `main()`.
- [ ] Understands the role of `_sidata`, `_sdata`, `_sbss`, and `_estack` in the linker script.
- [ ] Can calculate PLL multipliers and dividers to achieve target CPU clock frequency.
- [ ] Understands why writing to GPIO `BSRR` is atomic while reading/writing `ODR` is not thread-safe.

➡️ **Next Step:** [Step 4: Essential MCU Peripherals](../04-essential-mcu-peripherals/README.md)
