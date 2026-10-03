# 📈 Analog-to-Digital (ADC) & Digital-to-Analog (DAC)

> Successive Approximation Registers (SAR), sampling time, impedance loading, and DMA integration.

---

## 1. SAR ADC Core Concepts

A Successive Approximation Register (SAR) ADC converts an analog voltage into a digital number via binary search:

```
Resolution: 12-bit (0 to 4095 counts)
Analog Reference Voltage: V_REF = 3.3V

Digital Count = (V_IN / V_REF) * 4095
V_IN = (Digital Count * V_REF) / 4095.0f
```

---

## 2. Sampling Time & Source Impedance Trap

Before the ADC can convert, its internal sampling capacitor ($C_{ADC} \approx 5-10\text{ pF}$) must charge up through the analog multiplexer and source resistance:
- If source resistance $R_{AIN}$ is high (e.g. from an unbuffered $100\text{ k}\Omega$ voltage divider), the capacitor does not fully charge during short sampling periods, resulting in artificially low ADC counts!
- **Solution:** Increase the sampling time register (`ADC_SMPR`) or add a $0.1\mu\text{F}$ capacitor at the pin to act as a charge reservoir (or buffer with an op-amp follower).

---

## 🛠️ Starter Exercise
Configure STM32 `ADC1` Channel 0 in continuous conversion mode backed by DMA stream. Buffer 16 samples in RAM and compute a moving average filter.
