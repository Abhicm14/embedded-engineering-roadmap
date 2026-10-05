# 🦾 Bare-Metal GPIO Walkthrough: STM32F4 (ARM Cortex-M4)

> **Target Part:** STM32F411xC/E & STM32F401xB/C (e.g. "Black Pill" or Nucleo-F411RE)  
> **Target Pin:** `PB12` (Port B, Pin 12)  
> **Official Documentation to Open:**  
> 1. **ST Reference Manual RM0383** (STM32F411) or **RM0368** (STM32F401):  
>    - *Chapter 6: Reset and clock control (RCC)*  
>    - *Chapter 8: General-purpose I/Os (GPIO)*  
> 2. **STM32F411xC/xE Datasheet DS10314**:  
>    - *Chapter 4: Pinouts and pin description (Alternate function mapping table)*  

---

### 🧠 The Engineering Mindset: From Silicon to Voltage

When using vendor code generators (like STM32CubeMX), a single click hides half a dozen silicon hardware transactions. A true embedded engineer never guesses what the silicon is doing. Every microcontroller output pin is driven by an internal electronic circuit hooked up to high-speed internal buses:

```
               ARM Cortex-M4 CPU Core (Up to 100 MHz)
                                 │
                     [ 32-bit AHB1 System Bus ]
                                 │
       ┌─────────────────────────┴─────────────────────────┐
       ▼                                                   ▼
┌──────────────┐                                   ┌──────────────┐
│  RCC Engine  │                                   │ GPIOB Engine │
│ (Clock Gate) │                                   │ (Registers)  │
└──────┬───────┘                                   └──────┬───────┘
       │                                                  │
       │ RCC_AHB1ENR[Bit 1] = 1 (Enable Clock)            │
       └─────────────────────────────────────────────────►│ Clock Active!
                                                          │
                                         ┌────────────────┴────────────────┐
                                         ▼                                 ▼
                                  MODER = 01 (Output)            BSRR = Atomic Set/Reset
                                  OTYPER = 0 (Push-Pull)                   │
                                  OSPEEDR = 01 (Medium)                    │
                                  PUPDR = 00 (No Pull)                     ▼
                                                                  Physical Pin PB12
                                                                  [ 0V <---> 3.3V ]
```

---

## 🧭 Step-by-Step Register Configuration

### Step 1: Open the Reference Manual & Pinout Table
1. Open **RM0383 Section 2.2 ("Memory Map")**:  
   Notice the bus topology:
   - Base address of `GPIOB`: **`0x4002 0400`**
   - Bus master: **AHB1** (Advanced High-performance Bus 1)
2. Open the **Datasheet Pinout Table**:  
   Verify that `PB12` exists on your package (e.g. Pin 25 on UFQFPN48 Black Pill). Ensure it is not multiplexed with dedicated crystal oscillator pins or unbonded pads.

---

### Step 2: Enable the Peripheral Bus Clock (RCC)
On ARM Cortex-M microcontrollers, **all peripheral clocks are disabled by default at power-on to conserve electrical energy**.  
If you attempt to write to any `GPIOB` register while its clock is disabled, the peripheral bus bridge ignores the write completely (or may trigger a hardware `BusFault`).

* **Register:** `RCC_AHB1ENR` (*RCC AHB1 Peripheral Clock Enable Register*)
* **Address:** `0x4002 3800` (RCC Base) + `0x30` (Offset) = `0x4002 3830`
* **Target Bit:** **Bit 1 (`GPIOBEN`)**
  * `0`: GPIOB clock disabled (default)
  * `1`: GPIOB clock enabled
* **Action:**
  ```c
  // Set Bit 1 of RCC_AHB1ENR to 1
  RCC->AHB1ENR |= (1 << 1);
  ```

---

### Step 3: Configure the Pin Direction Mode (`GPIOx_MODER`)
Each physical GPIO pin requires **2 configuration bits** in `MODER` to determine its fundamental operational state.

