# 📡 Universal Asynchronous Receiver-Transmitter (UART / USART)

> Asynchronous serial framing, baud rate generators, overrun errors, and circular DMA ring buffers.

---

## 1. Asynchronous Protocol Framing (8-N-1)

Unlike SPI or I2C, UART transmits data without a dedicated clock wire. Sender and receiver synchronize on the falling edge of the **Start Bit**:

```
Idle (HIGH) ──┐      ┌───┬───┬───┬───┬───┬───┬───┬───┐      ┌─── Idle (HIGH)
              │START │D0 │D1 │D2 │D3 │D4 │D5 │D6 │D7 │ STOP │
              └──────┴───┴───┴───┴───┴───┴───┴───┴───┘
```
- **Baud Rate:** Bit rate per second (e.g. 115,200 baud = $\approx 8.68\,\mu\text{s}$ per bit).
- **Framing Error:** Receiver detects a '0' where a Stop bit ('1') was expected (often due to mismatched baud rates or clock drift $> 2\%$).
- **Overrun Error (`ORE`):** A new byte arrives before the CPU reads the previous byte from `USART->DR`.

---

## 2. Baud Rate Divisor Formula

$$USARTDIV = \frac{f_{CK}}{16 \times \text{Baud Rate}}$$

For an APB1 peripheral clock $f_{CK} = 16\text{ MHz}$ and desired Baud = 115,200:
$$USARTDIV = \frac{16,000,000}{16 \times 115,200} \approx 8.6805$$
- Mantissa = 8 (`0x08`)
- Fraction = $0.6805 \times 16 \approx 11$ (`0x0B`)
- Value written to `USART_BRR`: `0x008B` *(Always verify in vendor Reference Manual!)*

---

## 🛠️ Starter Exercise
Build a non-blocking UART echo server: When characters arrive via `USART2_IRQHandler`, push them into a circular FIFO buffer. In the main loop, pop characters and transmit them back out.
