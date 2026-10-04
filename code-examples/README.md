# 💻 Embedded Code Construction & Examples Catalog

> **Core Philosophy:** *Never copy-paste code into your repository. Understand the electrical signal, register bit, timing diagram, and construction sequence behind every line before typing it.*

---

## 🧭 Pre-Coding Foundations

Before attempting to write firmware or compile these examples, you must master the core engineering principles documented in:

📘 [**Foundational Prerequisites Guide (`PREREQUISITES.md`)**](../PREREQUISITES.md)
- **Mathematical Foundations:** Binary, Hexadecimal, Two's complement, Bitwise logic, Fixed-point scaling.
- **Electrical Physics:** Ohm's Law, Pull-ups/Pull-downs, Decoupling capacitors, Open-drain vs Push-pull, Slew rate.
- **Computer Architecture:** Harvard vs Von Neumann, Memory-mapped I/O, CPU registers (PC, SP, LR), Endianness.
- **Embedded C Hygiene:** Pointers, Structure packing, the `volatile` qualifier, and deterministic ISR rules.
- **Datasheet Literacy:** How to locate pin multiplexing tables, register base offsets, and timing specs.

---

## 📐 The 7-Step Code Construction Methodology

Every code example in this repository was constructed using this systematic engineering framework:

```
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ Step 1: Establish the Hardware Contract & Electrical Characteristics   │
  │         Identify voltage rails, pin limits, pull-ups, and timing edges. │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 2: Extract Silicon Registers & Bitfields from Reference Manual     │
  │         Locate base addresses, control registers, and status flags.     │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 3: Calculate Mathematical Constants & Clock Dividers               │
  │         Derive prescalers, baud rate divisors, and sampling periods.    │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 4: Define Data Structures & Volatile Hardware Flags                │
  │         Use fixed-width types (uint8_t, uint32_t) and volatile vars.    │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 5: Write the Step-by-Step Initialization Sequence                  │
  │         Enable peripheral clock -> configure pins -> configure module.  │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 6: Implement Runtime Operations & Interrupt Handlers               │
  │         Check flags, clear interrupt flags in software, avoid delays.   │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 7: Verify with Lab Test Equipment & Unit Tests                     │
  │         Inspect waveforms with logic analyzer / scope, run test suites. │
  └─────────────────────────────────────────────────────────────────────────┘
```

---

## 🗂️ Catalog Index & Construction Breakdowns

### 1. Embedded C Architectural Patterns (`c/`)
*Self-testing C implementations with built-in unit tests executable on any host PC with GCC.*

- [`c/circular_buffer.c`](c/circular_buffer.c): **Lock-Free Circular Ring Buffer**
  - *Prerequisites:* FIFO queues, head/tail pointers, power-of-two modulo masking (`index & (SIZE - 1)`).
  - *How to Write Step-by-Step:* Define ring struct with buffer pointer, capacity, head, and tail. Implement `Push()` checking if full (`count == capacity`), increment head modulo capacity. Implement `Pop()` reading tail, increment tail modulo capacity. Ensure thread-safety for single-producer single-consumer without mutexes.
- [`c/fsm.c`](c/fsm.c): **Table-Driven Finite State Machine**
  - *Prerequisites:* State transition diagrams, event enums, function pointer dispatch tables.
  - *How to Write Step-by-Step:* Define state enums and event enums. Create a transition struct containing current state, triggering event, next state, and action function pointer. Loop through transition table on event occurrence, execute action callback, and update current state.
- [`c/debounce.c`](c/debounce.c): **Software Switch Debouncer**
  - *Prerequisites:* Mechanical contact bounce (5–20 ms noise), non-blocking timer ticks.
  - *How to Write Step-by-Step:* Create integrator or state counter. Sample raw GPIO state at a regular interval (e.g. 5 ms). Increment counter if state matches candidate; confirm stable state when counter exceeds debounce threshold.
- [`c/crc.c`](c/crc.c): **CRC-16 & CRC-32 Data Integrity**
  - *Prerequisites:* Polynomial division, bitwise XOR, frame check sequences.
  - *How to Write Step-by-Step:* Initialize shift register with seed (`0xFFFF`). For each byte, XOR into register, shift 8 times; if MSB is 1, XOR with generator polynomial (e.g. `0x1021` for CCITT).
- [`c/bit_ops.c`](c/bit_ops.c): **Bitwise Operations & Endianness**
  - *Prerequisites:* Bit masks, sign extension, byte swapping (`bswap32`).
  - *How to Write Step-by-Step:* Construct macros for bit setting (`REG |= (1UL << BIT)`), clearing (`REG &= ~(1UL << BIT)`), testing (`REG & (1UL << BIT)`), and byte-swapping using bitwise shifts.
- [`c/command_parser.c`](c/command_parser.c): **Serial CLI Command Dispatcher**
  - *Prerequisites:* String tokenization, null-termination, command lookup tables.
  - *How to Write Step-by-Step:* Buffer incoming UART characters until newline (`\r` or `\n`). Split string into command token and arguments using space delimiters. Look up command in an array of command-handler structs and invoke matched function.

---

### 2. Microchip PIC16F877A XC8 Track (`xc8/`)
*Comprehensive register-level implementations compiled with Microchip XC8 and simulated in PICSimLab.*

