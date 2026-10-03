# 🔁 Inter-Integrated Circuit (I2C)

> Synchronous 2-wire open-drain bus, addressing, arbitration, pull-up calculation, and bus lockup recovery.

---

## 1. Bus Signaling: START, STOP & ACK/NACK

Both `SDA` (Data) and `SCL` (Clock) are open-drain lines pulled up to 3.3V by external resistors ($2.2\text{ k}\Omega - 4.7\text{ k}\Omega$).

```
SCL:  ──────┐     ┌───┐   ┌───┐   ┌───┐       ┌──────
            └─────┘   └───┘   └───┘   └───────┘
SDA:  ──┐               Bit 7   Bit 6      ┌─────────
        └──────────────────────────────────┘
        ▲                                  ▲
     START Condition                     STOP Condition
  (SDA falls while SCL is HIGH)       (SDA rises while SCL is HIGH)
```

- **Data Change Rule:** `SDA` can only change state when `SCL` is LOW. If `SDA` changes while `SCL` is HIGH, it is interpreted as a **START** or **STOP** condition!
- **ACK / NACK:** On the 9th clock pulse, the receiver pulls `SDA` LOW to acknowledge receipt of the byte (ACK = 0). Leaving `SDA` HIGH indicates NACK (1).

---

## 2. The Infamous I2C Bus Lockup & 9-Clock Recovery

### Why it Happens:
If the MCU resets or halts mid-read while the slave sensor is outputting a '0' data bit, the slave holds `SDA` permanently LOW waiting for clocks. The MCU reboots, sees `SDA` LOW, thinks the bus is busy, and hangs indefinitely in `while(I2C->SR & BUSY)`.

### The 9-Clock Recovery Sequence in Software:
```c
void I2C_BusClear(void) {
    // 1. Configure SCL and SDA as GPIO Output Open-Drain
    // 2. Toggle SCL 9 times to clock out any hung slave data:
    for (int i = 0; i < 9; i++) {
        SCL_LOW();  DelayUs(5);
        SCL_HIGH(); DelayUs(5);
    }
    // 3. Generate a manual STOP condition:
    SDA_LOW();  DelayUs(5);
    SCL_HIGH(); DelayUs(5);
    SDA_HIGH(); DelayUs(5);
    // 4. Re-initialize hardware I2C peripheral
}
```

---

## 🛠️ Starter Exercise
Write a register-level I2C scanner that queries 7-bit addresses `0x08` through `0x77`. If a slave responds with ACK, print its hexadecimal address to UART terminal.
