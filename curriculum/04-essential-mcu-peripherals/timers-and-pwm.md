# ⏱️ Hardware Timers & Pulse Width Modulation (PWM)

> Prescalers, Auto-Reload Registers, Input Capture, and Output Compare PWM.

---

## 1. Hardware Timer Core Mechanics

A hardware timer is fundamentally a 16-bit or 32-bit digital counter driven by a prescaled clock:

```
Timer Clock Input (f_TIM = 16 MHz)
       │
       ▼
┌─────────────────────────────────┐
│ Prescaler Register (TIMx->PSC)  │ Divides clock by (PSC + 1)
└──────────────┬──────────────────┘
               │ (f_CNT)
               ▼
┌─────────────────────────────────┐
│ Counter Register (TIMx->CNT)    │ Increments on each tick
└──────────────┬──────────────────┘
               │ Matches?
               ├───► [ Auto-Reload Register: TIMx->ARR ] ──► Update Interrupt / Resets CNT to 0
               │ Matches?
               └───► [ Capture/Compare Register: CCRx  ] ──► Output PWM Pin Toggles
```

### Timer Frequency Formula:
$$f_{overflow} = \frac{f_{TIM}}{(PSC + 1) \times (ARR + 1)}$$

---

## 2. Generating Pulse Width Modulation (PWM)

In **PWM Mode 1** (Edge-aligned):
- While `TIMx->CNT < TIMx->CCR1`: Output pin is **HIGH (Active)**.
- While `TIMx->CNT >= TIMx->CCR1`: Output pin is **LOW (Inactive)**.
- When `CNT` reaches `ARR`: `CNT` resets to 0 and output pin returns HIGH.

```
       ◄─────────────── Timer Period (ARR + 1) ───────────────►
       ┌───────────────────────┐                               ┌───
Output:│      HIGH             │              LOW              │
       └───────────────────────┴───────────────────────────────┴───
       ◄── Pulse Width (CCR) ──►
```

### Duty Cycle Formula:
$$\text{Duty Cycle (\%)} = \frac{CCR1}{ARR + 1} \times 100\%$$

---

## 🛠️ Starter Exercise
Configure STM32 `TIM2` Channel 1 (mapped to `PA0`) to output a $20\text{ kHz}$ PWM signal for motor speed control with a variable duty cycle from 10% to 90%.
