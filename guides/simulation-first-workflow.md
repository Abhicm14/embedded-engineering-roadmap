# 🖥️ The Simulation-First Embedded Workflow Guide

> **How to Simulate PIC, Arduino Mega 2560, ESP8266, ESP32, and STM32 in Software Before Touching Physical Hardware**  
> *"Professional firmware engineers test algorithms and drivers in simulation first. It prevents burned silicon, eliminates hardware ambiguity, and provides instant instrument telemetry."*

---

## 🧭 Why Adopt a "Simulation-First" Workflow?

1. **Hardware Safety:** You cannot fry a virtual pin or destroy a microcontroller with an incorrect pull-up or short-circuit in a simulator.
2. **Zero Financial or Shipping Friction:** You can master ARM Cortex-M4 startup code, FreeRTOS dual-core task synchronization, and automotive CAN protocols today without waiting for circuit boards to arrive.
3. **Perfect Diagnostic Visibility:** Simulators allow you to pause CPU execution, inspect physical register flip-flops, export digital waveforms directly to **PulseView**, and hook up GDB step-debugging with zero cabling issues.

> 💡 **The 16-Year-Old Plain English Analogy:**  
> Before airline pilots fly a real Boeing 747 airplane with passengers, they spend hundreds of hours inside a **Flight Simulator**.  
> If they make a mistake in the flight simulator, they just hit the **"Restart"** button! No planes crash and nobody gets hurt.  
> In embedded systems, software simulators (like PICSimLab and Wokwi) are your **Flight Simulator**! You can wire up virtual circuits, test your code, and make all your mistakes safely on your laptop screen before plugging in real physical hardware!

---

## 📊 Microcontroller Simulation Ecosystem Matrix

