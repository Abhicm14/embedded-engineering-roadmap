# 🧠 Embedded Engineering Prerequisites

> **What Every Aspiring Embedded Engineer (Even a 16-Year-Old!) Must Know Before Writing Code**  
> *"If you cannot trace the electrical signal, register bit, and memory address behind your code, you are guessing, not engineering."*

---

## 🚀 Welcome! What Even IS an Embedded System?

Have you ever wondered how a microwave oven knows when to stop heating your pizza? How a drone balances perfectly in mid-air against the wind? Or how your game controller sends your button presses instantly to your console?

All of those devices are powered by **Embedded Systems**!

```
      Traditional Laptop / PC                     Embedded Microcontroller
  ┌───────────────────────────────┐           ┌───────────────────────────────┐
  │ • Giant brain (Multi-GHz CPU) │           │ • Compact chip (16 - 100 MHz) │
  │ • Gigabytes of RAM & SSD      │           │ • Tiny RAM (a few kilobytes!) │
  │ • Sits on a desk all day      │           │ • Touches the real world!     │
  │ • Runs millions of web tabs   │           │ • Controls motors, reads      │
  │ • Isolated from physical pins │           │   sensors, blinks lights      │
  └───────────────────────────────┘           └───────────────────────────────┘
```

Think of a regular PC like a **brilliant professor sitting at a library desk**: super smart, but trapped behind a screen.  
A **microcontroller** is like an **athletic robot with hands and eyes**: it has pins (tiny metal legs) sticking out that can physically touch light switches, listen to heat sensors, spin electric motors, and send radio signals!

When you write embedded code, there is no operating system holding your hand. Your code runs directly on the bare silicon chip. If you write the wrong number to a register, you can physically freeze the chip or even burn out an LED!

That's why this guide exists. We will explain all the foundation concepts in **plain, simple English** with real-world analogies so everything clicks easily.

---

## 🧭 The 4 Pillars You Need Before Writing Code

```
                        ┌────────────────────────────────────────────────────────┐
                        │           EMBEDDED ENGINEERING PREREQUISITES          │
                        └───────────────────────────┬────────────────────────────┘
          ┌─────────────────────────┬───────────────┴───────────────┬─────────────────────────┐
          ▼                         ▼                               ▼                         ▼
 ┌─────────────────┐       ┌─────────────────┐             ┌─────────────────┐       ┌─────────────────┐
 │ 1. NUMBERS &    │       │ 2. ELECTRICITY  │             │ 3. COMPUTER     │       │ 4. EMBEDDED C   │
 │    MATH FOUND.  │       │    & CIRCUITS   │             │    BRAIN (MCU)  │       │    SUPERPOWERS  │
 ├─────────────────┤       ├─────────────────┤             ├─────────────────┤       ├─────────────────┤
 │ • Light switches│       │ • Water pipe law│             │ • Chef's kitchen│       │ • House address │
 │ • Hex nicknames │       │ • Floating pins │             │ • Flash vs RAM  │       │ • Volatile note │
 │ • Car odometer  │       │ • Bus bell cord │             │ • Register maps │       │ • Book packing  │
 │ • Fixed pennies │       │ • Water buckets │             │ • Endianness    │       │ • Pizza doorbell│
 └─────────────────┘       └─────────────────┘             └─────────────────┘       └─────────────────┘
```

---

## 1. Numbers & Math Made Simple

### 1.1 Binary: An 8-Light-Switch Dashboard
Computers don't know numbers; they only know electricity. A wire inside a chip can only have two states:
- **OFF** ($0\text{ Volts}$) $\rightarrow$ We call this `0`.
- **ON** ($3.3\text{ Volts}$ or $5\text{ Volts}$) $\rightarrow$ We call this `1`.

Imagine a wall with **8 light switches in a row**. Each switch is called a **bit**. All 8 switches together make one **byte**:

```
   Bit 7     Bit 6     Bit 5     Bit 4     Bit 3     Bit 2     Bit 1     Bit 0
   [ 1 ]     [ 0 ]     [ 1 ]     [ 1 ]     [ 0 ]     [ 0 ]     [ 0 ]     [ 1 ]
   128's     64's      32's      16's       8's       4's       2's       1's
```

To find what decimal number that is, just add up the switches that are ON:
$$128 + 32 + 16 + 1 = 177$$

