# ⚡ ESP32 Dual-Core & FreeRTOS Custom Workspace

> **Silicon Platform:** Espressif Systems ESP32 (Tensilica Xtensa Dual-Core 32-bit LX6, 240 MHz, 520 KB SRAM, Wi-Fi + BLE)  
> **Simulation First:** [**Wokwi ESP32 Simulator**](https://wokwi.com/projects/new/esp32)

---

## 🎯 Dual-Core & FreeRTOS Architecture to Master

The ESP32 is the industry powerhouse for connected IoT and multi-threaded embedded firmware:
1. **Symmetric Multi-Processing (SMP) FreeRTOS:**
   - **Core 0 (Protocol CPU):** Handles Wi-Fi, Bluetooth stacks, and background networking.
   - **Core 1 (Application CPU):** Runs real-time sensor loops, control algorithms, and UI updates.
   - Core pinning: `xTaskCreatePinnedToCore(..., tskNO_AFFINITY / 0 / 1)`.
2. **Inter-Process Communication (IPC):**
   - FreeRTOS Queues (`xQueueSend`, `xQueueReceive`) for thread-safe message passing between cores.
   - Binary & Counting Semaphores for ISR-to-Task synchronization.
   - Mutexes with Priority Inheritance (`xSemaphoreCreateMutex`) to avoid priority inversion.
3. **Advanced Peripherals:**
   - 12-bit Successive Approximation ADC (ADC1 & ADC2, 18 channels).
   - LED Control (LEDC) hardware PWM with 16 independent channels.
   - 3 Hardware UARTs, 2 Hardware I2C buses, 3 Hardware SPI buses.
   - Hardware Capacitive Touch sensors.
   - Hardware Cryptographic Accelerators (AES, SHA-2, RSA, ECC).
4. **Power Management:**
   - Active mode $\rightarrow$ Modem Sleep $\rightarrow$ Light Sleep $\rightarrow$ **Deep Sleep ($\approx 10\,\mu\text{A}$)**.
   - Wakeup sources: Timer, RTC GPIO pin, Touch pin, ULP co-processor.

---

## 💻 Simulation Before Hardware Flashing

Before uploading to your physical ESP32 board:
1. Open the [**Wokwi ESP32 Simulator**](https://wokwi.com/projects/new/esp32).
2. Wire up OLED displays (SSD1306), sensors (DHT22, BMP280, MPU6050), and pushbuttons.
3. Test FreeRTOS multi-tasking and queue passing in the simulator console.
4. Simulate Wi-Fi connectivity with the virtual `Wokwi-GUEST` access point.
5. Use the virtual **8-Channel Logic Analyzer** in Wokwi to capture and download VCD files for PulseView!
6. Save your sketches into [`sketches/`](sketches/) and upload to physical hardware with ESP-IDF or Arduino IDE.

---

## 📁 Sketches Directory
Place your ESP32 FreeRTOS and IoT projects in:
👉 [`sketches/`](sketches/)
