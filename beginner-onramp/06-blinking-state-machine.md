# 🚦 Project 6: Traffic Light State Machine (Ditching `delay()` Forever!)

> **Goal:** Build an intelligent 3-color traffic light controller (RED $\to$ GREEN $\to$ YELLOW) that responds instantly to a pedestrian crosswalk button at any microsecond without ever freezing or using blocking delays.  
> **Prerequisites:** Projects 1 through 5.

---

### 🧠 The Real-World Intuition: The "Frozen Chef" Problem

In Projects 1 through 5, we used `delay(500)` or `__delay_ms(500)`.  
While great for quick tests, **`delay()` is the single most dangerous function in embedded engineering!**

#### The Analogy:
Imagine you hire a restaurant chef to cook steak and bake bread:
- A bad chef puts bread in the oven, sits down on a stool, and **stares motionless at a stopwatch for 30 minutes**, refusing to move or blink. Meanwhile, the steak catches fire on the stove!
- That is literally what `delay(1000)` forces your microcontroller to do: the CPU sits in an empty spin loop burning 16 million clock cycles doing zero work. If a button is pressed or an airbag sensor trips during that delay, **the microcontroller is totally deaf and blind!**

```
Blocking delay():      [ Light RED ] ───► (CPU FROZEN FOR 5 SECONDS) ───► [ Light GREEN ]
                                          ▲
                                          │ If pedestrian presses crosswalk button HERE,
                                          │ the chip completely misses it!
```

#### The Professional Way: The Kitchen Wall Clock (`millis()`)
A great chef checks the kitchen wall clock:
- *"What time is it now? 12:00. The bread needs to come out at 12:30. In the meantime, I will chop onions, season the meat, and answer the phone!"*

```
Non-Blocking FSM:      Every millisecond, check:
                       "Has enough time passed to switch lights?"
                       YES ──► Switch light color and note current time.
                       NO  ──► Read pedestrian crosswalk button immediately!
```

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