* **Register:** `GPIOB_MODER` (*GPIO Port Mode Register*)
* **Address:** `0x4002 0400` + `0x00` = `0x4002 0400`
* **Target Bits:** Bits `[25:24]` (corresponding to Pin 12, since $12 \times 2 = 24$)
* **Bit Definitions:**
  * `00`: Input mode (reset default)
  * `01`: General-purpose output mode
  * `10`: Alternate function mode (UART, SPI, I2C, Timers)
  * `11`: Analog mode (ADC input or low-power state)
* **Action:** To safely configure bits `[25:24]` to `01`, always **clear both bits first** before setting:
  ```c
  GPIOB->MODER &= ~(3U << (12 * 2)); // Clear bits 25:24 to 00
  GPIOB->MODER |=  (1U << (12 * 2)); // Set bits 25:24 to 01 (General-purpose Output)
  ```

---

### Step 4: Configure the Output Driver Type (`GPIOx_OTYPER`)
Microcontroller output stages feature two complementary internal MOSFETs (P-channel to $3.3\text{V}$, N-channel to Ground).

* **Register:** `GPIOB_OTYPER` (*GPIO Port Output Type Register*)
* **Address:** `0x4002 0400` + `0x04` = `0x4002 0404`
* **Target Bit:** **Bit 12 (`OT12`)**
* **Bit Definitions:**
  * `0`: **Push-Pull** (The pin actively drives both High to $3.3\text{V}$ and Low to $0\text{V}$).  
    *Use case:* LEDs, digital signaling, SPI lines, chip selects.
  * `1`: **Open-Drain** (The pin only has an N-channel transistor to Ground; it can pull Low to $0\text{V}$, but when High, it enters high-impedance and disconnects, relying on an external pull-up resistor).  
    *Why does Open-Drain exist?* For shared multi-device communication buses like **I2C**, where multiple chips share a single wire. If two push-pull pins drove High and Low simultaneously, they would create a short circuit and burn the silicon! Open-drain enables a "wired-AND" bus where anyone can pull down safely.
* **Action:**
  ```c
  GPIOB->OTYPER &= ~(1U << 12); // Bit 12 = 0 -> Push-Pull
  ```

---

### Step 5: Configure Slew Rate & Edge Speed (`GPIOx_OSPEEDR`)
Output speed controls the rise and fall time (slew rate) of the output voltage transitions.

* **Register:** `GPIOB_OSPEEDR` (*GPIO Port Output Speed Register*)
* **Address:** `0x4002 0400` + `0x08` = `0x4002 0408`
* **Target Bits:** Bits `[25:24]` (Pin 12)
* **Bit Definitions:**
  * `00`: Low speed (2 MHz)
  * `01`: Medium speed (12.5 MHz to 25 MHz)
  * `10`: Fast speed (25 MHz to 50 MHz)
  * `11`: High speed (50 MHz to 100 MHz)
* **Engineering Rationale:**  
  *Why not always pick High Speed?* Faster switching edges produce sharp voltage spikes ($\frac{di}{dt}$ and $\frac{dv}{dt}$). On circuit board traces, sharp edges create high-frequency **Electromagnetic Interference (EMI)**, inductive ringing, and crosstalk into adjacent sensor lines!  
  For an LED or low-frequency switch, **Low or Medium speed** is optimal. High speed is strictly reserved for high-frequency buses ($>20\text{ MHz}$ SPI, SDIO).
* **Action:**
  ```c
  GPIOB->OSPEEDR &= ~(3U << (12 * 2)); // Clear bits 25:24
  GPIOB->OSPEEDR |=  (1U << (12 * 2)); // 01 -> Medium speed
  ```

---

### Step 6: Configure Internal Pull-Up/Pull-Down (`GPIOx_PUPDR`)
* **Register:** `GPIOB_PUPDR` (*GPIO Port Pull-Up/Pull-Down Register*)
* **Address:** `0x4002 0400` + `0x0C` = `0x4002 040C`
* **Target Bits:** Bits `[25:24]` (Pin 12)
* **Bit Definitions:**
  * `00`: No pull-up, no pull-down (Floating / High-Z)
  * `01`: Pull-up (Activates internal $\approx 40\text{ k}\Omega$ resistor to $3.3\text{V}$)
  * `10`: Pull-down (Activates internal $\approx 40\text{ k}\Omega$ resistor to $0\text{V}$)
  * `11`: Reserved
