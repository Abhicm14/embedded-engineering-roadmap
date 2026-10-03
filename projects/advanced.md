# 🔴 Advanced Portfolio Projects (Projects 6 & 7 + Specializations)

---

## Project 6: Connected IoT Node (TLS 1.3, MQTT & Fail-Safe OTA)

> **Core Focus:** Hardware-accelerated TLS 1.3 cryptographic handshake, mutual X.509 certificate authentication (mTLS), MQTT 3.1.1 telemetry streaming, and dual-bank Flash A/B fail-safe Over-The-Air (OTA) firmware rollback.

### 1. Hardware Architecture
- **Target Silicon:** ESP32-WROOM-32 or STM32F401 + ESP8266/ESP32 as Wi-Fi Co-Processor (via AT commands or SPI).
- **Security:** Hardware cryptographic accelerator (AES-NI, SHA, RSA engine).

```
 [ Local Sensor ] ──► MCU ──► TLS 1.3 (mTLS) ──► Wi-Fi ──► [ AWS IoT Core / Mosquitto ]
                                                                       │
                                                                       ▼
                                                          Inbound Firmware Update
                                                          (Topic: devices/node/ota)
```

### 2. Dual-Bank A/B Flash Partitioning & Rollback Safety

```
SPI Flash Layout:
┌──────────────────────────────────────┐
│ Secure Bootloader (0x00000000)       │
├──────────────────────────────────────┤
│ Partition Table & NVS Certificates   │
├──────────────────────────────────────┤
│ Bank A (Slot 0): Active App (v1.0)   │ ◄── Running CPU code
├──────────────────────────────────────┤
│ Bank B (Slot 1): Update Slot (v1.1)  │ ◄── Inbound chunks stream here
├──────────────────────────────────────┤
│ OTA Sequence Metadata                │
└──────────────────────────────────────┘
```

### Rollback Logic:
1. Boot into Bank B after verified digital signature and CRC-32 checksum.
2. Arm a 30-second hardware watchdog.
3. If new firmware successfully authenticates with the cloud MQTT broker within 30 seconds, it calls `OTA_MarkValid()` to permanently commit Bank B.
4. If it crashes, enters a boot loop, or fails to connect, the watchdog expires, and the bootloader reverts back to Bank A on next power-on!

---

## Project 7: TinyML Edge Device (CMSIS-NN Quantized Gesture Classifier)

> **Core Focus:** On-device deep learning on ARM Cortex-M4 with hardware FPU, 6-axis IMU DSP pipeline, INT8 weight quantization, and sub-5ms inference latency.

### 1. Processing Pipeline & Architecture

```
 [ InvenSense MPU-6050 6-Axis IMU (50 Hz) ]
                     │ (Ax, Ay, Az, Gx, Gy, Gz)
                     ▼
 ┌───────────────────────────────────────┐
 │ 50-Sample Sliding Window Buffer (1.0s)│
 │ - Rolling Mean & Variance             │
 │ - Fast Peak-to-Peak Magnitude (RMS)   │
 └───────────────────┬───────────────────┘
                     │ (12 Normalized Features)
                     ▼
 ┌───────────────────────────────────────┐
 │ TensorFlow Lite Micro (CMSIS-NN INT8) │
 │ Dense(12 -> 32 -> 16 -> 4)            │
 └───────────────────┬───────────────────┘
                     │
                     ▼ (ArgMax Prediction)
        Detected: [ WAVE / PUNCH / STATIONARY ]
```

### 2. Why INT8 Quantization Matters on MCUs
- **Flash Footprint:** Quantizing weights from Float32 (4 bytes) to INT8 (1 byte) reduces neural network model size by **75%** (e.g. from 120 KB to 30 KB).
- **Execution Speed:** ARM Cortex-M4 SIMD instructions (e.g. `SMLAD` - Signed Multiply-Accumulate Dual) can perform two 16-bit multiplications and accumulations in a single CPU clock cycle!

---

## Specialization Add-On: Automotive CAN Bus Gateway Node

> **Core Focus:** CAN 2.0B / CAN-FD differential bus, hardware acceptance filter masks, and OBD-II PID telemetry parsing.

- **Silicon:** STM32F401 (bxCAN) + TI SN65HVD230 3.3V CAN transceiver.
- **Physical Wiring:** Differential twisted pair (`CAN_H`, `CAN_L`) with $120\,\Omega$ termination resistors.
- **Filter Bank:** Hardware filters configured so the CPU only wakes on diagnostic query IDs (`0x7DF` functional, `0x7E0` ECU physical).
- **OBD-II Engine:** Parses Service 01 PIDs: Engine RPM (`0x0C`), Vehicle Speed (`0x0D`), and Engine Coolant Temperature (`0x05`).

---

## 🔌 PIC Advanced Track: Capstone Projects P6 & P7

### Project P6: Closed-Loop PID Temperature Controller with LCD & EEPROM
- **Platform:** Microchip PIC16F877A with external 4 MHz crystal on breadboard or PICSimLab.
- **Core Focus:** Closed-loop discrete PID control, 10-bit ADC sampling with moving average filter, CCP1 hardware PWM heater drive, on-chip 256-byte Data EEPROM setpoint persistence with mandatory unlock sequence (`0x55`/`0xAA`), and 16x2 character LCD interface.
- **Architecture:**
  - Sensor input: 10k NTC thermistor / analog voltage on `RA0/AN0`.
  - Actuator output: CCP1 PWM on `RC2` driving a heating element or fan stage.
  - User controls: Pushbuttons on `RB1` and `RB2` to adjust target temperature setpoint, persisted across resets in internal EEPROM.
- **Implementation Guide & Code:**
  - Comprehensive Guide: [PIC Systems & Hardware Interfacing (Steps 9–13)](../pic-mplab-xc8/advanced.md)

### Project P7: PIC16F877A to PC Serial Telemetry & Control Bridge
- **Platform:** PIC16F877A USART module connected to host PC running a Python telemetry visualizer.
- **Core Focus:** Interrupt-driven serial framing, multi-channel analog sensor acquisition, formatted ASCII telemetry stream, and remote command parser.
- **Features:**
  - Streams periodic formatted sensor packets: `$SENS,AN0,AN1,AN2,SWITCHES*CHECKSUM\r\n`.
  - Python host application charts real-time voltages and sends remote commands (`$CMD,SET_PWM,75*`) to control local PIC outputs.
- **Implementation Guide & Code:**
  - UART Driver: [`code-examples/xc8/uart_echo.c`](../code-examples/xc8/uart_echo.c)
  - Capstone Details: [PIC Track Advanced Guide](../pic-mplab-xc8/advanced.md#step-13-capstone-projects)
