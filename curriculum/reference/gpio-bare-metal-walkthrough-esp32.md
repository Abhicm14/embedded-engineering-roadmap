# 📶 Bare-Metal GPIO Walkthrough: Espressif ESP32 (Xtensa Dual-Core)

> **Target Part:** Espressif ESP32 (ESP32-D0WDQ6 / ESP32-WROOM-32 / DevKit v1)  
> **Target Pin:** `GPIO2` (Onboard Blue LED on most DevKit boards / Strapping Pin)  
> **Official Documentation to Open:**  
> 1. **ESP32 Technical Reference Manual (TRM)**:  
>    - *Chapter 4: "IO_MUX and GPIO Matrix (GPIO, IO_MUX)"*  
>    - *Section 4.2: "Architecture (Pad, IO_MUX, GPIO Matrix)"*  
>    - *Section 4.10: "Register Summary"*  
>    - *Section 4.11: "Register Description"*  
> 2. **ESP32 Datasheet**:  
>    - *Section 2.4: "Strapping Pins"*  

---

### 🧠 The Engineering Mindset: The IO_MUX & GPIO Matrix Crossbar

On traditional microcontrollers (like STM32, PIC, and AVR), a physical pin is hardwired to a tiny selection of fixed peripheral functions (e.g. Pin 17 can only be `CCP1` or `RC2`).  
The ESP32 completely redesigns this concept with a **2-stage flexible routing crossbar**:

```
                  Xtensa Dual-Core CPU Core (240 MHz)
                                   │
              [ Internal Peripheral Bus (0x3FF44000) ]
                                   │
                                   ▼
                   ┌───────────────────────────────┐
                   │          GPIO MATRIX          │
                   │ (Full Signal-to-Pad Crossbar) │
                   └───────────────┬───────────────┘
                                   │
                                   ▼
                   ┌───────────────────────────────┐
                   │            IO_MUX             │
                   │ (Pad Function, Pulls, Drive)  │
                   └───────────────┬───────────────┘
                                   │
                                   ▼
                          Physical Pad (GPIO2)
                           [ Onboard Blue LED ]
                            [ 0V <---> 3.3V ]
```

1. **The IO_MUX:** Sits directly between the physical silicon pads and internal circuits. It sets electrical properties (drive strength, pull-up, pull-down) and selects whether the pad connects to high-speed dedicated functions or to the GPIO Matrix.
2. **The GPIO Matrix:** A full crossbar router. Any internal digital peripheral signal (UART TX, SPI Clock, PWM) can be routed to **almost any physical pad** on the chip!
3. **The Rule for Basic GPIO:** To use a pin as a simple bare-metal GPIO, you must configure the **IO_MUX** to select **Function 2 (`MCU_SEL = 2`)**, which hands control of the pad over to the general-purpose GPIO module!

---

## 🧭 Step-by-Step Register Configuration

### Step 1: Open the Technical Reference Manual
1. Open **Chapter 4 ("IO_MUX and GPIO Matrix") of the ESP32 TRM**.
2. Locate the base address:
   * **GPIO Peripheral Base:** **`0x3FF44000`**
   * **IO_MUX Peripheral Base:** **`0x3FF49000`**

---

### Step 2: Configure the IO_MUX Pad Function (`IO_MUX_GPIO2_REG`)
Every physical pad on the ESP32 has its own dedicated IO_MUX configuration register.

* **Register:** `IO_MUX_GPIO2_REG`
* **Address:** `0x3FF49078` (IO_MUX Base `0x3FF49000` + Offset `0x78` for GPIO2)
* **Target Bitfields:**
  * **`MCU_SEL` (Bits `[14:12]`):** Function selection.
    * `0`: Dedicated High-speed function (e.g., HSPI)
    * `2`: **Function 2 = GPIO function** (Connects pad to GPIO controller)
  * **`FUN_DRV` (Bits `[11:10]`):** Drive strength capability.
    * `0`: $\approx 5\text{ mA}$
    * `1`: $\approx 10\text{ mA}$
    * `2`: $\approx 20\text{ mA}$ (Default)
    * `3`: $\approx 40\text{ mA}$ (Maximum drive capability)
  * **`FUN_WPU` (Bit `8`):** Internal weak pull-up enable ($1 = \text{Enabled}$).
  * **`FUN_WPD` (Bit `7`):** Internal weak pull-down enable ($1 = \text{Enabled}$).

```c
// Configure Pad for GPIO mode, 20mA drive, no pull-up/down:
volatile uint32_t *IO_MUX_GPIO2 = (volatile uint32_t *)0x3FF49078;
*IO_MUX_GPIO2 = (2U << 12) | (2U << 10); // MCU_SEL = 2 (GPIO), FUN_DRV = 2 (20mA)
```

---