* **Action:** For push-pull outputs, the pin actively drives both rails, so internal pull resistors are disabled to prevent unnecessary leakage current:
  ```c
  GPIOB->PUPDR &= ~(3U << (12 * 2)); // 00 -> No Pull
  ```

---

### Step 7: Drive the Pin Atomically (`GPIOx_BSRR`)
To change the output level, you have two register options: `ODR` and `BSRR`.

#### ⚠️ The Read-Modify-Write Danger of `ODR`:
If you write:
```c
GPIOB->ODR |= (1 << 12); // Danger!
```
The CPU must execute **3 separate assembly instructions**:
1. `LDR` (Load `ODR` from RAM into CPU register `r0`)
2. `ORR` (Set bit 12 in `r0`)
3. `STR` (Store `r0` back into `ODR`)

If a high-priority interrupt fires between step 1 and step 3 and modifies Pin 13 on the same port, the main thread will overwrite and corrupt the interrupt's modification when it stores `r0`!

#### 💡 The Solution: Atomic Writes via `BSRR`:
`BSRR` (*Bit Set/Reset Register*) is a 32-bit register designed specifically for atomic hardware writes:
* **Lower 16 bits (`[15:0]` - `BSx`):** Writing `1` sets the pin High. Writing `0` does nothing.
* **Upper 16 bits (`[31:16]` - `BRx`):** Writing `1` resets the pin Low. Writing `0` does nothing.

A single write instruction (`STR`) updates the exact pin in 1 CPU cycle without touching any other pin!

```c
// Turn PB12 ON (Set High to 3.3V)
GPIOB->BSRR = (1U << 12);

// Turn PB12 OFF (Reset Low to 0V)
GPIOB->BSRR = (1U << (12 + 16)); // Bit 28
```

---

### Step 8: Reading the Physical Pin Logic Level (`GPIOx_IDR`)
To read whether the pin is physically High or Low:
* **Register:** `GPIOB_IDR` (*GPIO Port Input Data Register*, Offset `0x10`)
* **Bit 12:** Reflects the voltage sampled by the input Schmitt trigger.
```c
uint32_t pin_state = (GPIOB->IDR & (1U << 12)) ? 1 : 0;
```

---

## 📊 Complete STM32F4 PB12 Register Map Summary

| Register | Offset | Target Bits | Binary Value | Hex Equivalent | Engineering Purpose |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **`RCC_AHB1ENR`** | `0x30` | `[1]` | `1` | `0x00000002` | Turn on the AHB1 bus clock to GPIOB. |
| **`GPIOB_MODER`** | `0x00` | `[25:24]` | `01` | `0x01000000` | Configure PB12 as General-Purpose Output. |
| **`GPIOB_OTYPER`** | `0x04` | `[12]` | `0` | `0x00000000` | Select Push-Pull output stage (drives 3.3V & 0V). |
| **`GPIOB_OSPEEDR`**| `0x08` | `[25:24]` | `01` | `0x01000000` | Medium speed (prevents EMI and line ringing). |
| **`GPIOB_PUPDR`** | `0x0C` | `[25:24]` | `00` | `0x00000000` | No pull-up / pull-down resistors needed. |
| **`GPIOB_BSRR`** | `0x18` | `[12]` | `1` | `0x00001000` | Atomic Set PB12 High ($3.3\text{V}$). |
| **`GPIOB_BSRR`** | `0x18` | `[28]` | `1` | `0x10000000` | Atomic Reset PB12 Low ($0\text{V}$). |

---

## 💻 Full Compilable Bare-Metal C Implementation

This code has zero external library dependencies. It compiles directly with `arm-none-eabi-gcc`:

