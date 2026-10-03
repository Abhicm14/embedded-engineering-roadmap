# ⚡ Type Qualifiers: Volatile, Const, Static & Extern

> How compiler optimization interacts with hardware registers, interrupt routines, and storage classes.

---

## 1. The `volatile` Qualifier

The `volatile` keyword tells the C compiler:
> *"The value of this variable can change at any time due to external hardware or asynchronous execution. Do not optimize reads or writes away, and do not cache its value in a CPU core register."*

### What Happens Without `volatile`?
```c
// Incorrect implementation:
uint8_t g_flag = 0;

void EXTI0_IRQHandler(void) {
    g_flag = 1; // Set by button press
}

int main(void) {
    while (g_flag == 0) {
        // Compiler compiles this into:
        // LDR R0, [g_flag]
        // loop: CMP R0, #0 ; BEQ loop  (Infinite loop! It never re-reads RAM!)
    }
}
```

### Correct Implementation:
```c
volatile uint8_t g_flag = 0; // Forces CPU to re-read from memory on every loop iteration
```

### The 3 Mandatory Uses of `volatile`:
1. **Memory-Mapped Peripheral Registers:** (e.g. `volatile uint32_t * const UART_DR = (uint32_t *)0x40011004;`)
2. **Global Variables Modified in an ISR:** Flags and counters shared with main loop.
3. **Multi-threaded Shared Variables:** Global state accessed across RTOS tasks without locks.

---

## 2. The `const` Qualifier

- In desktop programming, `const` prevents variable modification.
- In embedded systems, `const` does something even more crucial: **it places the data into Flash memory (`.rodata`) instead of consuming precious SRAM!**

```c
// Consumes 256 bytes of precious SRAM:
uint8_t sin_table[256] = { ... };

// Stored permanently in Flash memory, consuming 0 bytes of SRAM:
const uint8_t sin_table[256] = { ... };
```

---

## 3. Pointers and `const` Combinations

Read pointer declarations from right to left:

| Declaration | Meaning | Can change address (`p++`)? | Can change data (`*p = x`)? |
| :--- | :--- | :---: | :---: |
| `const char *p` | Pointer to constant character | ✅ YES | ❌ NO |
| `char * const p` | Constant pointer to character | ❌ NO | ✅ YES |
| `const char * const p` | Constant pointer to constant character | ❌ NO | ❌ NO |

---

## 4. `static` and `extern`

- **`static` at File Scope:** Limits variable or function visibility strictly to the current `.c` file (encapsulation / private API).
- **`static` inside a Function:** Preserves the variable's value across multiple invocations (stored in `.data` or `.bss`, not on the stack).
- **`extern`:** Informs the compiler that the symbol is defined in another translation unit and will be resolved during the linking phase.
