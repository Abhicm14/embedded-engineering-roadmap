# 🌱 True Beginner On-Ramp — Your First Steps in Embedded Systems

> **Zero Pre-Existing Knowledge Required • 100% Free Simulation • Hands-On from Minute 1**  
> *"You don't need a degree in electrical engineering or 10 years of programming to make a chip blink its first LED. Start here, build real confidence, and have fun!"*

---

### 👋 Welcome! Is This Where You Should Start?

If you are:
- A high school student curious about robotics, drones, or game controllers,
- A computer science or college student who has never touched a physical hardware circuit,
- A self-taught programmer with zero C language background, or
- Someone who opened a 1,000-page microcontroller datasheet and felt completely lost...

**You are in the right place!** 

Embedded engineering can feel intimidating because textbooks throw giant acronyms at you: *ALU, NVIC, DMA, BSRR, RTOS*. But at its core, embedded engineering is simply about **making a physical computer interact with the real world**: lighting up LEDs, listening to buttons, sensing room temperature, and spinning motors.

In this Beginner On-Ramp, we skip all heavy theory and give you **6 tiny, fully guided projects**. Every project gives you:
1. A simple physical analogy so your brain clicks immediately.
2. Complete, working copy-paste code that you can run right away.
3. A friendly line-by-line explanation of what every single word in the code does.
4. Two simulation options so you don't even need to buy physical hardware yet!

---

## 🛠️ Choose Your Simulation Playground (No Hardware Needed!)

You don't need to spend any money or wait for circuit boards to ship in the mail. Pick one of these two friendly paths:

```
                            CHOOSE YOUR ON-RAMP TRACK
                                        │
                    ┌───────────────────┴───────────────────┐
                    ▼                                       ▼
        ┌───────────────────────┐               ┌───────────────────────┐
        │  TRACK A: WOKWI SIM   │               │ TRACK B: PICSIMLAB    │
        ├───────────────────────┤               ├───────────────────────┤
        │ • 100% In-Browser     │               │ • Desktop Simulator   │
        │ • Zero installation   │               │ • Real silicon tool   │
        │ • Arduino Uno / C++   │               │ • Microchip PIC16F    │
        │ • Instant click & run │               │ • MPLAB X & XC8 C     │
        └───────────────────────┘               └───────────────────────┘
```

### 🌐 Track A: The Instant Web Playground (Wokwi) — *Zero Setup!*
- Runs right inside your web browser (Chrome, Firefox, Safari, Edge) on Windows, Mac, Linux, or even a Chromebook.
- Uses an Arduino Uno board with standard C/C++ syntax.
- **Link:** [https://wokwi.com](https://wokwi.com) (free, no account required to start).

### 🖥️ Track B: The PIC Silicon Simulator (PICSimLab)
- A realistic desktop electronic circuit simulator that mimics real Microchip PIC microcontrollers.
- Uses the official industry toolchain: Microchip MPLAB X IDE and the free XC8 C compiler.
- If you plan to follow this repository's [**PIC Microcontroller Track**](../pic-mplab-xc8/README.md), follow our quick [**PIC Installation Guide**](../pic-mplab-xc8/installation.md) first.

---

## 🗺️ The 6-Project On-Ramp Checklist

Work through these 6 short projects in order. Each one takes only 15 to 30 minutes:

| # | Project | The Physical Intuition | What You Will Build |
| :-: | :--- | :--- | :--- |
| **01** | [**Project 1: Blink an LED**](01-blink-led.md) | The "Hello World" of physical computing. | Make a light turn on, pause, turn off, and repeat. |
| **02** | [**Project 2: Read a Pushbutton**](02-read-button.md) | How switches work & the "floating pin" trap. | Turn on an LED only when a finger presses a button. |
| **03** | [**Project 3: Serial "Hello World"**](03-serial-hello.md) | Two walkie-talkies (TX and RX) talking over wires. | Print live text messages from the chip to your PC screen. |
| **04** | [**Project 4: The Volume Knob (Potentiometer)**](04-potentiometer-knob.md) | Taking a digital ruler to smooth voltage. | Read an analog dial and adjust LED blink speed or brightness. |
| **05** | [**Project 5: Interactive Terminal Controller**](05-button-led-serial.md) | Combining input, output, and data telemetry. | Count button presses and report stats live to the terminal. |
| **06** | [**Project 6: Traffic Light State Machine**](06-blinking-state-machine.md) | Ditching `delay()` so your chip never freezes! | Build a responsive Red $\to$ Yellow $\to$ Green traffic signal. |

---

## 💡 Quick Rules for Beginner Success

1. **Don't rush to memorize:** If you don't remember what a word means, check our [**Beginner's Glossary (`cheatsheets/glossary.md`)**](../cheatsheets/glossary.md).
2. **Break things on purpose:** Once your project works, change a number! Make the LED blink 10 times faster. What happens if you change `500` to `50`? Discovering through experimentation is how real engineers learn.
3. **Where to go after completing all 6:**  
   Once you finish Project 6, you will have the confidence and intuition to dive into the core engineering foundations! Proceed to [**`PREREQUISITES.md`**](../PREREQUISITES.md) and [**Step 1: C & Embedded C**](../curriculum/01-c-embedded-c/README.md).
