# 🟢 Beginner Steps: PIC Foundations

> First three steps establish the MPLAB X workflow, basic I/O, and timing fundamentals using PIC16F877A in PICSimLab.

## Step 1: MPLAB X Project & LED Blink

### Goal
Create your first MPLAB X project, configure the PIC16F877A, and blink an LED connected to RB0.

### Prerequisites
- MPLAB X IDE v5.35+ installed
- XC8 Compiler v2.36+ with FREE license
- PICSimLab v0.7+ installed
- Basic understanding of binary/hexadecimal

### Concept Explanation

#### Microcontroller Selection
We use **PIC16F877A** as our learning platform because:
- Well-documented with clear datasheet
- Rich peripheral set (ADC, UART, SPI, I2C, CCP, comparators)
- Available in DIP40 package for breadboarding
- Inexpensive (~$2) and widely supported

#### Configuration Bits (`#pragma config`)
Before `main()`, we must configure:
- **Oscillator**: Internal RC oscillator at 4MHz (`INTOSCIO`)
- **Watchdog Timer**: Disabled during learning (`WDT_OFF`)
- **Power-up Timer**: Enabled (`PWRT_ON`)
- **Brown-out Reset**: Enabled at 2.7V (`BOREN_ON`)
- **Low-voltage Programming**: Disabled (`LVP_OFF`)
- **Code Protection**: Disabled (`CP_OFF`)

#### Delay Functions
- `__delay_ms()` and `__delay_us()` use instruction cycle counting
- At 4MHz with 1:4 prescaler (default), 1 instruction = 1µs
- **Accuracy**: ±1% (depends on oscillator tolerance)

### Minimal XC8 Snippet
```c
#include <xc.h>

// Configuration bits for PIC16F877A
#pragma config FOSC = INTOSCIO  // Internal RC oscillator, port function on RA6/RA7
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low-voltage programming disabled
#pragma config CPD = OFF        // Data EE code protection off
#pragma config CP = OFF         // Flash program memory code protection off

#define _XTAL_FREQ 4000000     // Required for __delay_ms()

void main(void) {
    // Configure RB0 as output
    TRISB0 = 0;                // Clear TRIS bit for output
    
    while(1) {
        RB0 = 1;               // Set LED on
        __delay_ms(500);       // Wait 500ms
        RB0 = 0;               // Set LED off
        __delay_ms(500);       // Wait 500ms
    }
}
```

### PICSimLab Procedure
1. **Create Project** in MPLAB X:
   - File → New Project → Microchip Embedded → Standalone Project
   - Device: PIC16F877A
   - Tool: PICSimLab (select simulator)
   - Compiler: XC8
   - Name: `Blink_LED`

2. **Add Source File**:
   - Right-click Source Files → New → main.c
   - Paste the code above

3. **Build Project**:
   - Click Hammer icon or press F11
   - Verify "BUILD SUCCESSFUL" in output

4. **Load into PICSimLab**:
   - Click PICSimLab icon in toolbar (or Run → Debug Project)
   - PICSimLab should auto-launch with PIC16F877A Demo Board
   - Load the generated `.hex` file when prompted

5. **Observe Results**:
   - LED on RB0 should blink at 1Hz (500ms on, 500ms off)
   - Use PICSimLab's oscilloscope to verify timing
   - LED corresponds to physical pin 33 on DIP40 package

### Expected Result
- LED on RB0 (pin 33) blinks continuously at 1Hz
- MPLAB X output shows successful build: `Memory usage: ...`
- PICSimLab status bar shows running simulation
- Power consumption ~1.8mA during operation (simulated)

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| LED doesn't blink | Forgot to set `TRISB0 = 0` | RB0 defaults to input; TRIS must be cleared |
| Build fails on `__delay_ms` | Missing `_XTAL_FREQ` definition | Define `_XTAL_FREQ` before `<xc.h>` |
| Wrong frequency | Oscillator not configured | Verify `#pragma config FOSC` matches `_XTAL_FREQ` |
| SIMULATION_PAUSED | PICSimLab not auto-running | Click play button in PICSimLab toolbar |
| Hex file not found | Project not built | Build project before debugging |

