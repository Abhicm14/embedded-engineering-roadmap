# 🔬 Lab Instruments & Probing Mastery for Firmware Engineers

> How to use a Multimeter, Digital Storage Oscilloscope (DSO), and Logic Analyzer without introducing measurement errors.

---

## 1. Digital Multimeter (DMM)

### What to Measure:
1. **Continuity:** Verifying physical copper traces between MCU pins and sensor pads. Always perform with target power OFF!
2. **DC Voltage:** Measuring $3.3\text{V}$ and $5\text{V}$ power rails directly at MCU pins.
3. **Current Consumption:** Inserting the DMM in series with the power rail to measure active mode current ($\approx 15-40\text{ mA}$) and sleep mode current ($\mu\text{A}$).

---

## 2. Digital Storage Oscilloscope (DSO)

### The 10X Probe Rule:
- Always set your oscilloscope probe switch to **10X**.
- In **1X mode**, the probe introduces $\approx 100\text{ pF}$ of capacitance and $1\text{ M}\Omega$ resistance. This massive capacitance loads high-speed lines, distorts SPI waveforms, and will halt sensitive quartz crystal oscillators!
- In **10X mode**, capacitance drops to $\approx 10-15\text{ pF}$ and resistance increases to $10\text{ M}\Omega$, preserving true signal integrity.

### Probe Compensation:
Before taking measurements, connect the 10X probe to the oscilloscope's $1\text{ kHz}$ calibration square-wave terminal. Turn the probe trimmer capacitor using a plastic screwdriver until the corners of the square wave are perfectly square (no overshoot or rounding).

```
   Over-Compensated:      Properly Compensated:     Under-Compensated:
        ┌─/\─┐                   ┌────┐                   ┌─/──┐
        │    │                   │    │                   │    │
        ┘    └───                ┘    └───                ┘    └───
```

---

## 3. Logic Analyzer & Protocol Decoders

A logic analyzer samples multiple digital lines at discrete time intervals and reconstructs logical transitions (0 and 1).

### Setup with PulseView:
1. Connect logic analyzer ground lead directly to the target board ground.
2. Connect Channel 0 to `UART_TX` or `I2C_SDA`, and Channel 1 to `I2C_SCL`.
3. Set sample rate to at least $4\times$ the bus rate (e.g. $4\text{ MHz}$ sample rate for a $400\text{ kHz}$ I2C bus).
4. Add the protocol decoder (select "I2C" or "UART").
5. Trigger on the falling edge of SDA (I2C START condition) to capture transaction packets.