### Step 3: Configure Direction via the Enable Register (`GPIO_ENABLE_REG`)
On the ESP32, output direction is controlled by the **`GPIO_ENABLE_REG`** register.

* **Register:** `GPIO_ENABLE_REG`
* **Address:** `0x3FF44020`
* **Target Bit:** **Bit 2**
* **Definitions:**
  * `1`: **OUTPUT** (Transistor driver enabled).
  * `0`: **INPUT** (High-impedance listening mode).

#### 💡 Atomic Direction Setting (`GPIO_ENABLE_W1TS_REG`):
Just like STM32's `BSRR`, Espressif provides **W1TS (Write 1 To Set)** and **W1TC (Write 1 To Clear)** registers for thread-safe, atomic operations:
* `GPIO_ENABLE_W1TS_REG` (`0x3FF44024`): Writing `(1 << 2)` sets Bit 2 to `1` without affecting any other pin.
* `GPIO_ENABLE_W1TC_REG` (`0x3FF44028`): Writing `(1 << 2)` clears Bit 2 to `0`.

```c
// Enable GPIO2 output driver atomically:
volatile uint32_t *GPIO_ENABLE_W1TS = (volatile uint32_t *)0x3FF44024;
*GPIO_ENABLE_W1TS = (1U << 2);
```

---

### Step 4: Configure Output Driver Type (`GPIO_PIN2_REG`)
* **Register:** `GPIO_PIN2_REG`
* **Address:** `0x3FF44088` (Offset for Pin 2)
* **Target Bit:** **Bit 2 (`PAD_DRIVER`)**
* **Definitions:**
  * `0`: **Push-Pull** (Actively drives $3.3\text{V}$ and $0\text{V}$).
  * `1`: **Open-Drain** (Only pulls to Ground; needs external pull-up).

```c
volatile uint32_t *GPIO_PIN2 = (volatile uint32_t *)0x3FF44088;
*GPIO_PIN2 &= ~(1U << 2); // Bit 2 = 0 -> Push-Pull
```

---

### Step 5: Atomic Pin Driving (`GPIO_OUT_W1TS` & `GPIO_OUT_W1TC`)
To prevent race conditions between the two CPU cores (PRO_CPU and APP_CPU), **never write to `GPIO_OUT_REG` (`0x3FF44004`) directly!** If Core 0 and Core 1 modify adjacent pins at the same time using read-modify-write, they will corrupt each other's outputs.

Use the hardware **Atomic Set / Clear** registers:
* **`GPIO_OUT_W1TS_REG` (`0x3FF44008`):** *Write 1 to Set*. Writing `(1 << 2)` drives GPIO2 **HIGH** ($3.3\text{V}$).
* **`GPIO_OUT_W1TC_REG` (`0x3FF4400C`):** *Write 1 to Clear*. Writing `(1 << 2)` drives GPIO2 **LOW** ($0\text{V}$).

```c
volatile uint32_t *GPIO_OUT_W1TS = (volatile uint32_t *)0x3FF44008;
volatile uint32_t *GPIO_OUT_W1TC = (volatile uint32_t *)0x3FF4400C;

// Turn LED ON (Set High to 3.3V)
*GPIO_OUT_W1TS = (1U << 2);

// Turn LED OFF (Clear Low to 0V)
*GPIO_OUT_W1TC = (1U << 2);
```

---

### Step 6: Reading the Pin Level (`GPIO_IN_REG`)
To read incoming logic voltages on any pin from GPIO 0 to 31:
* **Register:** `GPIO_IN_REG`
* **Address:** `0x3FF4403C`
```c
volatile uint32_t *GPIO_IN = (volatile uint32_t *)0x3FF4403C;
uint32_t state = (*GPIO_IN & (1U << 2)) ? 1 : 0;
```

---

### Step 7: ⚠️ CRITICAL GOTCHA: Strapping Pins (Boot Modes)
The ESP32 features **5 Strapping Pins**: **`GPIO0`, `GPIO2`, `GPIO5`, `GPIO12`, `GPIO15`**.  
During power-on reset (while the `CHIP_PU` pin transitions from Low to High), the internal ROM bootloader samples the physical voltage on these pins:

| Strapping Pin | Voltage at Reset | Meaning to ESP32 ROM Bootloader |
| :--- | :---: | :--- |
| **`GPIO0`** | `HIGH` (3.3V) | Boot from SPI Flash (Normal Execution). |
| **`GPIO0`** | `LOW` (0V) | Enter Serial UART Flashing Mode (Download Code). |
| **`GPIO2`** | `LOW` or Floating | **Mandatory during programming:** Must not be pulled high if entering download mode! |
| **`GPIO12` (`MTDI`)** | `HIGH` (3.3V) | Sets Flash voltage to **$1.8\text{V}$**. *(Warning: If your module has a $3.3\text{V}$ Flash chip, pulling this high can brick the flash!)* |
| **`GPIO15` (`MTDO`)** | `LOW` (0V) | Silences ROM boot logging messages. |