### 🔗 Next Step
[Step 2: GPIO & Button Control](#step-2-gpio-button-control) → [Step 3: Timers & Interrupts](#step-3-timers-interrupts)

> **Register Focus**: Understand TRISB (direction), PORTB (read), LATB (write/latch) before proceeding.

---

## Step 2: GPIO & Button Control

### Goal
Control an LED with a button input and implement firmware debouncing.

### Prerequisites
- Completion of Step 1
- Understanding of TRIS, PORT, and LAT registers
- Basic button wiring knowledge

### Concept Explanation

#### TRIS vs PORT vs LAT
- **TRIS** (TRIState): Direction register. `0` = output, `1` = input
- **PORT**: Read/write to actual pin state. Reading PORT reads pin voltage.
- **LAT**: Latch register. Write-only. Safer for output manipulation (avoids read-modify-write issues)

#### Button Debouncing
Mechanical buttons bounce for ~5-20ms when pressed/released. Without debouncing, multiple state changes are detected.

### Minimal XC8 Snippet
```c
#include <xc.h>

#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

// Debounce timing
#define DEBOUNCE_COUNT 50  // ~5ms at 4MHz (50 * 100us loops)

void main(void) {
    // Configure RB0 as output (LED)
    TRISB0 = 0;
    LATB0 = 0;  // Start with LED off
    
    // Configure RB1 as input with pull-up (Button)
    TRISB1 = 1;  // Input
    // Note: PIC16F877A has internal weak pull-ups via Option Register
    // For simplicity, we'll use external pull-up
    
    unsigned char button_state = 1;  // Current stable state
    unsigned char last_state = 1;      // Previous stable state
    unsigned char debounce_counter = 0;
    
    while(1) {
        // Read raw button (active-low, so 0 = pressed)
        unsigned char raw_button = PORTBbits.RB1;
        
        // Simple debounce algorithm
        if(raw_button != last_state) {
            debounce_counter = 0;  // Reset timer on state change
            last_state = raw_button;
        } else if(debounce_counter < DEBOUNCE_COUNT) {
            debounce_counter++;
        } else {
            // State stable for DEBOUNCE_COUNT iterations
            if(button_state != last_state) {
                button_state = last_state;
                
                // Toggle LED on button press (active-low button)
                if(button_state == 0) {
                    LATB0 = ~LATB0;
                }
            }
        }
        
        __delay_ms(1);
    }
}
```

### PICSimLab Procedure
1. Wire button between RB1 and GND in PICSimLab
2. Internal pull-up can be enabled via `OPTION_REG` registers
3. Observe LED toggling each time button is pressed
4. Verify no fals triggers from button bounce

### Expected Result
- LED on RB0 toggles on each button press
- No fals triggers from button bounce
- LED remains stable until next press

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| Button always reads 0 | Floating input | Add external pull-up resistor |
| Multiple toggles per press | No debouncing | Implement software debounce counter |
| LED doesn't change | Wrong pin polarity | Check active-low vs active-high |
| Always pressed | Pull-up not enabled | Set WPUB1 = 1 in WPED bits |

### 🔗 Next Step
[Step 3: Timers & Interrupts](#step-3-timers-interrupts)

> **Register Focus**: OPTION_REGbits.nPUE for pull-ups, PORTBbits.RB1 for reading, LATB for writing.

---

## Step 3: Timers & Interrupts

### Goal
Replace software delays with Timer0 interrupt for non-blocking timing and precise control.

### Prerequisites
- Completion of Steps 1 and 2
- Understanding of Timer0 operation
- Basic knowledge of interrupt handling

### Concept Explanation

#### Timer0 Architecture (PIC16F877A)
- 8-bit timer/counter (0-255)
- Prescaler (1:1 to 1:128)
- Can increment from internal clock (Fosc/4) or external T0CKI pin
- Overflow generates TMR0IF flag

#### Interrupt Flow
1. Global interrupts enabled (GIE = 1)
2. Peripheral interrupts enabled (PEIE = 1)
3. Specific peripheral interrupt enabled (TMR0IE = 1)
4. Event occurs (timer overflow)
5. Flag set (TMR0IF = 1)
6. CPU jumps to interrupt vector (0x0004)
7. ISR executes, clears flag
8. Return from interrupt (RETFIE)

### Minimal XC8 Snippet
```c
#include <xc.h>

#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

volatile unsigned char blink_state = 0;

void __interrupt() ISR(void) {
    if(TMR0IF) {
        // Timer0 overflow
        static unsigned int counter = 0;
        counter++;
        
        // Toggle LED every 100 overflows (approx 250ms at 4MHz)
        if(counter >= 100) {
            counter = 0;
            blink_state = ~blink_state;
            LATB0 = blink_state;
        }
        
        // Clear flag
        TMR0IF = 0;
        
        // Reload timer for exactly 256 counts
        TMR0 = 0xFB;  // Adjust for precise timing
    }
}

void main(void) {
    // Configure RB0 as output
    TRISB0 = 0;
    LATB0 = 0;
    
    // Configure Timer0
    TMR0 = 0;                    // Clear timer
    T0CON = 0b10000011;          // TMR0 on, 1:32 prescaler
    INTCON = 0b11000000;         // GIE=1, PEIE=1, TMR0IE=1
    
    while(1) {
        // Main loop can do other work here
        // LED is toggled in ISR
        __delay_ms(100);  // Non-critical delay to show main loop is active
    }
}
```

### PICSimLab Procedure
1. Load project into PICSimLab
2. Add LED to RB0 and button to RB1 as before
3. Observe LED blinking at regular intervals
4. Main loop remains responsive (can add other tasks)

### Expected Result
- LED blinks at ~1Hz using Timer0 interrupt
- `main()` loop remains non-blocking
- Precise timing unaffected by CPU load

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| ISR never fires | GIE/PEIE not set | Check INTCON = 0b11000000 |
| Flag not cleared | Forgetting TMR0IF = 0 | Always clear interrupt flag |
| Timing wrong | Wrong prescaler/reload | Calculate: (256 - reload) * 32 * 4MHz⁻¹ |
| Nested interrupts | GIE cleared during ISR | Use GET_AND_CLEAR_IPEN() macro |

### 🔗 Next Steps
- Continue to [Intermediate Steps](./intermediate.md) for ADC, PWM, UART
- Explore PIC16F877A datasheet for other timers (TMR1, TMR2)

> **Register Focus**: TMR0, T0CON, INTCON, OPTION_REG. These combine to control all aspects of timer behavior.
