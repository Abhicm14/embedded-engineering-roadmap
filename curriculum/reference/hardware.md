# 🔌 Comprehensive Hardware Taxonomy for Embedded Systems

> Broad taxonomy tree of electrical physics, circuit design, PCB engineering, test gear, and FPGA digital synthesis.

---

## 1. Electronics Physics & Digital Design

- **Electric Circuits & Component Physics:**
  - Ohm's law, Kirchhoff's current and voltage laws (KCL / KVL), Thévenin and Norton equivalents.
  - Passives: Resistors (E-series, precision current shunts, pull-ups), Capacitors (dielectrics, MLCC DC-bias derating, ESR, decoupling networks), Inductors (saturation current, ferrite beads).
  - Actives: Diodes (Schottky, Zener, TVS protection), Transistors (BJT NPN/PNP, MOSFET N-channel/P-channel, $V_{GS(th)}$, $R_{DS(on)}$), Op-Amps (inverting, non-inverting, buffers, comparators).
- **Digital Logic & Computer Architecture:**
  - Logic gates (AND, OR, NOT, XOR, NAND, NOR), Boolean algebra, De Morgan's laws.
  - Combinational blocks: Multiplexers, decoders, priority encoders, adders.
  - Sequential blocks: SR latches, D flip-flops, JK flip-flops, counters, shift registers.
  - Microprocessor architectures: RISC vs CISC, Harvard vs von Neumann, pipelining, hazards (data, structural, control), branch prediction, cache hierarchies.
  - Processor cores: ARM Cortex-M/A/R series, RISC-V (RV32I, RV64G), x86.

---

## 2. Power Supply Architecture & Energy Management

- **Linear Regulators (LDO):** Low ripple, clean analog power, drop-out voltage, thermal dissipation calculations.
- **Switched-Mode Power Supplies (SMPS):** Buck (step-down), Boost (step-up), Buck-Boost, inductor sizing, switching frequency selection, EMI snubber circuits.
- **Battery Management:** Li-Ion/LiPo CC/CV charging profiles, protection circuits (under/over-voltage, over-current), fuel gauge Coulomb counters, CR2032 coin-cell pulsing.

---

## 3. Laboratory Test & Measurement Equipment

- **Digital Multimeter (DMM):** Voltage measurement, low-current measurement ($\mu\text{A}$ sleep states), resistance, diode drop, and continuity testing.
- **Digital Storage Oscilloscope (DSO):** Bandwidth rules ($5\times$ rule), sampling rate, AC/DC coupling, $10\text{X}$ probe compensation, triggering (edge, pulse-width, runt), FFT noise analysis.
- **Logic & Protocol Analyzers:** Multi-channel digital state capture, protocol decoding (UART, SPI, I2C, CAN, USB), trigger conditions (packet match).
- **Bench Power Supplies & Electronic Loads:** Current limiting (constant current mode), transient load step testing.

---

## 4. Hardware Prototyping, PCB Design & Manufacturing

- **Prototyping:** Solderless breadboards, stripboards, perfboards, wire-wrapping, breadboard stray capacitance limits ($< 5\text{ MHz}$).
- **Schematic Capture & PCB Layout (KiCad / Altium):**
  - Schematic hierarchy, net labels, power flags, footprint assignment.
  - PCB stackups: 2-layer vs 4-layer (Signal - GND - Power - Signal).
  - Continuous ground planes, return current path minimization, avoiding ground slot crossings.
  - High-speed routing: $50\,\Omega$ single-ended traces, $90\,\Omega$ USB differential pairs, $120\,\Omega$ CAN differential pairs, length matching.
  - Design for Manufacturing (DFM) and Design for Assembly (DFA): Clearances, annular rings, teardrops, test points, fiducials.
- **Soldering & Rework:** Through-hole soldering, SMD soldering (0805, 0603, QFP, QFN), hot-air rework, solder wick, flux types (no-clean vs rosin).

---

## 5. FPGA & Digital Hardware Description (HDL)

- **Hardware Description Languages:** Verilog, SystemVerilog, VHDL.
- **FPGA Architecture:** Configurable Logic Blocks (CLBs), Look-Up Tables (LUTs), Flip-Flops, Block RAM (BRAM), DSP slices, Clock Management Tiles (PLL/MMCM).
- **Synthesis & Implementation Flow:** RTL Code $\rightarrow$ Synthesis $\rightarrow$ Translation $\rightarrow$ Mapping $\rightarrow$ Place & Route $\rightarrow$ Bitstream Generation.
- **Educational References:** Nandland (Go Board), HDLBits interactive challenges, FPGA to ASIC journey.