**The Golden Rule:** When connecting external hardware, avoid using Strapping Pins unless necessary, or ensure your external sensors do not hold those pins Low or High during boot!

---

## 📊 Complete ESP32 GPIO2 Register Map Summary

| Register | Address | Target Bits | Value | Engineering Purpose |
| :--- | :---: | :---: | :---: | :--- |
| **`IO_MUX_GPIO2_REG`** | `0x3FF49078` | `[14:12]` (`MCU_SEL`) | `2` (`010b`) | Connects pad to GPIO Matrix / GPIO controller. |
| **`IO_MUX_GPIO2_REG`** | `0x3FF49078` | `[11:10]` (`FUN_DRV`) | `2` (`10b`)  | Selects $20\text{ mA}$ standard drive strength. |
| **`GPIO_ENABLE_W1TS`**  | `0x3FF44024` | `[2]` | `1` | **Atomic Output Enable:** Enables driver for GPIO2. |
| **`GPIO_PIN2_REG`**    | `0x3FF44088` | `[2]` (`PAD_DRIVER`) | `0` | Configures Push-Pull output mode. |
| **`GPIO_OUT_W1TS_REG`**| `0x3FF44008` | `[2]` | `1` | **Atomic Bit Set:** Drives GPIO2 High ($3.3\text{V}$). |
| **`GPIO_OUT_W1TC_REG`**| `0x3FF4400C` | `[2]` | `1` | **Atomic Bit Clear:** Drives GPIO2 Low ($0\text{V}$). |
| **`GPIO_IN_REG`**      | `0x3FF4403C` | `[2]` | Read | Read physical sampled voltage on GPIO2. |

---

## 💻 Full Compilable Bare-Metal C Implementation

This code runs under ESP-IDF or pure FreeRTOS on the ESP32 without calling the `gpio_set_direction()` or `gpio_set_level()` driver wrapper libraries:

```c
/**
 * @file main.c
 * @brief ESP32 Pure Register-by-Register GPIO2 Driver
 */
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Hardware Register Definitions from ESP32 TRM
#define DR_REG_GPIO_BASE        (0x3FF44000UL)
#define DR_REG_IO_MUX_BASE      (0x3FF49000UL)

#define IO_MUX_GPIO2_REG        (*(volatile uint32_t *)(DR_REG_IO_MUX_BASE + 0x78))
#define GPIO_ENABLE_W1TS_REG    (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x24))
#define GPIO_PIN2_REG           (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x88))
#define GPIO_OUT_W1TS_REG       (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x08))
#define GPIO_OUT_W1TC_REG       (*(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x0C))

void app_main(void) {
    // Step 2: Configure IO_MUX for GPIO2
    // MCU_SEL = 2 (Function 2: GPIO), FUN_DRV = 2 (20mA drive strength)
    IO_MUX_GPIO2_REG = (2U << 12) | (2U << 10);

    // Step 3: Configure Output Type as Push-Pull (Bit 2 = 0)
    GPIO_PIN2_REG &= ~(1U << 2);

    // Step 4: Atomically Enable Output Driver for GPIO2 (Bit 2 in W1TS)
    GPIO_ENABLE_W1TS_REG = (1U << 2);

    // Step 5: Superloop - Atomic LED Blink using W1TS and W1TC
    while (1) {
        // Drive GPIO2 HIGH (3.3V)
        GPIO_OUT_W1TS_REG = (1U << 2);
        vTaskDelay(pdMS_TO_TICKS(500));

        // Drive GPIO2 LOW (0V)
        GPIO_OUT_W1TC_REG = (1U << 2);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

---

## 🔍 "Why Is Nothing Happening?" Troubleshooting Checklist

1. **Did you configure the `IO_MUX` function to `MCU_SEL = 2`?**  
   *Symptom:* Writing to `GPIO_ENABLE` and `GPIO_OUT_W1TS` changes nothing on the pin. If `MCU_SEL` is not set to `2`, the pad is still hooked up to the default internal hardware peripheral!
2. **Did you accidentally tie a strapping pin to the wrong rail?**  
   *Symptom:* ESP32 fails to reboot after programming or spits out `rst:0x10 (RTCWDT_RTC_RESET),boot:0x13 (WAITING_FOR_DOWNLOAD)`. Ensure `GPIO0` and `GPIO2` are allowed to float high/low according to their normal boot requirements.
3. **Are you trying to output on GPIO 34, 35, 36, or 39?**  
   *CRITICAL SILICON LIMITATION:* **GPIO 34–39 are INPUT ONLY!** They do not have output drivers, pull-ups, or pull-downs on the silicon die. Attempting to set them as outputs will fail completely.
