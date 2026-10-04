# 🧠 Embedded Engineering Prerequisites

> **What Every Aspiring Embedded Engineer Must Master Before Writing Code**  
> *"If you cannot trace the electrical signal, register bit, and memory address behind your code, you are guessing, not engineering."*

---

## 🧭 Why Prerequisites Matter in Embedded Systems

In desktop or web development, software runs atop multiple layers of abstraction: an operating system, virtual memory managers, memory garbage collectors, and runtime execution engines. If a bug occurs, the OS displays an error or terminates the process safely.

In **Embedded Systems**, your firmware runs directly on the bare silicon:
- A misplaced pointer doesn't throw a polite runtime exception; it triggers a **HardFault exception**, locks the processor core in an infinite handler loop, or corrupts hardware peripheral control registers.
- Writing to a GPIO output pin that is physically shorted to a low-impedance rail without a current-limiting resistor will **permanently burn the silicon output driver transistors**.
- Misunderstanding the `volatile` qualifier causes compiler optimization passes (`-O2` / `-O3`) to completely optimize away hardware register polling loops, leaving your firmware frozen indefinitely.

Before writing a single line of firmware for STM32, PIC, ESP32, or AVR, you must build a solid mental model across **four foundational disciplines**:

```
                       ┌────────────────────────────────────────────────────────┐
                       │           EMBEDDED ENGINEERING PREREQUISITES          │
                       └───────────────────────────┬────────────────────────────┘
         ┌─────────────────────────┬───────────────┴───────────────┬─────────────────────────┐
         ▼                         ▼                               ▼                         ▼
┌─────────────────┐       ┌─────────────────┐             ┌─────────────────┐       ┌─────────────────┐
│ 1. MATHEMATICAL │       │ 2. ELECTRICAL   │             │ 3. COMPUTER     │       │ 4. EMBEDDED C   │
│    FOUNDATIONS  │       │    PHYSICS      │             │    ARCHITECTURE │       │    MASTERY      │
├─────────────────┤       ├─────────────────┤             ├─────────────────┤       ├─────────────────┤
│ • Binary & Hex  │       │ • Ohm's & KCL   │             │ • Harvard/Von N.│       │ • Pointer Arith │
│ • 2's Complement│       │ • Pull-up/down  │             │ • CPU Registers │       │ • Struct Align  │
│ • Bitwise Masks │       │ • Decoupling    │             │ • Stack/Heap/Map│       │ • Volatile/Const│
│ • Fixed-Point   │       │ • Open-Drain    │             │ • Memory Maps   │       │ • ISR Hygiene   │
│ • Nyquist / DSP │       │ • Rise/Fall Time│             │ • Endianness    │       │ • Atomic Ops    │
└─────────────────┘       └─────────────────┘             └─────────────────┘       └─────────────────┘
```

---

## 1. Mathematical & Number Systems Foundations

### 1.1 Binary, Hexadecimal, and Byte Representation
Microcontroller registers are banks of physical flip-flops that store binary voltages ($0\text{ V} = 0$, $3.3\text{ V} = 1$). You must be fluent in translating between binary, hexadecimal, and decimal without hesitation.

| Decimal | Binary (8-bit) | Hexadecimal | Common Peripheral Representation |
|:-------:|:--------------:|:-----------:|:---------------------------------|
| `0` | `0000 0000` | `0x00` | Clear all register bits |
| `1` | `0000 0001` | `0x01` | Bit 0 set (Enable bit) |
| `15` | `0000 1111` | `0x0F` | Lower nibble mask |
| `16` | `0001 0000` | `0x10` | Bit 4 set |
| `127` | `0111 1111` | `0x7F` | Maximum signed 8-bit positive integer |
| `128` | `1000 0000` | `0x80` | Bit 7 set (Sign bit / MSB) |
| `255` | `1111 1111` | `0xFF` | All 8 bits set (Byte broadcast / mask) |

