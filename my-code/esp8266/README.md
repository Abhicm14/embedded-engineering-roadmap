# 📡 ESP8266 (NodeMCU / ESP-12) Custom Workspace

> **Silicon Platform:** Espressif Systems ESP8266 (Tensilica Xtensa L106 32-bit RISC, 80/160 MHz, 4MB Flash, 2.4 GHz Wi-Fi)  
> **Simulation First:** [**Wokwi ESP8266 Simulator**](https://wokwi.com/projects/new/esp8266)

---

## 🎯 Wi-Fi & Embedded Networking Features

The ESP8266 introduces you to TCP/IP networking, wireless sockets, and IoT protocols:
1. **Wi-Fi Modes:**
   - Station Mode (`WIFI_STA`): Connects to your home/lab Wi-Fi router.
   - Access Point Mode (`WIFI_AP`): Broadcasts its own Wi-Fi SSID for direct phone/laptop configuration.
   - Dual Mode (`WIFI_AP_STA`).
2. **TCP/IP Stack (lwIP):**
   - Raw TCP and UDP sockets.
   - HTTP Web Server (REST APIs, JSON telemetry responses).
3. **IoT Telemetry:**
   - MQTT Protocol: Publish sensor telemetry to HiveMQ / Mosquitto brokers.
   - NTP Clock Sync: Synchronizing local RTC clock with internet atomic time servers.
4. **Hardware Peripherals:**
   - Single 10-bit ADC (`A0`, 0 to 1.0V or 0 to 3.3V on NodeMCU with onboard divider).
   - Hardware UART0 (Pins TX0/RX0) and UART1 (Transmit-only on GPIO2).
   - Software I2C and SPI buses.

---

## 💻 Simulation Before Hardware Flashing

Before powering your physical ESP8266 / NodeMCU module:
1. Open the [**Wokwi ESP8266 Simulator**](https://wokwi.com/projects/new/esp8266).
2. Use the virtual **Wokwi-GUEST** Wi-Fi access point (no password needed) to simulate internet connectivity in real-time.
3. Test your HTTP GET/POST requests and MQTT publish/subscribe logic.
4. Verify your logic analyzer traces for I2C and SPI sensors.
5. Save your completed sketches into [`sketches/`](sketches/) and flash your physical hardware using Arduino IDE or PlatformIO.

---

## 📁 Sketches Directory
Place your ESP8266 sketches and source files in:
👉 [`sketches/`](sketches/)
