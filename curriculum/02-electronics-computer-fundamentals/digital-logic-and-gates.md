# 🔢 Digital Logic, Boolean Algebra & Flip-Flops

> From raw transistors to logic gates, combinational logic, and synchronous state storage.

---

## 1. Fundamental Logic Gates

| Gate | Symbol | Truth Table | Boolean Equation | Hardware Behavior |
| :---: | :---: | :--- | :--- | :--- |
| **AND** | `&` | `0&0=0, 0&1=0, 1&0=0, 1&1=1` | $Y = A \cdot B$ | Output HIGH only if both inputs are HIGH |
| **OR**  | `\|` | `0\|0=0, 0\|1=1, 1\|0=1, 1\|1=1` | $Y = A + B$ | Output HIGH if either input is HIGH |
| **NOT** | `~` | `~0=1, ~1=0` | $Y = \bar{A}$ | Inverts digital logic state |
| **XOR** | `^` | `0^0=0, 0^1=1, 1^0=1, 1^1=0` | $Y = A \oplus B$ | Output HIGH if inputs are different |

---

## 2. Flip-Flops & Synchronous Registers

While combinational logic evaluates instantaneously, sequential logic stores state synchronized to a clock edge:

```
D Flip-Flop (The fundamental building block of CPU registers):
       ┌──────────┐
  D ───┤ D      Q ├─── Q (Stored State)
       │          │
CLK ───┤ >     !Q ├─── !Q
       └──────────┘
```

- **Rising Clock Edge:** On the rising edge of `CLK`, the logic level present at `D` is captured and latched to output `Q`. Between clock edges, changes on `D` have zero effect.
- **CPU Registers:** A 32-bit register (`R0` or `GPIOA->ODR`) is simply an array of 32 D flip-flops sharing a single clock gating signal.

---

## 3. Timing Constraints: Setup & Hold Time

In digital electronics, data cannot change at the exact instant a clock edge arrives:

```
Data Line:  ───────[ Valid Data In ]─────────
                   ├─── t_su ───┼─── t_hold ───┤
Clock Line: ────────────────────▲───────────────
                           Clock Edge
```

- **Setup Time ($t_{su}$):** Minimum duration data must remain stable **before** the clock edge.
- **Hold Time ($t_h$):** Minimum duration data must remain stable **after** the clock edge.
- **Metastability:** If data changes within the $t_{su} + t_h$ window, the flip-flop can enter a metastable state—oscillating between 0 and 1 for an indeterminate time before settling, causing catastrophic random crashes. This is why asynchronous external signals (like button presses or UART RX) must be synchronized through two cascaded flip-flops before entering CPU logic.
