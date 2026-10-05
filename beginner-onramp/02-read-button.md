# 🔘 Project 2: Read a Pushbutton (Digital Inputs & The "Floating Pin" Trap)

> **Goal:** Connect a physical pushbutton switch, detect when a human finger presses it, and turn an LED on and off accordingly.  
> **Prerequisites:** Project 1 (Blink an LED).

---

### 🧠 The Real-World Intuition: Why Microcontroller Pins Float Like Antennas

When you configure a pin as an **OUTPUT**, the chip firmly drives it to either $5\text{V}$ or $0\text{V}$.  
However, when you configure a pin as an **INPUT**, the chip turns into a high-sensitivity listener.

If you connect a button between a pin and $5\text{V}$, what happens when the button is **not** pressed?  
The pin is connected to **absolutely nothing!**

```
                   [ 5 Volts ]
                        │
                     ──o  o──  Pushbutton (Open / Not Pressed)
                        │
                        ▼
            Microcontroller Input Pin ──── ? ? ?  (Connected to NOTHING!)
            Acts like an antenna picking up static electricity, Wi-Fi waves,
            and mains hum, erratically jumping between 1 and 0!
```

This is called a **Floating Pin**. A floating input pin acts like a radio antenna picking up electromagnetic noise from the room. Your code will see ghost button presses even when nobody is in the room!

### 💡 The Solution: The Pull-Up Resistor (The Screen Door Spring)
To fix this, we connect a gentle $10\text{ k}\Omega$ resistor between the pin and $5\text{V}$.  
Think of it like a **gentle spring on a screen door**:
- When nobody touches the door, the spring holds the door safely shut (**HIGH = $5\text{V}$**).
- When you press the pushbutton down to Ground ($0\text{V}$), you easily overcome the weak spring, pulling the pin to Ground (**LOW = $0\text{V}$**).

```
                  [ 5 Volts ] (VCC)
                        │
                  ┌─────┴─────┐
                  │ 10k RESISTOR (The Spring)
                  └─────┬─────┘
                        ├───► Microcontroller Input Pin (Sees 5V when button is idle!)
                        │
                     ──o  o──  Pushbutton
                        │
                     Ground [ 0V ] (When pressed, pin drains to 0V!)
```

This is called **Active-Low Logic**:
* **Button Released:** Pin reads `HIGH` ($5\text{V}$).
* **Button Pressed:** Pin reads `LOW` ($0\text{V}$).

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

Most modern microcontrollers have internal pull-up resistors built right into the silicon chip! You can turn them on with a single line of code (`INPUT_PULLUP`).

### Step 1: Open Wokwi
Open [https://wokwi.com](https://wokwi.com) $\to$ Select **Arduino Uno**.

### Step 2: Add a Pushbutton to the Diagram
1. In the simulator, click the **"+" (Add a new part)** button at the top.
2. Select **Pushbutton**.
3. Click on the button's left terminal and drag a wire to Arduino **Pin 2**.
4. Click on the button's right terminal and drag a wire to Arduino **GND**.

### Step 3: Paste the Code into `sketch.ino`

```cpp
// Project 2: Read a Pushbutton with Internal Pull-Up Resistor
const int BUTTON_PIN = 2;   // Pushbutton connected between Pin 2 and GND
const int LED_PIN    = 13;  // Built-in LED on Pin 13

void setup() {
  // 1. Tell the chip Pin 13 drives the LED
  pinMode(LED_PIN, OUTPUT);

  // 2. Tell the chip Pin 2 listens for button presses.
  // 'INPUT_PULLUP' activates the internal silicon spring pulling the pin to 5V!
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // 3. Read the electrical state of Pin 2
  int buttonState = digitalRead(BUTTON_PIN);

  // 4. Remember: Active-Low! When pressed, the pin is pulled to LOW (0V).
  if (buttonState == LOW) {
    digitalWrite(LED_PIN, HIGH); // Turn LED ON while button is pressed!
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn LED OFF when button is released
  }
}
```

### Step 4: Run and Test
Click **Play (▶)**. Click and hold the pushbutton on the screen. The LED lights up instantly! Let go of your mouse, and the LED turns off.

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

On the PIC16F877A:
- Pushbutton `RB0` is wired to ground on the PICGenios board.
- LED `RB1` is our indicator light.
- We enable the Port B internal pull-up resistors by clearing the `nRBPU` bit in `OPTION_REG`.

### Paste the Code into `main.c`:

```c
// PIC16F877A - Project 2: Button Input & LED Control
#define _XTAL_FREQ 4000000

#include <xc.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF

void main(void) {
    // 1. Configure Pin Directions (0 = Output, 1 = Input)
    TRISBbits.TRISB0 = 1;  // Pin RB0 is an INPUT (Button)
    TRISBbits.TRISB1 = 0;  // Pin RB1 is an OUTPUT (LED)

    // 2. Enable Port B Internal Pull-Up Resistors
    // Clearing the 'nRBPU' bit in OPTION_REG turns on internal pull-up springs!
    OPTION_REGbits.nRBPU = 0;

    // 3. Superloop: Continuously check button status
    while (1) {
        // Active-Low: 0 means finger is pressing the button
        if (PORTBbits.RB0 == 0) {
            PORTBbits.RB1 = 1; // Turn LED ON
        } else {
            PORTBbits.RB1 = 0; // Turn LED OFF
        }
    }
}
```

Build the project in MPLAB X, load the `.hex` into PICSimLab, and press button `SW1` (connected to `RB0`). LED `D1` lights up while pressed!

---

## 🏀 What is Switch "Bouncing"? (The Ping-Pong Ball Effect)

When you press a mechanical pushbutton, two tiny flexible metal leaves collide inside the switch plastic housing.  
Under a microscope, they don't make clean contact instantly. Instead, they **bounce off each other like a ping-pong ball** dropped on concrete for 5 to 20 milliseconds!

```
Ideal Switch Press:       HIGH ───────┐
                                      └──────── LOW (Clean transition)

Real Mechanical Switch:   HIGH ──┐ ┌─┐ ┌──┐
                                 └─┘ └─┘  └─── LOW (Bounces 20+ times in 10ms!)
```

Because your microcontroller CPU runs millions of times per second, it will see 10 or 20 distinct button presses in a fraction of a millisecond!

### 💡 Simple Software Debounce:
When you detect a press, wait 20 milliseconds (`delay(20)` or `__delay_ms(20)`) for the mechanical metal leaves to stop vibrating before checking the pin again!

---

## 🚀 Beginner Experiments & Challenges

1. **Invert the Logic:** Make the LED stay ON normally, and turn OFF only when you press the button.
2. **The Toggle Switch (Push-On, Push-Off):**  
   Can you make the button work like a bedroom lamp switch? (Click once $\to$ LED turns ON and stays ON even after you release your finger. Click again $\to$ LED turns OFF!).  
   *(Hint: You will need a variable to store the previous button state and a small debounce delay!)*

---

➡️ **Next Project:** [**Project 3: Serial "Hello World" (Talking to Your PC Screen)**](03-serial-hello.md)
