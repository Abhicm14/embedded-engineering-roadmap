# 🏎️ Automotive Embedded Systems Track

> Vehicle Electronic Control Units (ECUs), CAN/CAN-FD, UDS Diagnostics, AUTOSAR, and ISO 26262 Functional Safety.

---

## 1. Controller Area Network (CAN 2.0B & CAN-FD)

Modern passenger vehicles contain 70 to 150+ ECUs interconnected across multiple CAN buses:
- **Physical Layer:** Differential signaling on twisted pair (`CAN_H` and `CAN_L`) terminated with $120\,\Omega$ at each cable end.
- **Bit Timing:** Nominal baud rate typically $500\text{ kbps}$ (CAN 2.0B) or up to $5-8\text{ Mbps}$ during data phase (CAN-FD).
- **Non-Destructive Bitwise Arbitration:** A dominant '0' bit overrides a recessive '1' bit. The message with the lowest numerical ID wins bus arbitration without packet collision delay.

---

## 2. Automotive Diagnostic Protocols (UDS / ISO 14229)

Unified Diagnostic Services (UDS) operates over ISO-TP (ISO 15765-2) transport layer:
- **Service `0x10`:** Diagnostic Session Control (Default, Programming, Extended).
- **Service `0x22`:** Read Data By Identifier (DID).
- **Service `0x2E`:** Write Data By Identifier.
- **Service `0x27`:** Security Access (Seed & Key cryptographic challenge).
- **Service `0x31`:** Routine Control (Starting motor calibration or self-tests).

---

## 3. AUTOSAR Architecture Overview

AUTOSAR (Automotive Open System Architecture) standardizes vehicle software:

```
┌────────────────────────────────────────────────────────┐
│ APPLICATION LAYER (Software Components - SWCs)         │
├────────────────────────────────────────────────────────┤
│ RUNTIME ENVIRONMENT (RTE - Virtual Functional Bus)     │
├────────────────────────────────────────────────────────┤
│ BASIC SOFTWARE (BSW)                                   │
│  - System Services (OS, Watchdog, EcuM)                │
│  - Memory Services (NvM, Fee, Fls)                     │
│  - Communication Services (Com, CanIf, CanTp, PduR)    │
│  - I/O Hardware Abstraction                            │
├────────────────────────────────────────────────────────┤
│ MICROCONTROLLER ABSTRACTION LAYER (MCAL)               │
│  - Silicon-specific register drivers (Can, Dio, Port)  │
└────────────────────────────────────────────────────────┘
```

---

## 4. Functional Safety (ISO 26262) & MISRA-C

- **Automotive Safety Integrity Levels (ASIL):** Ranging from ASIL A (lowest risk) to ASIL D (highest risk, e.g. steering and braking).
- **MISRA-C:2012:** Mandatory coding guidelines restricting undefined behaviors in C:
  - No unbounded recursion.
  - No dynamic memory allocation after initialization.
  - Explicit pointer conversions and mandatory curly braces for all control blocks.
