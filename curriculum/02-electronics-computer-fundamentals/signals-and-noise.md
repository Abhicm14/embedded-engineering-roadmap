# 〰️ Signals, Noise, Slew Rate & Grounding

> Understanding the physical transmission of digital square waves and eliminating noise in firmware and circuits.

---

## 1. Digital Signals are Analog Waveforms

A digital square wave is not an instantaneous transition between 0V and 3.3V. According to Fourier analysis, a square wave is an infinite summation of odd harmonics:
$$f(t) = \sin(\omega t) + \frac{1}{3}\sin(3\omega t) + \frac{1}{5}\sin(5\omega t) + \dots$$

```
Ideal Square Wave:       Real World Square Wave:
┌──────┐                 ┌─/\─┐  ◄── Overshoot / Ringing
│      │                 │    │
│      │               ┌─┘    └─┐◄── Slew Rate (Rise / Fall Time)
┘      └───────        ┘        \__/\_
```

- **Rise Time ($t_r$):** Time taken to transition from 10% to 90% of signal amplitude. High-frequency electromagnetic radiation is proportional to $1 / t_r$, **not** the repetition frequency!
- **Ringing / Overshoot:** Caused by parasitic trace inductance ($L$) interacting with pin capacitance ($C$). Prevented by placing a series damping resistor ($22\,\Omega - 33\,\Omega$) at the output pin.

---

## 2. Ground Bounce & Simultaneous Switching Noise (SSN)

When multiple high-current GPIO output pins switch from HIGH to LOW simultaneously:
- Discharge currents rush through internal silicon bond wires to the chip ground pin.
- The parasitic inductance ($L_{bond}$) of the package ground lead induces a voltage spike:
  $$V_{bounce} = L_{bond} \frac{di}{dt}$$
- This ground bounce momentarily shifts the chip's internal ground reference relative to the PCB ground, causing false interrupt triggers or memory glitches.

### Firmware Solutions:
1. **Reduce Slew Rate in Registers:** Set GPIO speed register (`OSPEEDR`) to **Low** or **Medium** unless driving high-speed SPI or external memory. High slew rate creates unnecessary EMI and ground bounce!
2. **Stagger Output Pin Switching:** Avoid toggling 16 pins in the exact same clock cycle if driving heavy loads.

---

## 3. Electrostatic Discharge (ESD) Protection

Human bodies can accumulate 15,000+ volts of static charge. When plugging in a USB cable or touching a button, this charge discharges into the circuit.
- **TVS (Transient Voltage Suppressor) Diodes:** Silicon clamping diodes placed directly at the physical connector edge between signal lines and Ground. They remain transparent during normal 3.3V operation, but clamp high-voltage spikes to safe levels ($< 6\text{V}$) within picoseconds.