### 1.2 Two's Complement Signed Arithmetic
Microcontrollers represent negative numbers using **Two's Complement**:
- To negate an 8-bit binary number: **Invert all bits and add 1**.
  $$\text{Example: } +5 = \text{0b00000101} \implies \text{Invert: } \text{0b11111010} \implies \text{Add 1: } \text{0b11111011} = -5$$
- An 8-bit signed integer (`int8_t`) spans $-128$ to $+127$.
- An 8-bit unsigned integer (`uint8_t`) spans $0$ to $255$.
> [!WARNING]
> Mixing signed and unsigned integers in peripheral comparisons is one of the most common causes of silent embedded bugs (e.g. comparing `int16_t temperature = -5` with `uint16_t threshold = 10` causes the compiler to promote `-5` to `65531`, making `-5 > 10` evaluate to `true`!).

### 1.3 Bitwise Operations & Masks
You will manipulate register bits constantly. Memorize these fundamental identities:

```c
// 1. Setting a bit (Bitwise OR with mask)
REG |= (1UL << BIT_POS);          // Forces BIT_POS to 1, leaves other bits unchanged

// 2. Clearing a bit (Bitwise AND with inverted mask)
REG &= ~(1UL << BIT_POS);         // Forces BIT_POS to 0, leaves other bits unchanged

// 3. Toggling a bit (Bitwise XOR with mask)
REG ^= (1UL << BIT_POS);          // Inverts BIT_POS (0->1 or 1->0)

// 4. Checking if a bit is set (Bitwise AND test)
if (REG & (1UL << BIT_POS)) { ... } // Evaluates true if bit is 1

// 5. Clearing multiple bits and setting new bitfield values
REG = (REG & ~MASK) | (VALUE & MASK);
```

### 1.4 Fixed-Point Arithmetic vs Floating-Point
Many lower-cost microcontrollers (ARM Cortex-M0/M0+, PIC16/18, AVR) **do not have a hardware Floating-Point Unit (FPU)**.
- Performing software floating-point operations (`float a = b * 3.14159f;`) pulls in thousands of bytes of software emulation runtime library code and consumes hundreds of CPU clock cycles.
- **Master Fixed-Point scaling**: Store millivolts ($2500\text{ mV}$) instead of volts ($2.50\text{ V}$), millidegrees ($25340\text{ m}^\circ\text{C}$) instead of degrees ($25.34^\circ\text{C}$), and scale by powers of 2 (e.g. Q15 or Q31 formats) so operations can be computed with fast integer shifts.

---

## 2. Electrical Physics & Circuit Prereqs

You cannot write low-level firmware without understanding the physical behavior of electrons moving through copper traces and semiconductor gates.

### 2.1 Ohm's Law and Kirchhoff's Laws
- **Ohm's Law:** $V = I \times R \iff I = \frac{V}{R} \iff R = \frac{V}{I}$
  - *Example:* Driving an LED from a 3.3V GPIO pin. If the LED forward voltage $V_f = 2.0\text{ V}$ and desired current $I = 5\text{ mA}$:
    $$R = \frac{V_{DD} - V_f}{I} = \frac{3.3\text{ V} - 2.0\text{ V}}{0.005\text{ A}} = \frac{1.3}{0.005} = 260\,\Omega \implies \text{Use standard } 270\,\Omega \text{ or } 330\,\Omega.$$
- **Kirchhoff's Current Law (KCL):** Total current entering a junction equals total current leaving.
- **Kirchhoff's Voltage Law (KVL):** Sum of voltages around any closed circuit loop is zero.

### 2.2 Pull-Up and Pull-Down Resistors
A digital input pin connected to nothing is in a high-impedance state (**floating**). In this state, trace capacitance and stray electromagnetic radiation cause the input buffer to flip unpredictably between 0 and 1, burning power and generating phantom switch triggers.