### Step 1: Open Wokwi
Open [https://wokwi.com](https://wokwi.com) $\to$ Select **Arduino Uno**.

### Step 2: Add Components
- Add 3 LEDs:
  - **Red LED:** Anode to **Pin 12**, Cathode to **GND** (via resistor).
  - **Yellow LED:** Anode to **Pin 11**, Cathode to **GND** (via resistor).
  - **Green LED:** Anode to **Pin 10**, Cathode to **GND** (via resistor).
- Add 1 **Pushbutton** (Pedestrian Crosswalk Call):
  - Left terminal to **Pin 2**, Right terminal to **GND**.

### Step 3: Paste the Code into `sketch.ino`

```cpp
// Project 6: Non-Blocking Traffic Light State Machine
const int RED_LED_PIN    = 12;
const int YELLOW_LED_PIN = 11;
const int GREEN_LED_PIN  = 10;
const int BUTTON_PIN     = 2;

// Define human-readable names for our traffic light states
enum TrafficState {
  STATE_RED,
  STATE_GREEN,
  STATE_YELLOW
};

TrafficState currentState = STATE_RED;
unsigned long stateStartTime = 0; // Remembers when the current state started
const unsigned long RED_DURATION    = 4000; // 4 seconds in Red
const unsigned long GREEN_DURATION  = 4000; // 4 seconds in Green
const unsigned long YELLOW_DURATION = 1500; // 1.5 seconds in Yellow

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("🚦 Non-Blocking Traffic Light Controller Started!");
  setLights(true, false, false); // Start with RED on
  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis(); // What time is it right now?

  // 1. INSTANT BUTTON RESPONSE: The chip never freezes, so it hears clicks instantly!
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (currentState == STATE_GREEN) {
      Serial.println("🚶 [PEDESTRIAN] Button pressed! Fast-forwarding Green to Yellow!");
      // Shorten green duration to switch quickly for the pedestrian
      currentState = STATE_YELLOW;
      setLights(false, true, false);
      stateStartTime = currentTime;
      delay(200); // Quick debounce
    }
  }

  // 2. FINITE STATE MACHINE (FSM) TRANSITIONS
  switch (currentState) {
    case STATE_RED:
      if (currentTime - stateStartTime >= RED_DURATION) {
        currentState = STATE_GREEN;
        setLights(false, false, true); // Green ON
        stateStartTime = currentTime;
        Serial.println("🚦 State Change -> GREEN");
      }
      break;

    case STATE_GREEN:
      if (currentTime - stateStartTime >= GREEN_DURATION) {
        currentState = STATE_YELLOW;
        setLights(false, true, false); // Yellow ON
        stateStartTime = currentTime;
        Serial.println("🚦 State Change -> YELLOW");
      }
      break;

    case STATE_YELLOW:
      if (currentTime - stateStartTime >= YELLOW_DURATION) {
        currentState = STATE_RED;
        setLights(true, false, false); // Red ON
        stateStartTime = currentTime;
        Serial.println("🚦 State Change -> RED");
      }
      break;
  }
}

// Helper function to set all 3 LEDs in one clean call
void setLights(bool red, bool yellow, bool green) {
  digitalWrite(RED_LED_PIN,    red    ? HIGH : LOW);
  digitalWrite(YELLOW_LED_PIN, yellow ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN,  green  ? HIGH : LOW);
}
```

### Step 4: Run and Test
1. Click **Play (▶)**.
2. Watch the lights cycle smoothly: Red (4s) $\to$ Green (4s) $\to$ Yellow (1.5s).
3. While the light is Green, click the pedestrian button. Notice how the traffic light **immediately** switches to Yellow without lagging!

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

On the PIC16F877A, we use Port D pins `RD0` (Red), `RD1` (Yellow), and `RD2` (Green), with pushbutton `RB0`.

### Paste the Code into `main.c`:

```c
// PIC16F877A - Project 6: Non-Blocking Traffic Light State Machine
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF

typedef enum {
    STATE_RED,
    STATE_GREEN,
    STATE_YELLOW
} TrafficState_t;

void set_lights(unsigned char red, unsigned char yellow, unsigned char green) {
    PORTDbits.RD0 = red;
    PORTDbits.RD1 = yellow;
    PORTDbits.RD2 = green;
}

void main(void) {
    TRISDbits.TRISD0 = 0; // Red LED Output
    TRISDbits.TRISD1 = 0; // Yellow LED Output
    TRISDbits.TRISD2 = 0; // Green LED Output
    TRISBbits.TRISB0 = 1; // Pedestrian Button Input
    OPTION_REGbits.nRBPU = 0;

    TrafficState_t state = STATE_RED;
    unsigned int timer_tick = 0;

    set_lights(1, 0, 0); // Start Red

    while (1) {
        // Heartbeat tick: 10ms loop slice
        __delay_ms(10);
        timer_tick += 10;

        // Instant pedestrian button check
        if (PORTBbits.RB0 == 0 && state == STATE_GREEN) {
            state = STATE_YELLOW;
            timer_tick = 0;
            set_lights(0, 1, 0);
        }

        switch (state) {
            case STATE_RED:
                if (timer_tick >= 4000) { // 4 seconds
                    state = STATE_GREEN;
                    timer_tick = 0;
                    set_lights(0, 0, 1);
                }
                break;

            case STATE_GREEN:
                if (timer_tick >= 4000) { // 4 seconds
                    state = STATE_YELLOW;
                    timer_tick = 0;
                    set_lights(0, 1, 0);
                }
                break;

            case STATE_YELLOW:
                if (timer_tick >= 1500) { // 1.5 seconds
                    state = STATE_RED;
                    timer_tick = 0;
                    set_lights(1, 0, 0);
                }
                break;
        }
    }
}
```

---

## 🎓 Congratulations! You Have Completed the Beginner On-Ramp!

Take a moment to realize what you have accomplished across these 6 projects:
1. ✅ **Outputs & Electricity:** Understood voltage, current, resistors, and driving LEDs.
2. ✅ **Inputs & Pull-Ups:** Learned how mechanical switches work and how to tame floating antenna pins.
3. ✅ **Telemetry:** Sent serial communication from silicon to your computer screen using UART.
4. ✅ **Analog Sensing:** Converted continuous physical voltages into discrete digital numbers with an ADC.
5. ✅ **System Integration:** Built an interactive device combining inputs, outputs, edge-detection, and remote commands.
6. ✅ **Architecture:** Replaced primitive `delay()` loops with professional, non-blocking **Finite State Machines (FSM)**.

### 🗺️ Where to Go From Here:
You now possess the foundational intuition and confidence of a real firmware developer. You are ready to dive into the core engineering curriculum:

1. 📘 Study [**`PREREQUISITES.md`**](../PREREQUISITES.md) to understand computer memory, CPU architecture, and defensive C programming.
2. 🦾 Follow [**Step 1: C & Embedded C**](../curriculum/01-c-embedded-c/README.md) to master pointers, structs, and bitwise manipulation.
3. 🔌 Or continue writing register-level drivers in our [**PIC Microcontroller Track**](../pic-mplab-xc8/README.md)!
