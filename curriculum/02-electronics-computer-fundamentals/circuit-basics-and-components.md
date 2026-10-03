# ⚡ Circuit Basics & Electronic Components for Firmware Engineers

> Passive components, semiconductor switches, pull-up resistors, and power regulation.

---

## 1. Fundamental Circuit Laws

### Ohm's Law:
$$V = I \times R \quad \Longleftrightarrow \quad I = \frac{V}{R} \quad \Longleftrightarrow \quad R = \frac{V}{I}$$

### Calculating LED Current Limiting Resistor:
Given an MCU operating at $V_{DD} = 3.3\text{V}$, an LED with forward voltage drop $V_F = 2.0\text{V}$, and desired forward current $I_F = 5\text{ mA}$ ($0.005\text{ A}$):
$$R = \frac{V_{DD} - V_F}{I_F} = \frac{3.3\text{V} - 2.0\text{V}}{0.005\text{A}} = \frac{1.3\text{V}}{0.005\text{A}} = 260\,\Omega \quad (\text{Use standard } 270\,\Omega \text{ or } 330\,\Omega)$$

---

## 2. Pull-Up and Pull-Down Resistors

A digital input pin with high input impedance ($> 10\text{ M}\Omega$) will float unpredictably if left unconnected, antenna-picking electromagnetic noise from ambient air and toggling 0 and 1 randomly.

```
PULL-UP CONFIGURATION (Active LOW Button):
         3.3V
          │
         [ ] 10k Pull-Up Resistor
          │
          ├───► Microcontroller GPIO Input Pin
          │
        ┌─┴─┐
        │ O │ Pushbutton Switch
        └─┬─┘
          │
         GND
- When button is released: GPIO reads 3.3V (HIGH / 1).
- When button is pressed:  GPIO connected to GND (LOW / 0). Current = 3.3V / 10k = 0.33 mA.
```

---

## 3. Capacitors: Decoupling & Filtering

### Decoupling (Bypass) Rule:
Every microcontroller $V_{DD}$ power pin requires a local **$0.1\mu\text{F}$ (100nF) ceramic capacitor** placed within $3\text{mm}$ of the physical chip pin to provide instantaneous charge during high-speed transistor switching and suppress voltage dips. Bulk capacitors ($10\mu\text{F}$) are placed near power regulators.

---

## 4. MOSFETs as Digital Switches

Microcontroller GPIO pins cannot drive high-current loads (relays, motors, solenoids, or high-power LEDs). Use an N-Channel MOSFET in a **Low-Side Switch** configuration:

```
                  +12V Supply
                       │
                   ┌───┴───┐
                   │ Load  │ (Motor, Relay, or LED strip)
                   └───┬───┘
                       │
                       ├───────┐
                       │       │ [Flyback Diode: 1N4007 or Schottky]
                       │   ▲───┘ (Cathode to +12V, Anode to Drain)
                       ▼
                 Drain (D)
    3.3V GPIO ──[100R]── Gate (G)   N-Channel MOSFET (Logic Level)
                 Source (S)
                       │
                      GND
```
*Note: The flyback diode dissipates the reverse voltage spike ($V = -L \frac{di}{dt}$) generated when inductive coils turn off.*