| Microcontroller | Core Architecture | Recommended Simulator | What You Can Simulate in Real-Time |
| :--- | :--- | :--- | :--- |
| **Microchip PIC16F877A** | 8-bit Harvard RISC | [**PICSimLab**](https://lcgamboa.github.io/picsimlab_docs/) | LEDs, pushbuttons, $4 \times 4$ keypad, 16x2 LCD, 10-bit ADC, PWM fan/buzzer, UART serial, I2C RTC/EEPROM. |
| **Arduino Mega 2560** | 8-bit AVR (ATmega2560) | [**Wokwi Mega 2560**](https://wokwi.com/projects/new/arduino-mega) | 54 digital I/O, 4 hardware UARTs, 6 timers, 16 ADC channels, servo motors, I2C sensors, SPI LCDs. |
| **Espressif ESP8266** | 32-bit Xtensa L106 | [**Wokwi ESP8266**](https://wokwi.com/projects/new/esp8266) | Wi-Fi Station & AP mode, HTTP REST clients, MQTT brokers, TCP/UDP sockets, NTP time synchronization. |
| **Espressif ESP32** | 32-bit Xtensa Dual-Core | [**Wokwi ESP32**](https://wokwi.com/projects/new/esp32) | FreeRTOS SMP dual-core, queues, semaphores, mutexes, BLE beacons, Wi-Fi, virtual 8-channel logic analyzer. |
| **STM32 (Cortex-M4)** | 32-bit ARM Cortex-M4 | [**Wokwi STM32**](https://wokwi.com/) / [**QEMU**](https://www.qemu.org/) | Bare-metal `startup.c`, linker scripts, vector table, RCC clock trees, SysTick, UART CLI, GDB remote debugging. |

---

## 1. Microchip PIC16F877A Simulation (PICSimLab)

### Workflow:
1. **Develop & Build:** Open your project in **MPLAB X IDE**. Write code using the **XC8 compiler**. Click **Clean and Build** to generate the target `.hex` file.
2. **Launch PICSimLab:**
   - Go to `Board` $\rightarrow$ Select `Board 1: PICGenios` (or `Breadboard`).
   - Processor: Select `PIC16F877A`.
   - Clock Frequency: Set to `4.0 MHz` (matches `#define _XTAL_FREQ 4000000UL`).
3. **Load Firmware:**
   - Click `File` $\rightarrow$ `Load Hex`.
   - Browse to `<project_dir>/dist/default/production/<project_name>.production.hex`.
4. **Interactive Hardware Debugging:**
   - **LEDs & Buttons:** Push switches connected to `PORTB` and observe output LEDs.
   - **Potentiometer & ADC:** Turn the onboard POT knob to vary voltage on `RA0/AN0` and observe the ADC output.
   - **Oscilloscope:** Open `Tools` $\rightarrow$ `Oscilloscope` and connect probe to `RC2` to verify CCP1 hardware PWM frequency and duty cycle.
   - **Virtual Serial Port:** Configure `Modules` $\rightarrow$ `Serial Port` to connect to a virtual COM loopback (e.g. `com0com`) and read UART CLI data in PuTTY.

---

## 2. Arduino Mega 2560 Simulation (Wokwi)

The Mega 2560 gives you 54 I/O pins and 4 independent hardware serial ports.

### Workflow:
1. Open [**Wokwi Arduino Mega 2560**](https://wokwi.com/projects/new/arduino-mega).
2. **Add Components:** Click the `+` button in the diagram editor to add LEDs, resistors, an I2C 1602 LCD, an ultrasonic distance sensor (HC-SR04), or pushbuttons.
3. **Write Bare-Metal AVR Code or Arduino C++:**
   - You can test high-speed bare-metal register manipulation:
     ```c
     void setup() {
         // Configure Pin 13 (PORTB Bit 7) as output directly using registers:
         DDRB |= (1 << DDB7);
     }

     void loop() {
         PORTB ^= (1 << PORTB7); // Fast atomic toggle
         delay(500);
     }
     ```
4. **Multi-UART Testing:** Route `Serial1.print()` to a virtual GPS module or Bluetooth HC-05 receiver in Wokwi without buying physical sensor modules!

---

## 3. ESP8266 Wi-Fi & IoT Simulation (Wokwi)

Test IoT networking, REST API requests, and MQTT telemetry without exposing your home Wi-Fi password.

### Workflow:
1. Open [**Wokwi ESP8266**](https://wokwi.com/projects/new/esp8266).
2. **Virtual Wi-Fi Connection:**
   In your sketch, connect to Wokwi's public simulated gateway:
   ```cpp
   #include <ESP8266WiFi.h>

   const char* ssid     = "Wokwi-GUEST";
   const char* password = ""; // No password required!

   void setup() {
       Serial.begin(115200);
       WiFi.begin(ssid, password);
       while (WiFi.status() != WL_CONNECTED) {
           delay(250);
           Serial.print(".");
       }
       Serial.println("\nWiFi Connected! IP Address: ");
       Serial.println(WiFi.localIP());
   }
   ```
3. **HTTP Client & Cloud Ingestion:**
   Simulate fetching weather data from public JSON APIs or posting sensor telemetry to an online test endpoint (e.g. `httpbin.org`).

---

## 4. ESP32 Dual-Core & FreeRTOS Simulation (Wokwi)

The ESP32 is a dual-core powerhouse. Wokwi accurately simulates both Xtensa cores and FreeRTOS task scheduling.

### Workflow:
1. Open [**Wokwi ESP32**](https://wokwi.com/projects/new/esp32).
2. **Dual-Core Task Pinning Test:**
   ```cpp
   void Core0_Network_Task(void *pvParameters) {
       for (;;) {
           Serial.printf("Network task running on Core %d\n", xPortGetCoreID());
           vTaskDelay(pdMS_TO_TICKS(1000));
       }
   }

   void Core1_Sensor_Task(void *pvParameters) {
       for (;;) {
           Serial.printf("Sensor task running on Core %d\n", xPortGetCoreID());
           vTaskDelay(pdMS_TO_TICKS(500));
       }
   }

   void setup() {
       Serial.begin(115200);
       xTaskCreatePinnedToCore(Core0_Network_Task, "NetTask", 2048, NULL, 1, NULL, 0);
       xTaskCreatePinnedToCore(Core1_Sensor_Task,  "SensTask", 2048, NULL, 2, NULL, 1);
   }

   void loop() { vTaskDelete(NULL); }
   ```
3. **Virtual Logic Analyzer:**
   - In `diagram.json`, add a `"wokwi-logic-analyzer"` component.
   - Connect probe channels to your SPI or I2C lines (`SCL`, `SDA`).
   - Run simulation, stop it, and Wokwi automatically downloads a `.vcd` file.
   - Open the `.vcd` file in **PulseView** to decode protocol transactions!

---

## 5. STM32 Bare-Metal Simulation (QEMU & Wokwi)

Before purchasing an STM32F4 Black Pill or Nucleo board, write bare-metal C and test it directly in QEMU.

### Workflow with QEMU:
1. **Compile with ARM GCC:**
   ```bash
   arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -O0 -T stm32f4.ld startup.c main.c -o firmware.elf
   ```
2. **Run in QEMU Emulator:**
   ```bash
   qemu-system-arm -M netduinoplus2 -kernel firmware.elf -nographic -serial stdio
   ```
   - Characters transmitted to USART1 will print directly in your command prompt terminal!
3. **Attach GDB for Line-by-Line Debugging:**
   - Launch QEMU with GDB server enabled:
     ```bash
     qemu-system-arm -M netduinoplus2 -kernel firmware.elf -nographic -s -S
     ```
   - In a second terminal window, connect GDB:
     ```bash
     arm-none-eabi-gdb firmware.elf
     (gdb) target remote localhost:1234
     (gdb) break main
     (gdb) continue
     (gdb) print/x *(uint32_t*)0x40023830  # Inspect RCC_AHB1ENR register!
     (gdb) step
     ```

---

## 🛡️ Pre-Flight Checklist: Transitioning from Simulator to Real Hardware

Once your code runs cleanly in simulation, follow this hardware safety checklist before connecting power to physical boards:

- [ ] **Verify Voltage Rails:** Are your sensors 3.3V or 5.0V? (Connecting 5V to an STM32 non-5V-tolerant pin will destroy the input buffer).
- [ ] **Continuity & Short-Circuit Check:** Use your multimeter in continuity mode to verify no dead shorts between $V_{CC}$ and $GND$ on your breadboard before plugging in USB.
- [ ] **Check Current Limiting Resistors:** Are all LEDs wired in series with $220\,\Omega - 470\,\Omega$ resistors?
- [ ] **Verify Pull-Up Resistors on I2C Buses:** Are `SDA` and `SCL` connected to $3.3\text{V}$ via $2.2\text{ k}\Omega - 4.7\text{ k}\Omega$ pull-up resistors?
- [ ] **Decoupling Capacitors Installed:** Is a $100\text{ nF}$ ceramic capacitor placed across $V_{DD}$ and $V_{SS}$ pins?
- [ ] **Confirm Ground Reference:** Are all system components (sensors, MCU, debug probe, power supply) sharing a single common ground plane?
