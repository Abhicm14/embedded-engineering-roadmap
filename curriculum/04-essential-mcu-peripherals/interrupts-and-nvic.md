# ⚡ External Interrupts (EXTI) & ISR Design Rules

> Connecting pin edge transitions to the NVIC and writing rock-solid Interrupt Service Routines.

---

## 1. External Interrupt Architecture (EXTI)

Microcontrollers allow physical pin edge transitions (Rising, Falling, or Both) to asynchronously interrupt the CPU:

```
GPIO Pin (PA0) ──► SYSCFG Pin Mux ──► EXTI0 Line ──► Edge Detector ──► NVIC IRQ #6
                                                          │
                                                [Rising / Falling]
```

---

## 2. The 5 Golden Rules of ISR Design

1. **Keep ISRs Ultra-Short:** Perform minimal work in the ISR (e.g. read hardware data register, clear the pending interrupt flag, push to a ring buffer, and exit).
2. **Never Call Blocking Functions:** Never call `Delay()`, `printf()`, or busy-wait on other hardware flags inside an ISR.
3. **Never Call Non-Reentrant Functions:** `malloc()`, `free()`, and standard C library routines maintain global structures that become corrupted if called from an ISR.
4. **All Shared Variables Must Be `volatile`:** If a variable is written in an ISR and read in the main loop, mark it `volatile` to prevent compiler register caching.
5. **Always Clear Hardware Pending Flags First:** If you fail to clear the interrupt pending bit in `EXTI->PR` or peripheral status registers before exiting the ISR, the CPU will immediately re-enter the ISR in an infinite loop!

---

## 🛠️ Starter Exercise
Configure `PA0` with internal pull-up and route it to `EXTI0`. Set `EXTI0` to trigger on a falling edge (button press). Inside `EXTI0_IRQHandler`, clear the pending bit and toggle an LED flag.
