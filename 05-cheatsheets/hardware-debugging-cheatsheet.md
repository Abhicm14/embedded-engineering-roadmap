# 🔬 Hardware Debugging & Oscilloscope Guide

> Practical workbench troubleshooting guide for diagnosing electrical noise, bus lockups, floating pins, and signal integrity anomalies.

---

## 1. The "Before You Flip the Power Switch" Sanity Checklist

Over 80% of damaged development boards happen in the first 5 seconds of power-on. Run these three tests with a Digital Multimeter (DMM) on Resistance/Continuity mode:

1. **Short-to-Ground Test:** Measure resistance between `3.3V` / `5V` power rail and `GND`.
   - **Good:** High resistance ($> 1\text{ k}\Omega$, often climbing as bulk caps charge).
   - **Fatal:** Resistance $< 5\,\Omega$ or loud continuity beep. **DO NOT APPLY POWER.** Look for solder bridges or reversed ICs.
2. **Reverse Polarity Verification:** Verify the red lead of your power supply connects strictly to $V_{IN}$ and black to $GND$.
3. **Current-Limit Setup:** Set benchtop power supply current compliance to $100\text{ mA}$ during initial board bringup. If the MCU board draws $> 80\text{ mA}$ while idle, shut down immediately.

---

## 2. Oscilloscope Probing Mastery

### The $10\text{X}$ Probe Rule:
- Always use the probe in **$10\text{X}$ mode** rather than $1\text{X}$.
  - In $1\text{X}$ mode, probe capacitance is $\approx 100\text{ pF}$, which severely rounds high-speed edges (like SPI clock at 10 MHz) and can even stop quartz crystals from oscillating.
  - In $10\text{X}$ mode, probe capacitance drops to $\approx 10\text{ pF}$, minimizing circuit loading.

### Ground Lead Inductance:
- The standard 6-inch alligator ground clip creates an LC circuit with the probe's tip capacitance, causing artificial ringing on fast square wave edges.
- For high-speed signals ($> 20\text{ MHz}$), remove the alligator lead and use a short spring ground contact directly adjacent to the test point.

### Oscilloscope Trigger Modes:
- **Auto:** Continuous sweep; ideal for hunting steady-state DC levels or low-frequency signals.
- **Normal:** Sweeps only when the trigger condition is met; essential for capturing bursts of data.
- **Single:** Captures exactly one event and freezes; mandatory for catching power-up sequences, reset button presses, or rare fault spikes.

---

## 3. Logic Analyzer & Protocol Decoding (PulseView / Sigrok)

### Sampling Frequency Nyquist Rule:
To reliably decode a digital protocol without jitter artifacts, set the logic analyzer sample rate to at least **$4\times$ to $10\times$** the bus frequency:
- UART at 115,200 baud: Sample rate $\ge 1\text{ MHz}$.
- I2C at 400 kHz: Sample rate $\ge 4\text{ MHz}$.
- SPI at 10 MHz: Sample rate $\ge 50\text{ MHz}$ (or use a dedicated hardware protocol analyzer).

---

## 4. Top 6 Embedded Hardware Traps & How to Fix Them

| Trap | Symptom | Root Cause | Fix |
| :--- | :--- | :--- | :--- |
| **Floating Input Pin** | MCU GPIO reads erratic 0s and 1s randomly when touching the wire. | Pin configured as high-impedance input with no internal/external resistor. | Enable internal pull-up/pull-down (`PUPDR`) or add external $10\text{ k}\Omega$ resistor. |
| **Back-Powering via I/O** | MCU LED glows faintly even when power supply is disconnected! | Voltage applied to GPIO pin feeds through internal ESD protection diode into $V_{DD}$ rail. | Disconnect programmer/UART cables before cutting target power, or use level translators. |
| **I2C Bus Lockup** | I2C transaction hangs forever; `SDA` held permanently LOW. | Slave device interrupted mid-transaction while outputting a '0' bit. | Send 9 manual clock pulses on SCL from MCU GPIO to release slave, then generate a STOP condition. |
| **Inverted UART Lines** | Terminal outputs garbage gibberish or nothing at all. | `TX` and `RX` swapped, or mismatched baud rates. | Remember: MCU `TX` connects to Adapter `RX`; MCU `RX` connects to Adapter `TX`. |
| **Random MCU Resets** | MCU reboots unexpectedly when a motor or relay switches on. | Inductive kickback or sudden voltage dip ($V_{DD} < V_{BOR}$). | Add freewheeling diode across inductive coil + $100\mu\text{F}$ bulk capacitor at power input. |
| **Missing Decoupling** | ADC readings fluctuate wildly; intermittent crashes under heavy computation. | High-frequency power rail ripple due to lack of local bypass capacitors. | Place a $0.1\mu\text{F}$ (100nF) ceramic capacitor as close as physically possible ($< 3\text{mm}$) to every $V_{DD}$ pin. |
