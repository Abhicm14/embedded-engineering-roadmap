# 📍 C Syntax & Pointers for Embedded Firmware

> Understanding memory addresses, pointer arithmetic, generic pointers, and function pointers.

---

## 1. What is a Pointer?

In embedded systems, a pointer is simply an integer value that represents a physical hardware address in the microcontroller's memory map (Flash, SRAM, or peripheral register bus).

```c
#include <stdint.h>

uint32_t val = 0x12345678;
uint32_t *ptr = &val; // ptr stores the memory address of val

// Dereferencing: reading or writing the data at that address
*ptr = 0xAABBCCDD;
```

---

## 2. Pointer Arithmetic

Pointer arithmetic scales automatically by the size of the underlying data type:
- If `p` is `uint8_t*` (1 byte), `p + 1` increments the address by **1 byte**.
- If `p` is `uint16_t*` (2 bytes), `p + 1` increments the address by **2 bytes**.
- If `p` is `uint32_t*` (4 bytes), `p + 1` increments the address by **4 bytes**.

```c
uint32_t buffer[4] = {10, 20, 30, 40};
uint32_t *p = buffer;

uint32_t second = *(p + 1); // Reads 20 (address + 4 bytes)
```

---

## 3. Generic Pointers (`void*`)

`void*` is a pointer to an untyped memory location. It is used extensively in HAL drivers and RTOS queues to pass arbitrary data payloads:
- You cannot dereference a `void*` directly without casting it to a concrete type.
- You cannot perform pointer arithmetic on standard `void*` (unless using GNU C extensions where `sizeof(void) == 1`).

```c
void Buffer_Clear(void *dest, uint8_t fill_byte, size_t len) {
    uint8_t *p = (uint8_t *)dest;
    while (len--) {
        *p++ = fill_byte;
    }
}
```

---

## 4. Function Pointers

Function pointers store the entry address of executable code in Flash memory. They are essential for:
1. **Interrupt Vector Tables:** Array of function pointers invoked by the CPU core upon hardware exceptions.
2. **Callbacks:** Allowing a peripheral driver (e.g. UART RX) to notify higher-level application logic when data arrives.
3. **Finite State Machines (FSM):** Clean dispatch tables without massive `switch-case` statements.

```c
// Prototype of function pointer: returns void, takes uint8_t parameter
typedef void (*UART_RxCallback_t)(uint8_t received_byte);

static UART_RxCallback_t s_rx_callback = NULL;

void UART_RegisterCallback(UART_RxCallback_t cb) {
    s_rx_callback = cb;
}

// Inside UART Interrupt Service Routine:
void USART1_IRQHandler(void) {
    uint8_t data = USART1->DR;
    if (s_rx_callback != NULL) {
        s_rx_callback(data); // Invoke callback
    }
}
```