| Decimal | Binary (8-bit) | Hexadecimal | What It Means to Hardware |
|:-------:|:--------------:|:-----------:|:--------------------------|
| `0` | `0000 0000` | `0x00` | All 8 pins/switches are OFF |
| `1` | `0000 0001` | `0x01` | Pin 0 is ON, all others OFF |
| `15` | `0000 1111` | `0x0F` | Lower 4 pins are ON |
| `128` | `1000 0000` | `0x80` | Highest pin (Bit 7) is ON |
| `255` | `1111 1111` | `0xFF` | All 8 pins are fully ON |

---

### 1.2 Hexadecimal: The Handy Nickname System
Writing long binary numbers like `0b10111110` is exhausting and easy to misread.  
**Hexadecimal (Hex)** is just a clever shorthand: **every 4 bits gets turned into a single character**.

Because 4 bits can count from 0 to 15, we use `0-9`, and then use letters `A` to `F` for 10 through 15:
- $10 = \text{A}$
- $11 = \text{B}$
- $12 = \text{C}$
- $13 = \text{D}$
- $14 = \text{E}$
- $15 = \text{F}$

```
  Binary:         1 0 1 1       1 1 1 0
  Value:            11            14
  Hex Nickname:      B             E   ──► Written in C as: 0xBE
```
Whenever you see `0x` in C code (like `0x5A` or `0xFF`), that just means: *"Hey compiler, this is a hex number!"*

---

### 1.3 Two's Complement: The Car Odometer Trick
*How does a chip represent negative numbers using only 1s and 0s?*

💡 **The Analogy:** Imagine an old car with a mechanical mileage counter (odometer) that reads `0000`. What happens if you put the car in reverse and drive backwards by 1 mile?  
The wheels roll backwards, and the counter flips to **`9999`**!

In an 8-bit microcontroller, if you have `0000 0000` (zero) and you subtract `1`, the bits roll backwards to **`1111 1111`**!  
So the chip treats `1111 1111` as **$-1$**.

- **Signed 8-bit integer (`int8_t`):** Can hold numbers from $-128$ up to $+127$.
- **Unsigned 8-bit integer (`uint8_t`):** Can hold numbers from $0$ up to $255$.

> [!CAUTION]
> **The Classic Bug:** If you compare a signed number (`int16_t temperature = -5`) with an unsigned number (`uint16_t limit = 10`), the computer secretly converts `-5` into `65531`! Suddenly, the computer thinks $-5$ is bigger than $10$!  
> **Rule:** Never mix signed and unsigned numbers in your `if()` statements.

---

### 1.4 Bitwise Operations: Flipping Dashboard Switches
In embedded systems, you will constantly turn individual pins ON and OFF without messing up the neighboring pins. Memorize these 3 core tricks:

```c
// 1. TURNING A PIN ON (Bitwise OR: |=)
// Imagine flipping Switch #3 UP while leaving all other switches untouched:
PORTB |= (1 << 3);

// 2. TURNING A PIN OFF (Bitwise AND with NOT: &= ~)
// Imagine flipping Switch #3 DOWN while leaving all other switches untouched:
PORTB &= ~(1 << 3);

// 3. TOGGLING A PIN (Bitwise XOR: ^=)
// If Switch #3 was ON, turn it OFF. If it was OFF, turn it ON!
PORTB ^= (1 << 3);

// 4. CHECKING IF A PIN IS ON
if (PORTB & (1 << 3)) {
    // Switch #3 is ON!
}
```

---

### 1.5 Fixed-Point Math: Counting in Pennies Instead of Dollars
Many basic microcontrollers (like the PIC16F or Arduino Mega) **do not have a floating-point math engine**.  
If you write `float temp = 25.34f * 1.5f;`, the chip has to run hundreds of lines of slow software emulation code just to do simple decimal math!

💡 **The Easy Fix:** Count in **pennies** instead of dollars!
- Instead of storing voltage as `2.50` Volts, store it as **$2500$ millivolts** (`uint16_t mv = 2500;`).
- Instead of storing temperature as `23.45` degrees, store it as **$2345$ hundredths of a degree**.
Now everything runs in fast, lightning-quick integer math!

---

## 2. Electricity & Circuits Demystified

You cannot write code for hardware without understanding how electricity actually moves through wires.

### 2.1 Ohm's Law: The Water Pipe Analogy
Think of an electrical wire exactly like a **garden water hose**:

```
   Water Tank (Voltage V) ──► [ Hose (Current I) ] ──► Squeezed Pipe (Resistor R)
```