```
       Pull-Up Configuration                  Pull-Down Configuration
             +3.3V / +5V                               +3.3V / +5V
                  │                                         │
                 ┌┴┐                                      ┌─┴─┐
                 │ │ 10k Resistor                         │ O │ Pushbutton (Open)
                 └┬┘                                      └─┬─┘
                  ├───► Microcontroller                     ├───► Microcontroller
                  │     Input Pin                           │     Input Pin
                ┌─┴─┐                                      ┌┴┐
                │ O │ Pushbutton (Open)                    │ │ 10k Resistor
                └─┬─┘                                      └┬┘
                  │                                         │
                 GND                                       GND
   (Pin is HIGH until button pressed)         (Pin is LOW until button pressed)
```

### 2.3 Push-Pull vs Open-Drain Output Stages
- **Push-Pull (Totem-Pole):** Consists of a high-side P-channel MOSFET and a low-side N-channel MOSFET. Active high drives the pin to $V_{DD}$; active low pulls the pin to $GND$. Used for SPI, UART TX, and general LED driving.
- **Open-Drain (Open-Collector):** The high-side P-channel transistor is omitted. The pin can only pull low to $GND$ or enter high-impedance ($Hi\text{-}Z$).
  - An external physical pull-up resistor pulls the bus high when all devices release the line.
  - Enables **wired-AND logic**, allowing multiple chips to share a single wire without short-circuit contention (the electrical basis of **I2C** and **1-Wire**).

### 2.4 Decoupling Capacitors & Power Rail Stability
Every time a digital CMOS gate switches state from 0 to 1, both upper and lower transistors are momentarily on simultaneously, creating a nanosecond current surge.
- Parasitic inductance in PCB traces ($V = L \frac{di}{dt}$) causes instantaneous voltage dips on the $V_{DD}$ rail ("rail collapse") and voltage spikes on $GND$ ("ground bounce").
- **The Rule:** Always place a **$100\text{ nF}$ ceramic decoupling capacitor** as physically close as possible (less than $5\text{ mm}$) to every $V_{DD}$ / $V_{SS}$ pin pair on the microcontroller.

### 2.5 Signal Rise Times, Slew Rate & Ringing
- Wires and breadboard rows are not ideal conductors; they possess parasitic inductance ($L$) and capacitance ($C$).
- A fast digital clock edge ($10\text{ MHz}$ SPI clock) has high-frequency harmonic content up to hundreds of megahertz.
- High $\frac{dv}{dt}$ combined with parasitic inductance causes **ringing (overshoot and undershoot)**.
- If ringing exceeds $V_{DD} + 0.3\text{ V}$ or drops below $-0.3\text{ V}$, it triggers the internal ESD protection diodes of the MCU, causing latch-up or reset.

---

## 3. Microcontroller & Computer Architecture Essentials

### 3.1 Harvard vs Von Neumann Architecture
- **Von Neumann Architecture:** Shared bus and memory space for both program instructions and runtime data (used in classic x86 PCs and simple MPUs).
- **Harvard Architecture:** Separate, independent physical buses and address spaces for Program Memory (Flash) and Data Memory (SRAM).
  - Modern ARM Cortex-M processors use a **Modified Harvard Architecture**: separate instruction (`I-Code`) and data (`D-Code`) buses internally connecting to an integrated memory bus matrix, allowing simultaneous instruction fetch and data read/write in a single CPU cycle.

### 3.2 The Core CPU Registers
Inside the microcontroller core, operations do not happen in RAM; they happen within internal CPU registers:

```
  ARM Cortex-M Core Registers
 ┌────────────────────────────────────────────────────────┐
 │ R0 - R12   : General-purpose arithmetic registers      │
 │ R13 (SP)   : Stack Pointer (MSP - Main, PSP - Process) │
 │ R14 (LR)   : Link Register (Holds function return addr)│
 │ R15 (PC)   : Program Counter (Address of NEXT opcode)  │
 │ xPSR       : Program Status Register (Flags: N, Z, C, V│
 └────────────────────────────────────────────────────────┘
```

