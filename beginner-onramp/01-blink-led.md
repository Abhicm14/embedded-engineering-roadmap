# 💡 Project 1: Blink an LED (The "Hello World" of Hardware)

> **Goal:** Turn an electrical light ON, pause, turn it OFF, pause, and repeat forever in a loop.  
> **Prerequisites:** None! You just need a web browser or PICSimLab.

---

### 🧠 The Real-World Intuition: Water Pipes and Light Valves

Before looking at any code, let's understand what is physically happening on the circuit board:

```
    Microcontroller Pin (The Faucet)
               │ [ 5 Volts ]
               ▼
        ┌─────────────┐
        │   RESISTOR  │  <── Acts like a narrow pipe to restrict water flow!
        └──────┬──────┘      (Without this, the current blows up the LED!)
               │
               ▼
             ──┬──
             \   /   LED (The Water Turbine Light Valve)
              \ /    Only lets current flow in ONE direction (Anode + to Cathode -)
             ──┴──
               │
               ▼
            Ground [ 0 Volts ] (The Drain)
```

1. **Voltage ($5\text{V}$ or $3.3\text{V}$):** Think of voltage like water pressure in a pipe. When a pin is set to **HIGH** ($5\text{V}$), the chip turns on a faucet, pushing electrical pressure out. When set to **LOW** ($0\text{V}$), the faucet is turned off.
2. **The LED (Light Emitting Diode):** A diode is like a **one-way valve**. Electricity can only flow through it in one direction (from the longer positive leg called the **Anode** to the shorter negative leg called the **Cathode**).
3. **The Current-Limiting Resistor:** An LED has almost zero electrical resistance on its own. If you connect it directly to $5\text{V}$ without a resistor, a massive rush of electricity flows through it like opening a high-pressure fire hydrant into a toy balloon—*POP!* The LED burns out in milliseconds. A $220\,\Omega$ or $330\,\Omega$ resistor acts as a narrow pipe restrictor, keeping the current safe and cool (around $10$ to $15\text{ mA}$).

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

### Step 1: Open the Simulator
1. Open [https://wokwi.com](https://wokwi.com) in your web browser.
2. Scroll to **"Arduino Uno"** and click it to open a fresh simulation workspace.

### Step 2: Paste the Code
Replace whatever code is in `sketch.ino` with this complete, working program:

```cpp
// Project 1: Blink an LED
// Pin 13 is connected to the built-in LED on the Arduino Uno board.

const int LED_PIN = 13;

void setup() {
  // 1. Tell the chip that pin 13 is an OUTPUT (a faucet that pushes voltage)
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // 2. Turn the faucet ON (send 5 Volts to the LED)
  digitalWrite(LED_PIN, HIGH);
  
  // 3. Freeze time for 500 milliseconds (half a second)
  delay(500);
  
  // 4. Turn the faucet OFF (send 0 Volts)
  digitalWrite(LED_PIN, LOW);
  
  // 5. Freeze time for another 500 milliseconds
  delay(500);
  
  // The microcontroller automatically jumps back to the top of loop() forever!
}
```

### Step 3: Run the Simulation
Click the green **Play (▶)** button at the top of the simulator. You will see the small orange LED marked **"L"** on the Arduino board start blinking on and off every half second!

### 🔍 Line-by-Line Code Breakdown:
- `const int LED_PIN = 13;`: Gives the number `13` a friendly human name so our code is readable.
- `void setup() { ... }`: A special function that runs **once** when the chip first powers on. This is where we configure pins.
- `pinMode(LED_PIN, OUTPUT);`: Microcontroller pins can either listen (INPUT) or speak/drive (OUTPUT). We set this pin to `OUTPUT` so it can push electricity out.
- `void loop() { ... }`: A function that repeats over and over in an infinite loop for as long as the chip has power.
- `digitalWrite(LED_PIN, HIGH);`: Turns on the electronic switch inside the silicon, sending $5\text{V}$ to pin 13.
- `delay(500);`: Pauses the CPU for 500 milliseconds ($0.5$ seconds) so your human eyes can see the light before it changes.
- `digitalWrite(LED_PIN, LOW);`: Turns off the switch, pulling the pin to $0\text{V}$ (Ground), extinguishing the light.

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

If you are following the [**PIC Microcontroller Track**](../pic-mplab-xc8/README.md) using MPLAB X and XC8:

### Step 1: Paste the Code into `main.c`

```c
// PIC16F877A - Project 1: Bare-Metal LED Blink
#define _XTAL_FREQ 4000000 // Tell compiler we are using a 4 MHz clock

#include <xc.h>

// Configuration Bits (turns off watchdog timer and sets standard crystal oscillator)
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF

void main(void) {
    // Step 1: Set Pin RB0 as an OUTPUT
    // In PIC chips, 'TRIS' sets the direction (0 = Output, 1 = Input)
    // Memory trick: '0' looks like 'O' for Output; '1' looks like 'I' for Input!
    TRISBbits.TRISB0 = 0;

    // Step 2: The Infinite Superloop
    while (1) {
        PORTBbits.RB0 = 1;      // Turn LED ON (Set pin to 5 Volts)
        __delay_ms(500);         // Wait 500 milliseconds
        
        PORTBbits.RB0 = 0;      // Turn LED OFF (Set pin to 0 Volts)
        __delay_ms(500);         // Wait 500 milliseconds
    }
}
```

### Step 2: Run in PICSimLab
1. In MPLAB X, click **Clean and Build** to generate the `.hex` file.
2. Open **PICSimLab**, select **Board 1: PICGenios**, microcontroller **PIC16F877A**.
3. Go to **File $\to$ Load Hex** and select your compiled `.hex`.
4. Watch LED `D0` (connected to Port B Pin 0) blink rhythmically on the virtual board!

### 🔍 How PIC Registers Work:
- `TRISBbits.TRISB0 = 0;`: **TRIS** stands for *Tri-State*. It controls the direction of Port B. Setting bit 0 to `0` configures pin `RB0` as an **Output**.
- `PORTBbits.RB0 = 1;`: **PORT** is the data register. Setting bit 0 to `1` charges the microscopic transistor switch, outputting $5\text{V}$ directly onto the pin.

---

## 🚀 Beginner Experiments & Challenges

Don't just run the code—experiment with it to build your muscle memory:

1. **The Fast Strobe:** Change both `delay(500)` calls to `delay(50)`. What happens? (The LED blinks like a strobe light!).
2. **The Heartbeat:** Can you make the LED pulse like a human heartbeat?  
   *(Hint: Blink ON for 100ms, OFF for 100ms, ON for 100ms, then OFF for 700ms!)*.
3. **SOS Distress Signal:** In Morse code, SOS is `... --- ...` (3 fast blinks, 3 slow blinks, 3 fast blinks). Write the loop to repeat this pattern!

---

## ⚠️ Common Beginner Mistakes

* **LED inserted backwards:** On a physical breadboard, if you flip the LED around, no light turns on because diodes only pass current in one direction. The longer leg is always positive (+).
* **Missing delay:** If you forget `delay()`, the LED turns on and off millions of times per second. To human eyes, it will look like a solid dim light because human retinas cannot see changes faster than 60 Hz!
* **Forgetting TRIS / pinMode:** If you try to write `HIGH` to a pin without first setting its direction to `OUTPUT`, the pin remains an `INPUT` (high-impedance) and cannot provide enough current to light the bulb.

---

➡️ **Next Project:** [**Project 2: Read a Pushbutton (Digital Inputs & Pull-Up Resistors)**](02-read-button.md)
