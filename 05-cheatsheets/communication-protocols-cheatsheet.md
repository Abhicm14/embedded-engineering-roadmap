# 📡 Communication Protocols Comparison & Technical Cheatsheet

> Quick reference comparison matrix, signal topologies, electrical levels, timing parameters, and frame formats for UART, SPI, I2C, and CAN buses.

---

## 1. Master Protocol Comparison Matrix

| Feature | UART / USART | SPI | I2C / SMBus | CAN 2.0B / CAN-FD |
| :--- | :--- | :--- | :--- | :--- |
| **Line Count** | 2 lines (`TX`, `RX`) (+ optional RTS/CTS) | 4 lines (`MOSI`, `MISO`, `SCK`, `CS`) | 2 lines (`SDA`, `SCL`) | 2 lines differential (`CAN_H`, `CAN_L`) |
| **Synchronization** | Asynchronous (clocks matched by baud) | Synchronous (explicit clock line) | Synchronous (explicit clock line) | Asynchronous (synchronized on bit edges) |
| **Duplex** | Full Duplex | Full Duplex | Half Duplex | Half Duplex |
| **Topology** | Point-to-Point | Multi-drop (individual `CS` or Daisy-chain) | Multi-master, Multi-slave (addressable) | Multi-master (broadcast with arbitration) |
| **Typical Speed** | 9600 bps - 1 Mbps | 1 Mbps - 50+ Mbps | 100 kbps (Standard), 400 kbps (Fast), 1-3.4 Mbps | 125 kbps - 1 Mbps (CAN), up to 5-8 Mbps (CAN-FD) |
| **Max Distance** | Short (< 3m); extended with RS-485 to 1200m | Short on-board (< 30cm) | Short on-board (< 1m) | Long (40m at 1 Mbps, 1000m at 50 kbps) |
| **Driver Type** | Push-Pull | Push-Pull | Open-Drain (requires pull-up resistors) | Differential transceiver (requires $120\Omega$ termination) |
| **Addressing** | None | Hardware chip select pin per slave | 7-bit (128 nodes) or 10-bit addressing | Content-based (11-bit or 29-bit message IDs) |

---

## 2. SPI Clock Modes (CPOL & CPHA)

The 4 SPI modes define the clock idle level and the sampling transition:

| Mode | CPOL (Clock Polarity) | CPHA (Clock Phase) | Clock Idle State | Data Captured / Sampled On | Data Shifted / Changed On |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **Mode 0** | 0 | 0 | LOW | 1st edge (Rising) | 2nd edge (Falling) |
| **Mode 1** | 0 | 1 | LOW | 2nd edge (Falling) | 1st edge (Rising) |
| **Mode 2** | 1 | 0 | HIGH | 1st edge (Falling) | 2nd edge (Rising) |
| **Mode 3** | 1 | 1 | HIGH | 2nd edge (Rising) | 1st edge (Falling) |

*Note: Mode 0 and Mode 3 are the most universally supported modes among sensors, flash memories, and displays.*

---

## 3. I2C Bus Signaling & Timing

### Bus Conditions:
- **IDLE:** Both `SDA` and `SCL` are pulled HIGH by external pull-up resistors.
- **START Condition:** `SDA` transitions from HIGH to LOW while `SCL` is HIGH.
- **STOP Condition:** `SDA` transitions from LOW to HIGH while `SCL` is HIGH.
- **Data Validity:** `SDA` must remain stable during the HIGH period of `SCL`. Data changes only when `SCL` is LOW.
- **ACK / NACK:** The receiver pulls `SDA` LOW on the 9th clock pulse to acknowledge (ACK) or leaves it HIGH (NACK).

### Pull-Up Resistor Calculation:
$$R_{p(min)} = \frac{V_{DD} - V_{OL(max)}}{I_{OL}} \approx \frac{3.3\text{V} - 0.4\text{V}}{3\text{ mA}} \approx 966\,\Omega$$
$$R_{p(max)} = \frac{t_r}{0.8473 \times C_b} \quad (t_r = 1000\text{ ns for } 100\text{ kHz}, 300\text{ ns for } 400\text{ kHz})$$

---

## 4. CAN Bus Differential Physical Layer & Framing

### Differential Voltages (High-Speed CAN):
- **Recessive Bit ('1'):** `CAN_H` = 2.5V, `CAN_L` = 2.5V ($\Delta V = 0\text{V}$). Both drivers passive; bus pulled together by termination resistors.
- **Dominant Bit ('0'):** `CAN_H` driven to 3.5V, `CAN_L` driven to 1.5V ($\Delta V = 2.0\text{V}$). Dominant overrides recessive.

### Termination:
- A $120\,\Omega$ termination resistor must be placed across `CAN_H` and `CAN_L` at each of the two physical extremities of the bus cable (net equivalent resistance: $60\,\Omega$).

### Standard CAN 2.0A Frame Anatomy:
`[SOF (1b)]` $\rightarrow$ `[Identifier (11b)]` $\rightarrow$ `[RTR (1b)]` $\rightarrow$ `[IDE (1b)]` $\rightarrow$ `[r0 (1b)]` $\rightarrow$ `[DLC (4b)]` $\rightarrow$ `[Data Payload (0-8 Bytes)]` $\rightarrow$ `[CRC (15b)]` $\rightarrow$ `[CRC Delim (1b)]` $\rightarrow$ `[ACK (1b)]` $\rightarrow$ `[ACK Delim (1b)]` $\rightarrow$ `[EOF (7b)]`
