# 🐞 On-Chip Debugging with GDB & OpenOCD

> Controlling MCU silicon via Serial Wire Debug (SWD), setting breakpoints, watchpoints, and inspecting memory.

---

## 1. The SWD / JTAG Physical Debug Link

The Serial Wire Debug (SWD) interface replaces bulky 20-pin JTAG with a streamlined 2-wire bus:
- **`SWCLK`:** Clock driven by host debug probe (ST-Link / J-Link / CMSIS-DAP).
- **`SWDIO`:** Bidirectional data line transmitting GDB control packets.
- **`GND` & `VREF`:** Signal reference and voltage target sense.

```
Host PC running arm-none-eabi-gdb
       │ (TCP port 3333)
       ▼
OpenOCD GDB Server
       │ (USB)
       ▼
ST-Link V2 Probe ──(SWDIO / SWCLK)──► ARM Cortex-M Core Debug Unit (DWT/FPB)
```

---

## 2. Essential GDB Commands for Embedded

```bash
# Connect to OpenOCD server
(gdb) target extended-remote localhost:3333

# Reset MCU and halt at Reset_Handler
(gdb) monitor reset halt

# Load new ELF firmware into target Flash
(gdb) load

# Breakpoints
(gdb) break main                  # Software breakpoint at main
(gdb) hbreak I2C1_EV_IRQHandler   # Hardware breakpoint in Flash memory

# Watchpoints (Data write trap - stopped by DWT hardware unit)
(gdb) watch g_buffer_overflow_flag

# Stepping execution
(gdb) step                        # Step into function
(gdb) next                        # Step over function
(gdb) continue                    # Resume execution

# Inspecting State
(gdb) info registers              # Dump R0-R15, xPSR, MSP, PSP
(gdb) backtrace                   # Print current call stack
(gdb) print /x *GPIOA             # Print entire GPIOA struct in hex
(gdb) x/16xw 0x20000000           # Hex dump 16 32-bit words at address
```
