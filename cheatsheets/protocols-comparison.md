# 📡 Embedded Communication Protocols Comparison

---

## 1. Master Protocol Comparison Matrix

| Specification | UART / USART | SPI | I2C / SMBus | CAN 2.0B / CAN-FD | USB 2.0 (FS/HS) | Ethernet (100Base-TX) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Lines / Wires** | 2 (`TX`, `RX`) | 4 (`MOSI`, `MISO`, `SCK`, `CS`) | 2 (`SDA`, `SCL`) | 2 (`CAN_H`, `CAN_L`) | 4 (`VBUS`, `D+`, `D-`, `GND`) | 4 / 8 (2/4 twisted pairs) |
| **Clocking** | Asynchronous | Synchronous | Synchronous | Asynchronous | Asynchronous (NRZI) | Synchronous (MLT-3) |
| **Duplex** | Full Duplex | Full Duplex | Half Duplex | Half Duplex | Half Duplex | Full Duplex |
| **Max Bitrate** | 1 - 5 Mbps | 10 - 80 Mbps | 100k / 400k / 1M / 3.4M | 1M (CAN) / 8M (CAN-FD) | 12 Mbps (FS) / 480 Mbps (HS) | 100 Mbps / 1 Gbps |
| **Max Distance** | $< 3\text{m}$ (1200m with RS485)| $< 30\text{cm}$ | $< 1\text{m}$ | $40\text{m}$ (1Mbps) to $1\text{km}$ | $5\text{m}$ | $100\text{m}$ |
| **Electrical Interface**| Push-Pull (3.3V/5V) | Push-Pull | Open-Drain (Pull-ups) | Differential (2V diff) | Differential (3.3V / 400mV) | Differential transformer |
| **Addressing** | None | Pin Chip Select | 7-bit / 10-bit address | 11-bit / 29-bit message ID | Dynamic enumeration | 48-bit MAC address |
| **Error Checking** | Optional parity | None in hardware | ACK / NACK bit | 15/17-bit CRC + ACK | 16-bit CRC + tokens | 32-bit CRC (FCS) |

---

## 2. SPI Clock Modes Summary

| Mode | CPOL | CPHA | Clock Idle | Sampling Transition |
| :---: | :---: | :---: | :---: | :--- |
| **Mode 0** | 0 | 0 | LOW | 1st edge (Rising) |
| **Mode 1** | 0 | 1 | LOW | 2nd edge (Falling) |
| **Mode 2** | 1 | 0 | HIGH | 1st edge (Falling) |
| **Mode 3** | 1 | 1 | HIGH | 2nd edge (Rising) |

---

## 3. I2C Bus Pull-Up Sizing Formula

$$R_{p(max)} = \frac{t_r}{0.8473 \times C_b}$$
- Standard Mode ($100\text{ kHz}$): $t_r = 1000\text{ ns}$
- Fast Mode ($400\text{ kHz}$): $t_r = 300\text{ ns}$
- Typical practical values: $2.2\text{ k}\Omega$ to $4.7\text{ k}\Omega$