- **Voltage ($V$, measured in Volts):** Water pressure pushing from the tap.
- **Current ($I$, measured in Amperes):** How fast the water is rushing through the hose.
- **Resistance ($R$, measured in Ohms $\Omega$):** Someone stepping on the hose or pinching it!

The golden rule of electronics is **Ohm's Law**:
$$V = I \times R \quad\iff\quad I = \frac{V}{R} \quad\iff\quad R = \frac{V}{I}$$

#### ❓ Why Does an LED Explode Without a Resistor?
An LED (Light Emitting Diode) is like an open valve with almost zero resistance ($R \approx 0$).  
If you connect an LED directly to a $5\text{V}$ power source without a resistor:
$$I = \frac{5\text{V}}{0\,\Omega} = \text{HUGE CURRENT!}$$
A massive flood of electricity rushes into the tiny LED, burning it into a puff of smoke!  
To protect it, we put a **$220\,\Omega$ to $330\,\Omega$ resistor** in series to pinch the flow down to a safe, gentle trickle ($10\text{ mA}$).

---

### 2.2 Pull-Up Resistors & The "Floating Pin" Trap

💡 **Imagine an input pin is like an ultra-sensitive microphone listening to the room.**  
If you connect a wire to the pin and leave the other end hanging in the air (unconnected), the pin is **"floating"**. Static electricity in the air and radio waves from your Wi-Fi will cause the pin to jump wildly between `0` and `1` a million times a second!

```
     Pull-Up Resistor (The Gentle Rubber Band)
                 +5V Power Rail
                      │
                     ┌┴┐
                     │ │  10k Resistor (Gentle pull)
                     └┬┘
                      ├───► Microcontroller Input Pin (Sees steady HIGH!)
                    ┌─┴─┐
                    │ O │ Pushbutton (Open)
                    └─┬─┘
                      │
                     GND
```

A **Pull-Up Resistor** is like a gentle rubber band holding the switch UP to $5\text{V}$ (HIGH).  
- When nobody is touching the button, the pin stays at a reliable, steady **HIGH (`1`)**.
- When you press the button, your finger easily overpowers the gentle resistor and pulls the pin to **Ground (LOW / `0`)**.
- When you let go, the rubber band pulls it right back up to **HIGH (`1`)**.  
No more ghost signals!

---

### 2.3 Push-Pull vs. Open-Drain (The Bus Bell Cord)

Microcontroller output pins come in two main flavors:

1. **Push-Pull (The Two Bouncers):**
   The pin has two internal electronic switches: one pushes the pin UP to $5\text{V}$, and the other pulls the pin DOWN to $0\text{V}$.  
   *Use case:* Blinking an LED, driving a speaker, or sending high-speed SPI signals.

2. **Open-Drain (The Bus Stop-Request Cord):**
   The pin **only has one switch** that can pull DOWN to Ground ($0\text{V}$). It cannot push UP! When turned off, it just lets go of the wire. An external pull-up resistor pulls the wire high.  
   💡 **Why do we want this?** Imagine the stop-request cord running along the ceiling of a school bus. Any kid sitting anywhere on the bus can yank the cord down to ring the bell. Because nobody is pushing up, **10 kids can yank the cord at the same time and nobody breaks their arm!**  
   This is how **I2C communication** works: dozens of sensor chips can share the exact same two wires without short-circuiting each other!

---

### 2.4 Decoupling Capacitors: The Emergency Water Bucket

Every time your chip executes an instruction or turns on an LED, it gulps a sudden burst of electrical current in a split nanosecond.  
If the main power supply is far away down a long wire, the voltage will dip momentarily (just like when someone flushes a toilet in your house and the shower water pressure suddenly drops!). If the voltage drops too low, the microcontroller will get confused and reboot!

💡 **The Solution:** A **Decoupling Capacitor ($100\text{ nF}$)** is like having a small bucket of water sitting right inside the shower stall. When the chip needs a sudden gulp of power, it drinks instantly from the capacitor right next to its power pins, keeping the power silky smooth and steady!

---

## 3. Inside the Microcontroller's Brain

### 3.1 The Chef's Kitchen Analogy
Inside a microcontroller, memory is organized just like a professional kitchen:

