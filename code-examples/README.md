# 💻 Embedded Code Examples Catalog

> Standalone, production-grade, well-commented code implementations across C, STM32 Bare-Metal, Arduino, ESP32, and Verilog HDL.

---

## 🧭 Directory Index

### 1. Embedded C Fundamental Patterns (`c/`)
- [`c/circular_buffer.c`](c/circular_buffer.c): Lock-free, power-of-two FIFO circular ring buffer.
- [`c/fsm.c`](c/fsm.c): Table-driven Finite State Machine with function pointer transition callbacks.
- [`c/debounce.c`](c/debounce.c): Non-blocking software switch debounce state machine.
- [`c/crc.c`](c/crc.c): Standard CRC-16 (CCITT) and CRC-32 polynomial calculation functions.
- [`c/bit_ops.c`](c/bit_ops.c): Bit manipulation macros, bitfield extraction, and endianness byte swapping.
- [`c/command_parser.c`](c/command_parser.c): Serial CLI command tokenizer and dispatcher.

### 2. Multi-Platform Hardware Blinky & Peripheral Examples
- [`stm32/blink.c`](stm32/blink.c): Pure register-level STM32 GPIO blinky (zero HAL).
- [`arduino/blink.ino`](arduino/blink.ino): Minimal Arduino framework baseline.
- [`esp32/wifi-blink.ino`](esp32/wifi-blink.ino): FreeRTOS-backed ESP32 Wi-Fi telemetry and asynchronous LED blinker.
- [`verilog/blink.v`](verilog/blink.v): Digital clock frequency divider and LED blinker in synthesizable Verilog.
- [`verilog/uart_tx.v`](verilog/uart_tx.v): Full 8-N-1 UART transmitter engine in synthesizable Verilog HDL.

---

## 🛠️ Compiling & Running the C Tests

All C code examples in `c/` are self-contained with built-in `main()` functions and unit test assertions:

```bash
# Compile and run any example on host PC
gcc -Wall -Wextra -Werror -std=c99 c/circular_buffer.c -o test_cb
./test_cb

gcc -Wall -Wextra -Werror -std=c99 c/bit_ops.c -o test_bits
./test_bits
```
