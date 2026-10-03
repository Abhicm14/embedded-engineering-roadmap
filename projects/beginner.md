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