- **Program Counter (PC):** Points to the exact memory address of the next machine instruction to be fetched and executed.
- **Stack Pointer (SP):** Points to the top of the Call Stack in SRAM where local function variables, saved register contexts, and return addresses reside.
- **Link Register (LR):** Automatically loaded with the return address whenever a function call (`BL` / Branch with Link) occurs.

### 3.3 The Embedded Memory Hierarchy
```
  Lowest Latency / Smallest Size
  ▲  ┌────────────────────────────┐
  │  │ Core CPU Registers         │ (Zero wait states, ~1 cycle)
  │  ├────────────────────────────┤
  │  │ On-Chip SRAM (Data RAM)    │ (Zero wait states, variables, stack, heap)
  │  ├────────────────────────────┤
  │  │ On-Chip Flash (ROM)        │ (Non-volatile firmware opcodes, const tables)
  │  ├────────────────────────────┤
  │  │ Internal Data EEPROM       │ (Byte-addressable calibration, wear-resistant)
  │  ├────────────────────────────┤
  │  │ External SPI/QQSPI Flash   │ (Megabytes storage, requires bus transactions)
  ▼  └────────────────────────────┘
  Highest Latency / Largest Size
```

### 3.4 Memory-Mapped I/O (MMIO)
Microcontrollers do not use special instructions to communicate with peripherals. Instead, every peripheral (GPIO ports, Timers, ADC, UART, SPI) is mapped to specific physical address ranges in the unified 32-bit or 16-bit address space.
- Writing to address `0x40020014` on an STM32 does not store a byte in RAM; it writes to the `GPIOA->ODR` output data register, altering the physical gate voltage of pin PA5!

### 3.5 Endianness
- **Little-Endian (ARM Cortex-M, x86, PIC):** The least significant byte (LSB) is stored at the lowest memory address.
  - Number `0x12345678` in memory:
    $$\text{Addr } 0x00 = \text{0x78}, \quad \text{Addr } 0x01 = \text{0x56}, \quad \text{Addr } 0x02 = \text{0x34}, \quad \text{Addr } 0x03 = \text{0x12}$$
- **Big-Endian (Network byte order, CAN protocol payloads):** The most significant byte (MSB) is stored at the lowest address.
- Firmware transmitting binary sensor packets over networks or automotive CAN buses must explicitly convert endianness using functions like `htons()` or `__builtin_bswap32()`.

---

## 4. Embedded C Programming Prerequisites

Writing embedded C requires a disciplined subset of C, fundamentally different from standard application programming.

### 4.1 Pointers and Pointer Arithmetic
You must understand pointers not as abstract concepts, but as **physical memory addresses on the bus matrix**.

```c
// Casting a physical silicon register address to a volatile pointer:
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000UL)

// Dereferencing the pointer writes directly to silicon flip-flops:
GPIOA_MODER |= (1UL << 10);
```

### 4.2 The `volatile` Type Qualifier
Tells the compiler optimizer: **"The value at this address can change at any time due to external hardware events outside the compiler's knowledge; do not optimize, cache, or eliminate reads/writes to this variable."**

> [!CAUTION]
> If you omit `volatile` on a global flag shared with an Interrupt Service Routine (ISR) or on a hardware status register:
> ```c
> // WRONG: Compiler reads UART_SR once, caches it in CPU register R2, 
> // and generates an infinite loop because it sees no code modifying it inside the loop!
> while (!(UART_SR & RXNE_FLAG)); 
> 
> // CORRECT: Compiler forces a physical memory bus read on EVERY iteration:
> while (!(*(volatile uint32_t *)UART_SR_ADDR & RXNE_FLAG));
> ```

### 4.3 Structure Padding and Memory Alignment
32-bit CPU architectures (ARM Cortex-M) access 32-bit words most efficiently when aligned to 4-byte boundaries. By default, compilers insert invisible padding bytes between struct members:

