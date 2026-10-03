# 🔍 When to Use an Oscilloscope vs a Logic Analyzer

> Choosing the right instrument to diagnose electrical versus protocol defects.

---

## 1. Instrument Comparison Matrix

| Problem Scenario | Best Tool | Why |
| :--- | :---: | :--- |
| **"Is the power rail noisy or dipping when the motor turns on?"** | **Oscilloscope** | Measures real analog voltages, high-frequency ripple, and voltage transients. |
| **"Why is the I2C sensor returning NACK?"** | **Logic Analyzer** | Decodes full protocol packet stream (Address, R/W, ACK/NACK, Data) over thousands of cycles. |
| **"Why does SPI clock look distorted and rounded?"** | **Oscilloscope** | Visualizes analog rise/fall times and parasitic trace capacitance loading. |
| **"Are bytes dropped during high-speed UART bursts?"** | **Logic Analyzer** | Records long continuous streams of digital bits and highlights framing/parity errors. |
| **"Did an inductive spike exceed the MCU's 3.6V absolute maximum rating?"** | **Oscilloscope** | High-bandwidth analog capture reveals nanosecond voltage overshoots. |

---

## 2. Structured Diagnostic Flowchart

```
Bug Observed: Peripheral Communication Failure
       │
       ▼
Check 1: Electrical Integrity (Oscilloscope)
 ├── Is VDD stable at 3.3V with < 50mV ripple?
 └── Are signal rise times sharp and within protocol specs?
       │
       ▼ (Electrical signals verified OK)
Check 2: Protocol Timing & Framing (Logic Analyzer)
 ├── Is baud rate / clock frequency within 2% of target?
 ├── Are SPI Mode CPOL and CPHA correctly matched?
 └── Did the slave acknowledge (ACK) its 7-bit address?
       │
       ▼ (Bus transactions verified OK)
Check 3: Firmware & Software Logic (GDB / Serial Logs)
 ├── Are variables marked volatile?
 ├── Are DMA buffer pointers aligned?
 └── Is an interrupt pending bit properly cleared?
```