```
  ┌────────────────────────────────────────────────────────────────────────┐
  │ Flash ROM (The Cookbook):                                              │
  │ Permanent recipes. Code stays safe even when the power is turned off!  │
  ├────────────────────────────────────────────────────────────────────────┤
  │ SRAM (The Refrigerator & Prep Table):                                  │
  │ Temporary workspace for cutting vegetables. Variables live here.       │
  │ When you turn off power, everything in SRAM vanishes!                  │
  ├────────────────────────────────────────────────────────────────────────┤
  │ Core Registers (The Cutting Board Right in Front of the Chef):         │
  │ Super-fast storage (R0, R1, etc.). The CPU core can add numbers here   │
  │ in 1 single clock tick!                                                │
  └────────────────────────────────────────────────────────────────────────┘
```

---

### 3.2 What is Memory-Mapped I/O?
*How does writing code in C actually turn on a physical plastic-and-glass LED?*

In a microcontroller, **hardware pins are hooked up to memory addresses!**  
- Address `0x20000000` might be a regular box in SRAM holding a variable.
- But address `0x40020014` is connected by microscopic copper wires directly to the physical silicon switches of **Port A, Pin 5**!

When you write in C:
```c
*(volatile uint32_t *)0x40020014 = (1 << 5);
```
The CPU sends an electrical signal across its memory bus, which physically charges the gate of a transistor on Pin 5, sending 3.3 Volts out of the chip to light up your LED!

---

### 3.3 Endianness: Reading Left-to-Right vs Right-to-Left
Suppose you have a 32-bit number with 4 bytes: `0x12 34 56 78`. How does the chip store those 4 bytes in RAM?

- **Little-Endian (ARM Cortex-M, PIC, x86):** Puts the "little end" (least significant byte `0x78`) first at the lowest address:
  `[0x78] [0x56] [0x34] [0x12]`
- **Big-Endian (Internet networks, CAN protocol):** Puts the "big end" (`0x12`) first:
  `[0x12] [0x34] [0x56] [0x78]`

💡 **Why does this matter?** When sending sensor data over a network or CAN bus, you must make sure both chips agree on whether they are reading backwards or forwards!

---

## 4. Embedded C Superpowers

### 4.1 Pointers: House Addresses on Sticky Notes

A regular variable is like a **box holding a toy**:
```c
uint32_t score = 100; // 'score' is the box, 100 is the toy inside
```

A **Pointer** is a **sticky note with a house address written on it**:
```c
uint32_t *p = &score; // 'p' holds the address of where the box lives in RAM!
```

- `p` gives you the address number (e.g. `0x20000040`).
- `*p` means: *"Walk to that address, open the box, and look at the toy inside!"* (Dereferencing).

---

### 4.2 The `volatile` Keyword: The Lazy Student Trap
Compilers are designed to make code run as fast as possible. If the compiler sees this loop:

```c
// Looking at a hardware sensor register without volatile:
while (BUTTON_PIN == 0) {
    // Wait for button press...
}
```

The compiler optimizer thinks: *"Hey, nobody inside this loop changes `BUTTON_PIN`. Why waste time reading memory every single cycle? I'll just check it once, save it in a CPU register, and loop forever!"*  
Suddenly, your program freezes and never notices when you press the button!

💡 **The Fix:** Adding `volatile` tells the compiler:
> **"HEY! A human finger or external hardware can change this value at any microsecond. Do NOT be lazy—read the real physical hardware register on EVERY SINGLE pass!"**

```c
#define BUTTON_PIN (*(volatile uint8_t *)0x1234)
```

---

### 4.3 Interrupts: The Pizza Delivery Doorbell

Imagine you are playing an intense online video game in your bedroom, and you ordered a pizza.

- **Option A (Polling - The Dumb Way):** Every 5 seconds, you pause your game, walk down the stairs, open the front door, see no pizza, walk back up, and unpause. You will lose your game!
- **Option B (Interrupt - The Smart Way):** You sit and play your game peacefully. **DING-DONG!** The doorbell rings! You pause the game, run downstairs, grab the pizza in 5 seconds, and go right back to playing.

```
  Normal Code:    [ Do Work ] ──► [ Do Work ] ──► [ Do Work ] ──► [ Do Work ]
                                         │ (DING-DONG! Button Pressed!)
                                         ▼
  Interrupt ISR:                  [ Grab Data! ] (Quick 5 microseconds!)
                                         │
  Normal Code Resumes:                   ▼
                                  [ Continue Working... ]
```