```c
struct SensorPacket {
    uint8_t  id;       // 1 byte
    // 3 bytes of invisible padding inserted here by compiler!
    uint32_t timestamp;// 4 bytes (aligned to 4-byte boundary)
    uint16_t value;    // 2 bytes
    // 2 bytes of trailing padding inserted here!
}; // Total size = 12 bytes, NOT 7 bytes!
```

If you transmit this struct directly over UART, SPI, or CAN to another system with a different compiler or alignment, the receiver will read garbled data!
- **Solution:** Use `#pragma pack(1)` or `__attribute__((packed))` for network and peripheral communication structs.

### 4.4 Rules of Interrupt Service Routine (ISR) Hygiene
Interrupts preempt the main firmware loop unpredictably. Follow these non-negotiable rules:
1. **Keep it short:** Never perform heavy calculations, floating-point math, or string formatting (`sprintf`) inside an ISR.
2. **Never block:** Never call blocking delays (`__delay_ms()`), mutex locks, or wait loops inside an ISR.
3. **Always clear the interrupt flag:** Microcontroller hardware sets an interrupt flag; the firmware must manually clear it in software before exiting the ISR to prevent an infinite interrupt loop.
4. **Use volatile variables:** All global variables shared between an ISR and background code must be qualified with `volatile`.
5. **Ensure atomicity:** Accessing a 16-bit or 32-bit variable from an 8-bit MCU main loop requires disabling interrupts temporarily to prevent reading half-updated data (**torn read**).

---

## 5. How to Read a Silicon Reference Manual

Never write firmware by copying snippets from forums without verifying the underlying register specification in the manufacturer's silicon documentation.

```
       Manufacturer Documentation Hierarchy
 ┌────────────────────────────────────────────────────────┐
 │ 1. Silicon Datasheet (~100 pages)                      │
 │    - Pinouts, pin multiplexing, DC electrical specs    │
 │    - Maximum clock frequencies, ADC sampling limits    │
 ├────────────────────────────────────────────────────────┤
 │ 2. Silicon Reference Manual (~1,000 to 2,000 pages)    │
 │    - Complete peripheral registers and bitfield maps   │
 │    - Clock tree distribution and reset behaviors       │
 │    - State machine operational sequences               │
 ├────────────────────────────────────────────────────────┤
 │ 3. Architecture Technical Reference Manual (~800 pages)│
 │    - CPU core registers (NVIC, SCB, SysTick, MPU)      │
 │    - Instruction set architecture and assembly opcodes │
 ├────────────────────────────────────────────────────────┤
 │ 4. Silicon Errata Sheet (~30 pages)                    │
 │    - Known silicon bugs and required software fixes    │
 └────────────────────────────────────────────────────────┘
```

### Navigating Register Tables
When consulting a peripheral register section in a Reference Manual (e.g. `TIMx_CR1`):
1. **Check Base Address and Offset:** Peripheral base (e.g. `0x40000000`) + register offset (`0x00`) = physical address.
2. **Check Reset Value:** What state the register flip-flops assume upon hardware reset (e.g. `0x00000000`).
3. **Check Bit Access Types:**
   - `R/W`: Read and write permitted.
   - `RO`: Read-only (hardware status flag; writes are ignored).
   - `WO`: Write-only.
   - `RC_W0`: Read-Clear-Write-0 (Software clears the flag by writing `0`, writing `1` has no effect).

---

## 6. Workbench Test Equipment & Diagnostic Mindset

Before blaming the compiler or writing firmware workarounds, verify physical reality with workbench instruments:

