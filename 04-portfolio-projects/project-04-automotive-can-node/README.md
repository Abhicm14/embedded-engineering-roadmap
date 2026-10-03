# 📁 Project 04: Automotive CAN Bus Gateway & Telemetry Node

> Target: STM32F4 (bxCAN) / ESP32 (TWAI) with 3.3V CAN Transceiver (SN65HVD230 / VP230)  
> Language: Embedded C (C99)  
> Key Technologies: CAN 2.0B / CAN-FD, Hardware Acceptance Filters, OBD-II PID Parser, Fault Recovery

---

## 🎯 Project Overview

This project implements an automotive telemetry node and diagnostic gateway interfacing with a vehicle's high-speed CAN bus (500 kbps). It filters incoming diagnostic requests and broadcasts engine telemetry according to SAE J1979 / OBD-II standards.

### Architectural Highlights:
1. **Hardware Filter Masks:** Setting up CAN acceptance filters to accept only diagnostic request IDs (`0x7DF` functional broadcast, `0x7E0` physical ECU query) directly in silicon hardware, shielding the CPU from non-relevant bus traffic.
2. **OBD-II Diagnostic Engine:** Parsing Service 01 PIDs:
   - PID `0x05`: Engine Coolant Temperature ($A - 40 \text{ ^\circ C}$)
   - PID `0x0C`: Engine RPM ($\frac{256A + B}{4} \text{ RPM}$)
   - PID `0x0D`: Vehicle Speed ($A \text{ km/h}$)
   - PID `0x11`: Throttle Position ($\frac{100A}{255} \%$)
3. **Bus-Off Recovery State Machine:** Detecting heavy error counter accumulation (`TEC` / `REC` > 255) and safely executing automatic or software-controlled bus-off recovery without locking up the vehicle network.

---

## 🔌 Hardware Setup

```
[ STM32 / MCU ]                   [ CAN Transceiver ]             [ Vehicle CAN Bus ]
   TX Pin (PA12) ────────► TXD (SN65HVD230) CAN_H ─────────────── High Wire
   RX Pin (PA11) ◄──────── RXD             CAN_L ─────────────── Low Wire
                                             ▲
                                             │ [ 120 Ohm Termination Resistor ]
```

---

## 🏎️ CAN Frame Layout (OBD-II Service 01 Response)

```
CAN ID: 0x7E8 (Engine ECU Response) | DLC: 8
┌───────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
│ Byte 0│  Byte 1  │  Byte 2  │  Byte 3  │  Byte 4  │  Byte 5  │  Byte 6  │  Byte 7  │
├───────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│ Length│ Service  │   PID    │ Data A   │ Data B   │ Data C   │ Data D   │ Padding  │
│ (0x04)│ (0x41)   │  (0x0C)  │ (0x1F)   │ (0x40)   │ (0xAA)   │ (0xAA)   │ (0xAA)   │
└───────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
RPM = ((0x1F * 256) + 0x40) / 4 = 2,000 RPM
```
