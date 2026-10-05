# 📶 ESP32: Bare-Metal GPIO Register Walkthrough

> **Looking for the deep register-by-register breakdown?**  
> We have a comprehensive, line-by-line guide explaining the ESP32 IO_MUX, GPIO Matrix, atomic W1TS/W1TC registers, and strapping pin hazards:

👉 [**📘 Open the Full ESP32 Bare-Metal GPIO Walkthrough (`curriculum/reference/gpio-bare-metal-walkthrough-esp32.md`)**](../../curriculum/reference/gpio-bare-metal-walkthrough-esp32.md)

---

### 🚀 Quick Register Blueprint for ESP32 GPIO2

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define IO_MUX_GPIO2_REG        (*(volatile uint32_t *)0x3FF49078)
#define GPIO_ENABLE_W1TS_REG    (*(volatile uint32_t *)0x3FF44024)
#define GPIO_OUT_W1TS_REG       (*(volatile uint32_t *)0x3FF44008)
#define GPIO_OUT_W1TC_REG       (*(volatile uint32_t *)0x3FF4400C)

void app_main(void) {
    // 1. Select GPIO Function in IO_MUX (MCU_SEL = 2)
    IO_MUX_GPIO2_REG = (2U << 12) | (2U << 10);

    // 2. Atomically enable output driver
    GPIO_ENABLE_W1TS_REG = (1U << 2);

    while (1) {
        GPIO_OUT_W1TS_REG = (1U << 2); // Set HIGH (3.3V)
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIO_OUT_W1TC_REG = (1U << 2); // Clear LOW (0V)
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
```

| Register | Address | Target Bit | Value | What It Does |
| :--- | :---: | :---: | :---: | :--- |
| **`IO_MUX_GPIO2_REG`** | `0x3FF49078` | `[14:12]` (`MCU_SEL`) | `2` | Routes physical pad to GPIO Matrix. |
| **`GPIO_ENABLE_W1TS`**  | `0x3FF44024` | `[2]` | `1` | Atomically enables output driver for GPIO2. |
| **`GPIO_OUT_W1TS`**    | `0x3FF44008` | `[2]` | `1` | Atomically drives pin High ($3.3\text{V}$). |
| **`GPIO_OUT_W1TC`**    | `0x3FF4400C` | `[2]` | `1` | Atomically drives pin Low ($0\text{V}$). |