```c
/**
 * @file main.c
 * @brief STM32F411 Bare-Metal PB12 Register-by-Register GPIO Driver
 */
#include <stdint.h>

// 1. Base Addresses from RM0383 Memory Map
#define PERIPH_BASE         (0x40000000UL)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)

#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOB_BASE          (AHB1PERIPH_BASE + 0x0400UL)

// 2. Hardware Register Structures
typedef struct {
    volatile uint32_t MODER;    // 0x00: Mode register
    volatile uint32_t OTYPER;   // 0x04: Output type register
    volatile uint32_t OSPEEDR;  // 0x08: Output speed register
    volatile uint32_t PUPDR;    // 0x0C: Pull-up/pull-down register
    volatile uint32_t IDR;      // 0x10: Input data register
    volatile uint32_t ODR;      // 0x14: Output data register
    volatile uint32_t BSRR;     // 0x18: Bit set/reset register
    volatile uint32_t LCKR;     // 0x1C: Configuration lock register
    volatile uint32_t AFR[2];   // 0x20-0x24: Alternate function registers
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t Reserved0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t Reserved1[2];
    volatile uint32_t AHB1ENR;   // 0x30: AHB1 Peripheral Clock Enable Register
} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *)RCC_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)

// Simple delay loop (at 16 MHz default internal clock, ~500ms)
static void delay_cycles(volatile uint32_t count) {
    while (count--) {
        __asm__("nop");
    }
}

int main(void) {
    // Step 2: Enable AHB1 bus clock for GPIOB
    RCC->AHB1ENR |= (1U << 1);

    // Short stabilization delay after clock gate release (2 bus cycles)
    __asm__("nop");
    __asm__("nop");

    // Step 3: Configure PB12 as General-Purpose Output (MODER[25:24] = 01)
    GPIOB->MODER &= ~(3U << (12 * 2));
    GPIOB->MODER |=  (1U << (12 * 2));

    // Step 4: Configure Output Type as Push-Pull (OTYPER[12] = 0)
    GPIOB->OTYPER &= ~(1U << 12);

    // Step 5: Configure Output Speed as Medium (OSPEEDR[25:24] = 01)
    GPIOB->OSPEEDR &= ~(3U << (12 * 2));
    GPIOB->OSPEEDR |=  (1U << (12 * 2));

    // Step 6: Configure No Pull-Up / No Pull-Down (PUPDR[25:24] = 00)
    GPIOB->PUPDR &= ~(3U << (12 * 2));

    // Step 7: Superloop - Atomic Pin Toggle via BSRR
    while (1) {
        // Set PB12 High (Charge output to 3.3V)
        GPIOB->BSRR = (1U << 12);
        delay_cycles(500000);

        // Reset PB12 Low (Drain output to 0V)
        GPIOB->BSRR = (1U << (12 + 16));
        delay_cycles(500000);
    }

    return 0;
}
```

---

## 🔍 "Why Is Nothing Happening?" Troubleshooting Checklist

If your LED is not toggling or the multimeter reads $0\text{V}$:

1. **Did you enable the clock in `RCC_AHB1ENR` BEFORE configuring GPIOB?**  
   *Symptom:* `GPIOB->MODER` remains `0x00000000` in the debugger. Writing to un-clocked registers is silently ignored!
2. **Did you clear `MODER` bits before setting them?**  
   *Symptom:* If the pin was in reset mode (`00`) or analog mode (`11`), doing `|= (1 << 24)` on `11` results in `11` (Analog mode), which completely disconnects the digital output driver! Always do `&= ~(3 << 24)` first.
3. **Did you write to the upper 16 bits of `BSRR` to reset?**  
   *Symptom:* Writing `(0 << 12)` to `BSRR` does **not** clear the pin! Writing zero to `BSRR` does nothing. To reset, you must write `(1 << (12 + 16))`.
4. **Is the pin shared with a reserved boot or debug function?**  
   *Check:* `PA13` and `PA14` are SWD debug pins (`SWDIO`/`SWCLK`). `PB2` is `BOOT1`. `PB12` is completely safe.