👉 **[Detailed Step-by-Step XC8 Construction Guide](xc8/README.md)**

- [`xc8/blink.c`](xc8/blink.c): Digital output on `RB0` with crystal configuration.
- [`xc8/button_led.c`](xc8/button_led.c): Button input with internal weak pull-ups (`OPTION_REG.nRBPU`) and 20 ms debouncer.
- [`xc8/timer_blink.c`](xc8/timer_blink.c): Hardware Timer0 prescaled overflow interrupt at 4 MHz.
- [`xc8/adc_voltage.c`](xc8/adc_voltage.c): 10-bit ADC (`ADCON0`, `ADCON1`) with integer millivolt scaling and LCD display.
- [`xc8/pwm_dimmer.c`](xc8/pwm_dimmer.c): CCP1 hardware PWM on `RC2` driven by Timer2 at 1 kHz.
- [`xc8/uart_echo.c`](xc8/uart_echo.c): 9600 baud full-duplex USART (`BRGH=1`, `SPBRG=25`) with overrun error recovery.
- [`xc8/i2c_temp.c`](xc8/i2c_temp.c): MSSP master I2C interface reading a digital thermal sensor.
- [`xc8/lcd_hello.c`](xc8/lcd_hello.c): 4-bit HD44780 alphanumeric LCD driver on `PORTD`.

---

### 3. Multi-Platform Hardware Blinky & Peripheral Examples

- [`stm32/blink.c`](stm32/blink.c): **STM32 Bare-Metal Register Blinky**
  - *Prerequisites:* ARM Cortex-M architecture, Memory-Mapped I/O, STM32 RCC clock enable registers.
  - *How to Write Step-by-Step:*
    1. Locate RCC base address (`0x40023800`) and AHB1 peripheral clock enable register (`RCC_AHB1ENR`, offset `0x30`).
    2. Enable GPIOC clock: Set bit 2 (`RCC_AHB1ENR |= (1UL << 2)`).
    3. Configure PC13 as output in `GPIOC->MODER` by writing `01` to bits `[27:26]`.
    4. In superloop, toggle PC13 atomically using Bit Set/Reset Register (`GPIOC->BSRR`).
- [`arduino/blink.ino`](arduino/blink.ino): **Hardware Timer Interrupt Blink**
  - *Prerequisites:* AVR Timer1 registers, CTC (Clear Timer on Compare Match) mode, prescalers.
  - *How to Write Step-by-Step:* Bypass Arduino `delay()`. Configure Timer1 in CTC mode with 1:1024 prescaler. Set OCR1A for 1 Hz compare match. Enable timer interrupt (`TIMSK1 |= (1 << OCIE1A)`) and toggle pin inside `ISR(TIMER1_COMPA_vect)`.
- [`esp32/wifi-blink.ino`](esp32/wifi-blink.ino): **FreeRTOS Non-Blocking WiFi Node**
  - *Prerequisites:* FreeRTOS task creation (`xTaskCreatePinnedToCore`), asynchronous WiFi state callbacks.
  - *How to Write Step-by-Step:* Separate network handling from hardware LED blinking into distinct FreeRTOS tasks pinned to separate Xtensa cores to ensure zero timing jitter on local hardware.
- [`verilog/blink.v`](verilog/blink.v): **Hardware Clock Frequency Divider**
  - *Prerequisites:* Synchronous digital design, D flip-flops, edge-triggered always blocks.
  - *How to Write Step-by-Step:* Define an N-bit binary counter clocked by the high-speed system oscillator (e.g. 50 MHz). On each rising clock edge, increment counter. Drive output LED from the most significant bit to produce human-visible blinking.
- [`verilog/uart_tx.v`](verilog/uart_tx.v): **8-N-1 UART Transmitter FSM**
  - *Prerequisites:* Finite State Machine in HDL, baud rate clock generator, shift registers.
  - *How to Write Step-by-Step:* Design a state machine with `IDLE`, `START`, `DATA`, and `STOP` states. Generate baud tick pulse. On transmit trigger, pull line LOW for 1 baud tick (Start bit), shift 8 data bits out sequentially, pull line HIGH for Stop bit, and return to IDLE.

---

## 🧪 Compiling & Running the C Unit Tests

All six C foundation modules (`c/`) compile cleanly under GCC with `-Wall -Wextra -Werror -std=c99` and include self-verifying test suites:

```bash
# Test Circular Buffer
gcc -Wall -Wextra -Werror -std=c99 c/circular_buffer.c -o test_cb && ./test_cb

# Test Finite State Machine
gcc -Wall -Wextra -Werror -std=c99 c/fsm.c -o test_fsm && ./test_fsm

# Test Switch Debouncer
gcc -Wall -Wextra -Werror -std=c99 c/debounce.c -o test_debounce && ./test_debounce

# Test CRC Algorithms
gcc -Wall -Wextra -Werror -std=c99 c/crc.c -o test_crc && ./test_crc

# Test Bitwise Operations
gcc -Wall -Wextra -Werror -std=c99 c/bit_ops.c -o test_bits && ./test_bits

# Test Command Parser
gcc -Wall -Wextra -Werror -std=c99 c/command_parser.c -o test_parser && ./test_parser
```
