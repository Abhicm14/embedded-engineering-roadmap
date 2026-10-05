# 📡 Project 3: Serial "Hello World" (Talking from Chip to PC Screen)

> **Goal:** Send text messages and numbers from the microcontroller over a serial wire and read them live on your computer screen.  
> **Prerequisites:** Project 1 (Blink an LED).

---

### 🧠 The Real-World Intuition: Two Walkie-Talkies and the Baud Rate

Blinking LEDs is fun, but what if your microcontroller needs to tell you: *"The temperature is 24°C"*, or *"Warning: Battery is at 10%"*?  
You can't blink an LED 24 times every second without going crazy trying to count!

That's why microcontrollers have a **UART (Universal Asynchronous Receiver-Transmitter)**.  
It uses just two wires to talk to your computer:

```
    Microcontroller                                    Computer (PC)
  ┌─────────────────┐                                ┌─────────────────┐
  │                 │    TX Wire (Mouth to Ear)      │                 │
  │  TX (Mouth) ────┼───────────────────────────────►│  RX (Ear)       │
  │                 │                                │                 │
  │  RX (Ear)   ◄───┼────────────────────────────────┼──── TX (Mouth)  │
  │                 │    RX Wire (Ear to Mouth)      │                 │
  │  GND (Ref)  ────┼────────────────────────────────┼──── GND (Ref)   │
  └─────────────────┘  Common Ground (Shared floor)  └─────────────────┘
```

#### 1. Why Cross TX and RX?
Think about how two humans converse:
- Your **Mouth (TX = Transmit)** must speak into the other person's **Ear (RX = Receive)**.
- If you connect TX to TX (mouth to mouth), nobody hears anything!

#### 2. What is the "Baud Rate"? (The Talking Speed)
Because there is no clock wire coordinating the two devices, both ends must agree in advance on **how fast they will talk**. This speed is called the **Baud Rate** (bits per second).
- Standard speeds include: `9600`, `19200`, `57600`, and `115200` baud.
- **The Alien Chatter Trap:** If your microcontroller talks at 9600 baud, but your computer serial monitor is set to 115200 baud, your screen will fill with crazy alien symbols like `@?#!` because the receiver is sampling at the wrong rhythm!

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

When an Arduino Uno connects to your PC via USB, an onboard converter chip transforms the hardware UART pins (Pin 0 and Pin 1) directly into a virtual USB COM port!

### Step 1: Paste the Code into `sketch.ino`

```cpp
// Project 3: Serial Communication "Hello World"
int loopCounter = 0; // A counter variable to track how many seconds have passed

void setup() {
  // 1. Initialize serial communication at 9600 baud (bits per second)
  Serial.begin(9600);

  // 2. Print a friendly welcome message once upon boot
  Serial.println("=========================================");
  Serial.println("🎉 Microcontroller is ALIVE and talking!");
  Serial.println("=========================================");
}

void loop() {
  // 3. Print our message and current counter value
  Serial.print("Heartbeat telemetry packet #");
  Serial.print(loopCounter);
  Serial.println(" - All systems nominal.");

  // 4. Increment counter
  loopCounter = loopCounter + 1;

  // 5. Wait 1 second before sending the next telemetry report
  delay(1000);
}
```

### Step 2: Open the Serial Monitor and Run
1. Look at the bottom of the Wokwi screen. Click the **"Serial Monitor"** tab to open the terminal drawer.
2. Click **Play (▶)**.
3. Watch your computer print live messages every second:
   ```text
   =========================================
   🎉 Microcontroller is ALIVE and talking!
   =========================================
   Heartbeat telemetry packet #0 - All systems nominal.
   Heartbeat telemetry packet #1 - All systems nominal.
   Heartbeat telemetry packet #2 - All systems nominal.
   ```

### 🔍 Code Breakdown:
- `Serial.begin(9600);`: Boots up the silicon UART hardware and configures its internal clock divider for 9600 bits per second.
- `Serial.print("text");`: Sends characters across the wire without starting a new line.
- `Serial.println("text");`: Sends characters and automatically appends a Newline (`\r\n`), moving the cursor down to the next line.

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

On the PIC16F877A, the hardware USART peripheral is located on pins **RC6 (TX)** and **RC7 (RX)**.

### Paste the Code into `main.c`:

```c
// PIC16F877A - Project 3: Hardware UART Telemetry
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF

// 1. Function to initialize USART hardware for 9600 baud at 4 MHz
void UART_Init(void) {
    TRISCbits.TRISC6 = 0; // RC6 is TX (Output)
    TRISCbits.TRISC7 = 1; // RC7 is RX (Input)

    // Formula from Datasheet: SPBRG = (Fosc / (16 * Baud)) - 1
    // (4,000,000 / (16 * 9600)) - 1 = 25.04 -> 25
    SPBRG = 25;
    TXSTAbits.BRGH = 1;   // High-speed baud rate generator
    TXSTAbits.TXEN = 1;   // Enable transmitter
    RCSTAbits.SPEN = 1;   // Enable serial port pins
}

// 2. Function to transmit a single character
void UART_Write_Char(char c) {
    while (!TXSTAbits.TRMT); // Wait until the transmit buffer is completely empty
    TXREG = c;               // Drop the byte into the hardware transmit register!
}

// 3. Function to transmit a whole string of text
void UART_Write_String(const char *str) {
    while (*str) {
        UART_Write_Char(*str++);
    }
}

void main(void) {
    UART_Init();
    UART_Write_String("\r\n=== PIC16F877A Serial Telemetry Online ===\r\n");

    int packetCount = 0;
    char buffer[40];

    while (1) {
        sprintf(buffer, "Telemetry Ping #%d: Status OK\r\n", packetCount++);
        UART_Write_String(buffer);
        __delay_ms(1000);
    }
}
```

### View Output in PICSimLab:
1. Build the `.hex` file in MPLAB X and load it into PICSimLab.
2. In the top menu of PICSimLab, go to **Modules $\to$ Serial Terminal**.
3. Set the baud rate to **9600** and select **Connect**.
4. Watch the telemetry stream appear on the virtual terminal screen!

---

## 🚀 Beginner Experiments & Challenges

1. **The Fast Counter:** Change the delay from 1000ms to 100ms. Notice how rapidly the terminal scrolls!
2. **Interactive Echo (Two-Way Chat):**  
   In Wokwi, can you modify the code to read a character typed by the user in the Serial Monitor and send it right back?  
   *(Hint: Use `if (Serial.available() > 0) { char incoming = Serial.read(); Serial.print(incoming); }`)*.
3. **Baud Rate Sabotage:** In Wokwi or PICSimLab, set the Serial Monitor to 115200 while leaving the code at 9600. Look at the garbled symbols on screen—now you will instantly recognize a baud rate mismatch whenever you see one in real life!

---

➡️ **Next Project:** [**Project 4: The Volume Knob (Analog Signals & ADC)**](04-potentiometer-knob.md)
