# 🟢 Beginner Portfolio Projects (Projects 1 & 2)

---

## Project 1: GPIO Control Board (Bare-Metal SysTick FSM)

> **Core Focus:** Pure register-level C (zero HAL/CMSIS bloat), vector table startup, custom linker script, SysTick timer, and switch debounce state machine.

### 1. Hardware Architecture & Pinout
- **MCU:** STM32F401CCU6 ("Black Pill") or STM32F411CEU6.
- **Debugger:** ST-Link V2 (SWD: `SWDIO`, `SWCLK`, `GND`, `3.3V`).
- **Inputs & Outputs:**
  - `PC13`: User LED (Active LOW with internal pull-up).
  - `PA0`: External Pushbutton with $10\text{ k}\Omega$ pull-up resistor to 3.3V.

```
       STM32F401 Black Pill
    ┌─────────────────────────┐
    │                     PC13├───► [Onboard Active-LOW LED]
    │                         │
    │                      PA0├───┬───[10k Pull-Up]─── 3.3V
    │                         │   │
    │                         │ ┌─┴─┐
    │                         │ │ O │ Pushbutton to GND
    │                         │ └─┬─┘
    │                         │   │
    │                      GND├───┴─── GND
    └─────────────────────────┘
```

### 2. Software Architecture & Debounce State Machine

```
              ┌─────────────────────┐
              │    BTN_RELEASED     │
              └──────────┬──────────┘
                         │ Raw button reads LOW (Press detected)
                         ▼
              ┌─────────────────────┐
              │  BTN_DEBOUNCE_PRESS │ ──(Bounces / Released)──► Revert to BTN_RELEASED
              └──────────┬──────────┘
                         │ 25 ms stable LOW
                         ▼
              ┌─────────────────────┐
              │     BTN_PRESSED     │ ──► Toggle LED via atomic BSRR!
              └──────────┬──────────┘
                         │ Raw button reads HIGH (Release detected)
                         ▼
              ┌─────────────────────┐
              │ BTN_DEBOUNCE_REL    │
              └──────────┬──────────┘
                         │ 25 ms stable HIGH
                         ▼
              (Back to BTN_RELEASED)
```

### 3. Key Register Manipulations
- **RCC Clock Enable:** `RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN);`
- **Output Mode:** Set `GPIOC->MODER` bits `[27:26]` to `01` (General purpose output).
- **Atomic Pin Toggle via `BSRR`:**
  ```c
  if (GPIOC->ODR & (1UL << 13)) {
      GPIOC->BSRR = (1UL << (13 + 16)); // Clear PC13 (Turn ON)
  } else {
      GPIOC->BSRR = (1UL << 13);        // Set PC13 (Turn OFF)
  }
  ```

---

## Project 2: UART Command Console (Ring Buffer & Shell)

> **Core Focus:** Asynchronous serial framing, interrupt-driven UART reception, lock-free circular FIFO ring buffer, and non-blocking token parsing.

### 1. Hardware Architecture & Wiring
- Connect STM32 `PA2` (`USART2_TX`) to USB-UART Adapter `RXD`.
- Connect STM32 `PA3` (`USART2_RX`) to USB-UART Adapter `TXD`.
- Connect USB-UART `GND` to STM32 `GND`.
- Set terminal baud rate to **115,200 baud, 8 data bits, no parity, 1 stop bit (8-N-1)**.

### 2. Data Pipeline Architecture

```
[ PC Keyboard ] ──(115200 Baud)──► PA3 (USART2 RX Pin)
                                           │
                                           ▼ (Hardware Interrupt)
                                 [ USART2_IRQHandler ]
                                           │
                                 RingBuffer_Push(&rx_fifo, byte)
                                           │
                                           ▼
                                 [ Circular Ring Buffer ]
                                           │
                                 RingBuffer_Pop(&rx_fifo, &byte)
                                           │
                                           ▼ (Main Loop)
                                 [ Command Parser FSM ]
                                 - Matches "led on"   -> Sets PC13 LOW
                                 - Matches "led off"  -> Sets PC13 HIGH
                                 - Matches "status"   -> Prints uptime & ticks
```

### 3. Code Implementation Reference
- Ring buffer implementation: [`code-examples/c/circular_buffer.c`](../code-examples/c/circular_buffer.c)
- Command parser engine: [`code-examples/c/command_parser.c`](../code-examples/c/command_parser.c)

---

## 🔌 PIC Beginner Track: Projects P1 & P2

### Project P1: PIC16F877A Debounced Switch & LED Pattern Controller
- **Platform:** Microchip PIC16F877A on breadboard or PICSimLab (Board 1 / PICGenios).
- **Core Focus:** Pure register-level GPIO configuration (`TRISB`, `PORTB`, `OPTION_REG.nRBPU`), non-blocking switch debouncing state machine, and hardware Timer0 overflow timebase.
- **Hardware Architecture:**
  - `RB0` (Pin 33): Active-high LED with 330Ω resistor to GND.
  - `RB1` (Pin 34): Pushbutton connected to GND (utilizes internal weak pull-up via `OPTION_REGbits.nRBPU = 0`).
- **Implementation Guide & Code:**
  - Step-by-Step Tutorial: [PIC Foundations (Steps 1–3)](../pic-mplab-xc8/beginner.md)
  - Standalone Code: [`code-examples/xc8/button_led.c`](../code-examples/xc8/button_led.c)
  - Hardware Timer Example: [`code-examples/xc8/timer_blink.c`](../code-examples/xc8/timer_blink.c)

### Project P2: PIC16F877A Interactive UART Serial CLI
- **Platform:** PIC16F877A USART hardware module connected to PC via USB-UART adapter (or PICSimLab virtual serial port).
- **Core Focus:** Full-duplex USART configuration (`TXSTA`, `RCSTA`, `SPBRG`), receiver overrun (`OERR`) error handling, and terminal command dispatch.
- **Baud Rate:** 9600 baud, 8-N-1 (`BRGH = 1`, `SPBRG = 25` at 4 MHz crystal, 0.16% error).
- **Commands Supported:** `HELP`, `LED ON`, `LED OFF`, `STATUS`.
- **Implementation Guide & Code:**
  - Standalone Code: [`code-examples/xc8/uart_echo.c`](../code-examples/xc8/uart_echo.c)
  - Cheatsheet: [PIC XC8 Cheatsheet](../cheatsheets/pic-xc8-cheatsheet.md)
