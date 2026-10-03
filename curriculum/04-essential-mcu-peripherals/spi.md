# ⚡ Serial Peripheral Interface (SPI)

> Synchronous 4-wire high-speed serial bus, clock polarity/phase modes, and slave selection.

---

## 1. SPI Bus Topology & Shift Register Ring

SPI connects a Master to one or more Slaves using 4 digital lines:
- **`MOSI` (Master Out, Slave In):** Data sent from Master to Slave.
- **`MISO` (Master In, Slave Out):** Data sent from Slave to Master.
- **`SCK` (Serial Clock):** Generated exclusively by Master (up to 50+ MHz).
- **`CS` / `NSS` (Chip Select):** Active LOW line selecting target peripheral.

```
       MASTER                                SLAVE
┌──────────────────┐                  ┌──────────────────┐
│  Shift Register  │ ───► [MOSI] ───► │  Shift Register  │
│  [b7..b0]        │ ◄─── [MISO] ◄─── │  [b7..b0]        │
│                  │ ───► [SCK ] ───► │                  │
│                  │ ───► [CS  ] ───► │                  │
└──────────────────┘                  └──────────────────┘
Every bit shifted OUT on MOSI simultaneously shifts a bit IN on MISO!
```

---

## 2. The 4 SPI Clock Modes

Configured via `CPOL` (Clock Polarity) and `CPHA` (Clock Phase) in `SPIx->CR1`:

| Mode | CPOL | CPHA | Clock Idle Level | Data Sampled On | Data Shifted / Changed On |
| :---: | :---: | :---: | :---: | :--- | :--- |
| **0** | 0 | 0 | LOW | 1st edge (Rising) | 2nd edge (Falling) |
| **1** | 0 | 1 | LOW | 2nd edge (Falling) | 1st edge (Rising) |
| **2** | 1 | 0 | HIGH | 1st edge (Falling) | 2nd edge (Rising) |
| **3** | 1 | 1 | HIGH | 2nd edge (Rising) | 1st edge (Falling) |

*Note: Modes 0 and 3 are by far the most widely adopted standards.*

---

## 🛠️ Starter Exercise
Connect an external Winbond W25Qxx SPI Flash memory or SPI sensor. Pull `CS` LOW, transmit command `0x9F` (Read JEDEC ID), read back 3 response bytes, and pull `CS` HIGH. Verify manufacturer ID against datasheet.