An **Interrupt Service Routine (ISR)** is your doorbell handler:
1. **Rule #1:** Keep it lightning fast! (Never put a delay like `__delay_ms()` inside an interrupt!).
2. **Rule #2:** Clear the interrupt flag before leaving, or the doorbell will keep ringing forever!

---

## 5. How to Read a Datasheet Without Your Brain Exploding

When you download a microcontroller datasheet, it might be **800 to 2,000 pages long**. Don't panic! **Nobody reads a datasheet from start to finish.**

Think of a datasheet like an **auto-repair encyclopedia** or a **giant recipe book**:
1. You don't read the whole book to cook an omelette; you flip to the "Eggs" chapter.
2. In a datasheet, if you want to use the UART serial port, flip to the **"USART Chapter"**.
3. Look for the **Numbered Checklist**: manufacturers almost always include a friendly list: *"Step 1: Set baud rate in SPBRG, Step 2: Enable TXEN, Step 3: Write byte to TXREG"*.
4. Convert those steps directly into C code!

👉 Read our full tutorial: [**How to Read Any Datasheet & Write Custom Drivers (`pic-mplab-xc8/datasheet-driver-guide.md`)**](pic-mplab-xc8/datasheet-driver-guide.md).

---

## 6. Workbench Test Equipment: Your Superhero Toolkit

When code doesn't work, don't guess. Use test equipment to "see" the invisible electrons:

```
  ┌────────────────────────────────────────────────────────────────────────┐
  │ 1. Digital Multimeter (DMM):                                           │
  │    Your electrical ruler. Measures voltage (is 5V really 5V?) and      │
  │    beeps for continuity (checks if two wires are touching).            │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 2. USB Logic Analyzer (PulseView):                                     │
  │    Your digital decoder ring. Plugs into USB and decodes binary UART,  │
  │    I2C, and SPI waveforms on your computer screen in real-time!        │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 3. Digital Storage Oscilloscope (DSO):                                 │
  │    Your electrical camera. Takes a picture of the exact analog voltage │
  │    wave shape, showing noise, ringing, and rise times.                 │
  └────────────────────────────────────────────────────────────────────────┘
```

---

## 7. The 7-Step Code Construction Workflow

To make sure you never get stuck staring at a blank screen or blindly copying code, always follow these 7 steps:

```
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ Step 1: Check the schematic: What pin is connected? What voltage rail?  │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 2: Open the datasheet: Find the control registers (TRIS, PORT, CR).│
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 3: Calculate constants: Prescalers, timer ticks, and baud rate.    │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 4: Pick clean variable types: Use uint8_t and uint32_t with stdint.│
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 5: Write the initialization function: Clock -> Pins -> Module.    │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 6: Write the main loop: Non-blocking logic, check flags, no delays.│
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 7: Test in simulator or with logic analyzer to verify behavior.   │
  └─────────────────────────────────────────────────────────────────────────┘
```

---

## 8. Pre-Coding Self-Assessment Checklist

Before jumping into your first project, test yourself with these 10 quick questions:

- [ ] Can you convert `0xA5` into binary? *(Hint: `A = 1010`, `5 = 0101` $\rightarrow$ `10100101`)*
- [ ] What does `(1 << 3)` evaluate to? *(Answer: Binary `00001000` = `8`)*
- [ ] Why do you need a resistor when connecting an LED to $5\text{V}$? *(Answer: To prevent huge current from burning the LED)*
- [ ] Why does a floating input pin behave crazily? *(Answer: It picks up radio/static noise like an antenna)*
- [ ] What is the difference between a variable and a pointer? *(Answer: A variable holds data; a pointer holds a memory address)*
- [ ] What does `volatile` tell the compiler? *(Answer: "Read from real physical memory every single time!")*
- [ ] What happens if an Interrupt Service Routine (ISR) takes too long? *(Answer: It starves the rest of the system of CPU time)*
- [ ] What is the difference between SRAM and Flash memory? *(Answer: Flash keeps code when powered off; SRAM loses data)*
- [ ] What is an Open-Drain pin? *(Answer: A pin that can only pull down to Ground, letting multiple chips share one wire)*
- [ ] Where can you find the exact initialization steps for any peripheral? *(Answer: In its chapter in the silicon datasheet!)*

If you understand these concepts, **you are 100% ready to build real firmware!**  
Proceed to [**Step 1: C & Embedded C**](curriculum/01-c-embedded-c/README.md) or start building PIC drivers in [**`my-code/pic16f877a/`**](my-code/pic16f877a/README.md)!