```
               Diagnostic Escalation Hierarchy
  ┌──────────────────────────────────────────────────────────────┐
  │ Level 1: Digital Multimeter (DMM)                            │
  │ - Measure DC voltage rails (is 3.3V actually 3.3V?)          │
  │ - Continuity test for cold solder joints or bridge shorts    │
  ├──────────────────────────────────────────────────────────────┤
  │ Level 2: USB Logic Analyzer (PulseView / sigrok)             │
  │ - Decode digital protocol packets (UART, SPI, I2C, CAN)      │
  │ - Verify baud rates, ACK/NACK bits, setup and hold times     │
  ├──────────────────────────────────────────────────────────────┤
  │ Level 3: Digital Storage Oscilloscope (DSO)                  │
  │ - Inspect analog waveform shape, rise/fall times, ringing    │
  │ - Measure ground bounce, rail ripple, and bus contention     │
  ├──────────────────────────────────────────────────────────────┤
  │ Level 4: Hardware In-Circuit Debugger (GDB / OpenOCD / SWD) │
  │ - Set hardware breakpoints and watchpoints in CPU registers  │
  │ - Inspect memory buffers and decode HardFault registers      │
  └──────────────────────────────────────────────────────────────┘
```

---

## 7. The 7-Step Code Construction Workflow

To enforce deep understanding and eliminate mindless copy-pasting, every piece of firmware in this curriculum follows the **7-Step Code Construction Workflow**:

```
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ Step 1: Establish the Hardware Contract & Electrical Characteristics   │
  │         Check schematic, operating voltage, pin max current, pull-up.   │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 2: Extract Silicon Registers & Bitfields from Reference Manual     │
  │         Identify TRIS/MODER, PORT/ODR, control & status registers.      │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 3: Calculate Mathematical Constants & Clock Dividers               │
  │         Derive prescalers, timer reload values, baud rate generators.   │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 4: Define Data Structures & Volatile Hardware Flags                │
  │         Declare stdint fixed-width types, packed structs, volatile flags│
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 5: Write the Step-by-Step Initialization Sequence                  │
  │         Power/Clock peripheral -> Configure pins -> Configure registers │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 6: Implement Runtime Operations & Interrupt Handlers               │
  │         Non-blocking state polling, flag clears, zero blocking delays.  │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 7: Verify with Lab Test Equipment & Unit Tests                     │
  │         Verify with logic analyzer, oscilloscope, DMM, and unit tests.  │
  └─────────────────────────────────────────────────────────────────────────┘
```

When building any project or studying any example:
1. **Never copy raw code into your editor.**
2. **Open the datasheet alongside your IDE.**
3. **Write each register configuration line manually after verifying its bit positions.**
4. **Calculate your own clock prescalers based on your specific crystal oscillator frequency.**

---

## 8. Pre-Coding Self-Assessment Checklist

Before starting any module in this curriculum or writing firmware, verify that you can answer these questions with confidence:

- [ ] Can you convert `0xA5` into 8-bit binary in your head?
- [ ] Do you know what `(1 << 5)` evaluates to in hexadecimal?
- [ ] Can you explain why connecting an LED directly to a $5\text{ V}$ pin without a resistor destroys the pin?
- [ ] Do you know why a floating digital input pin causes random toggles and high current draw?
- [ ] Can you explain the exact difference between `const char *ptr`, `char * const ptr`, and `volatile char *ptr`?
- [ ] Do you know what happens to a local variable allocated inside a function when that function returns?
- [ ] Do you know what the Program Counter (PC) register stores?
- [ ] Can you explain why `#pragma pack(1)` is critical when sending C structs over serial buses?
- [ ] Do you understand why an omitted `volatile` qualifier breaks hardware register polling loops?
- [ ] Can you locate the pin multiplexing table in your microcontroller's datasheet to check which pin connects to `USART1_TX`?

If you answered **Yes** to all of the above, you are prepared to build robust, production-grade embedded firmware! Proceed to [**Step 1: C & Embedded C**](curriculum/01-c-embedded-c/README.md) or explore the [**PIC Microcontroller Track**](pic-mplab-xc8/README.md).
