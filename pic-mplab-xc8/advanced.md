# 🔴 Advanced Steps: PIC Systems & Hardware Interfacing

> Steps 9–13 cover power management, deep interrupt architecture, analog comparators, non-volatile EEPROM storage, physical ICSP hardware flashing, and capstone engineering implementations.

---

## Step 9: Low-Power Sleep Modes & Watchdog Timer (WDT)

### Goal
Implement ultra-low-power sleep modes on the PIC16F877A, configure the hardware Watchdog Timer for crash recovery, and use external signals to wake the CPU.

### Concept Explanation

#### Sleep Mode Architecture
When the PIC16F executes the `SLEEP` assembly instruction:
- The on-chip crystal oscillator stops running.
- The CPU core, instruction pipeline, and all peripherals driven by the system clock are frozen.
- Power consumption drops from ~15 mA (at 20 MHz / 5 V) down to less than **20 µA**.
- Register contents, I/O pin latch states, and SRAM values are completely preserved.

```
Normal Run Mode (15 mA) ──[ SLEEP instruction ]──► Sleep Mode (<20 µA)
                                                        │
                      ┌─────────────────────────────────┴─────────────────────────────────┐
                      ▼                                                                   ▼
       Wake-up by External Interrupt                                         Wake-up by WDT Timeout
         (RB0/INT or RB4-RB7 change)                                         (Resumes at next instruction
         (Vectors to 0x0004 if GIE=1)                                           if GIE=0, or resets)
```

#### Watchdog Timer (WDT) & Reset Logic
The Watchdog Timer runs independently from an internal, dedicated on-chip RC oscillator (~18 ms nominal timeout):
- **WDT Assignment**: The 8-bit prescaler in `OPTION_REG` can be assigned to Timer0 (`PSA = 0`) or to the WDT (`PSA = 1`).
- With prescaler 1:128 assigned to WDT (`PSA = 1`, `PS2:PS0 = 111`), the timeout extends to:
  $$\text{Timeout} = 18\text{ ms} \times 128 \approx 2.3\text{ seconds}$$
- If the firmware fails to clear the WDT with `CLRWDT()` before timeout:
  - During normal execution: Causes a microcontroller device reset.
  - During Sleep: Causes the device to wake up and resume execution.
- Status register flags: `STATUSbits.nTO` (Time-Out bit, active low) and `STATUSbits.nPD` (Power-Down bit, active low) allow the firmware to determine whether a reset was caused by power-on, brown-out, or WDT.

### Minimal XC8 Snippet
```c
#include <xc.h>

// Configuration Bits: Enable Watchdog Timer
#pragma config FOSC = HS, WDTE = ON, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void main(void) {
    // Configure RB0 as output (LED indicator), RB1 as wake-up button input
    TRISB0 = 0;
    PORTBbits.RB0 = 0;
    TRISB1 = 1;

    // Configure OPTION_REG: Assign prescaler to WDT (1:128 timeout ~2.3s)
    OPTION_REGbits.nRBPU = 0;   // Enable PORTB internal pull-ups
    OPTION_REGbits.PSA = 1;     // Prescaler assigned to WDT
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;

    // Blink LED to indicate wake-up / boot
    PORTBbits.RB0 = 1;
    __delay_ms(200);
    PORTBbits.RB0 = 0;

    while(1) {
        CLRWDT();               // Clear watchdog timer in active task
        
        // Enter ultra-low-power sleep
        SLEEP();
        NOP();                  // Always follow SLEEP with NOP per datasheet recommendation
        
        // Execution resumes here after WDT timeout or external pin change!
        PORTBbits.RB0 = 1;
        __delay_ms(100);
        PORTBbits.RB0 = 0;
    }
}
```

---

## Step 10: Advanced Interrupt Architecture & Context Saving

### Goal
Master the single-vector interrupt controller on the PIC16F877A, manage peripheral interrupt flags, and prevent race conditions.

### Concept Explanation

#### The PIC16F Interrupt Pipeline
Unlike ARM Cortex-M which features a nested NVIC with dozens of independent vectors, the PIC16F877A has **one single interrupt vector located at address `0x0004`**.

```
Hardware Event (Timer0 / UART / INT0)
                │
                ▼
   CPU finishes current instruction
                │
                ▼
   Pushes PC to 8-level hardware stack
                │
                ▼
   Hardware clears Global Interrupt Enable (INTCONbits.GIE = 0)
                │
                ▼
   Jumps unconditionally to address 0x0004 (XC8 __interrupt() ISR)
                │
                ▼
   Firmware queries IF flags to identify triggering peripheral
                │
                ▼
   Firmware handles event and MANUALLY CLEARS peripheral IF flag
                │
                ▼
   RETFIE instruction restores PC and automatically re-enables GIE
```

#### Automatic Context Saving in XC8
In mid-range PIC16 devices, hardware only automatically saves the program counter (`PC`) on the stack. The Working register (`W`), Status register (`STATUS`), and Program Counter Latch High (`PCLATH`) must be preserved.
- In XC8, the `void __interrupt() isr(void)` decorator automatically handles prologue and epilogue context saving in compiler-managed memory locations.
- **Rules of ISR Hygiene**:
  1. Never call complex math functions (`sprintf`, floating-point division) inside the ISR.
  2. Keep execution time under 50 instruction cycles.
  3. Set a `volatile` state flag and defer heavy processing to the `main()` superloop.
  4. Always manually clear the triggering interrupt flag (e.g. `PIR1bits.RCIF`, `INTCONbits.T0IF`, `INTCONbits.INTF`); failure to clear causes an infinite interrupt loop!

### XC8 Interrupt Demultiplexer Example
```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

volatile unsigned char rx_byte = 0;
volatile unsigned char rx_flag = 0;
volatile unsigned int  tick_count = 0;

void __interrupt() isr(void) {
    // 1. Check UART Receive Interrupt
    if (PIR1bits.RCIF && PIE1bits.RCIE) {
        if (RCSTAbits.OERR) {
            // Overrun error recovery sequence
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        } else {
            rx_byte = RCREG;    // Reading RCREG automatically clears RCIF
            rx_flag = 1;
        }
    }

    // 2. Check Timer0 Overflow Interrupt
    if (INTCONbits.T0IF && INTCONbits.T0IE) {
        INTCONbits.T0IF = 0;    // Manually clear flag!
        tick_count++;
        if (tick_count >= 1000) {
            tick_count = 0;
            PORTBbits.RB0 ^= 1; // Toggle heartbeat LED every second
        }
    }
}
```

---

## Step 11: Analog Comparators & Internal Data EEPROM

### Goal
Use the dual analog comparator module (`CMCON`) for hardware voltage-level detection and write/read persistent calibration values into the on-chip 256-byte Data EEPROM.

### Concept Explanation

#### Comparator Module (`CMCON`) & Voltage Reference (`VRCON`)
The PIC16F877A includes two analog comparators connected to pins RA0–RA3:
- Configured via the `CMCON` register (8 operational modes).
- Compares analog inputs against each other or against an internal 16-level programmable reference voltage ladder generated by `VRCON`.
- Outputs can be read via software (`CMCONbits.C1OUT`, `C2OUT`) or output directly to physical pins (RA4, RA5) for zero-latency hardware trip circuits (e.g., over-current protection).

#### Internal Data EEPROM Non-Volatile Storage
The 256 bytes of internal EEPROM retain data through power cycles:
- **EEDATA**: Holds the 8-bit data byte read or to be written.
- **EEADR**: Holds the address (`0x00` to `0xFF`).
- **EECON1**: Control register containing `EEPGD`, `WREN`, `WR`, `RD`.
- **EECON2**: Physical write-protection unlock register (not directly readable).
- **Mandatory Unlock Sequence**: To prevent accidental data corruption during power transients, Microchip enforces a strict sequential write of `0x55` followed by `0xAA` to `EECON2` while interrupts are disabled!

```
Firmware disables GIE ──► Writes 0x55 to EECON2 ──► Writes 0xAA to EECON2 ──► Sets EECON1.WR bit
                                                                                       │
Microchip Silicon Hardware verifies strict 2-cycle key match ─────────────────────────┘
                                     │
                        Hardware initiates EEPROM write (~4 ms)
```

### Complete EEPROM Read & Write Driver
```c
#include <xc.h>

unsigned char EEPROM_Read(unsigned char address) {
    EEADR = address;            // Load target address
    EECON1bits.EEPGD = 0;       // Select Data EEPROM memory (not Program Flash)
    EECON1bits.RD = 1;          // Initiate read cycle
    return EEDATA;              // Return data available on next instruction cycle
}

void EEPROM_Write(unsigned char address, unsigned char data) {
    EEADR = address;            // Set address
    EEDATA = data;              // Set data
    EECON1bits.EEPGD = 0;       // Access data memory
    EECON1bits.WREN = 1;        // Enable writes
    
    // Critical Section: Must disable interrupts during unlock sequence
    unsigned char gie_status = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    
    // Required Microchip Unlock Sequence
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;          // Start write
    
    INTCONbits.GIE = gie_status;// Restore interrupt state
    
    // Wait for write cycle completion (typically 4-8 ms)
    while (EECON1bits.WR);
    EECON1bits.WREN = 0;        // Disable write enable for safety
}
```

---

## Step 12: Real Hardware Programming via ICSP & PICkit

### Goal
Transition from PICSimLab simulation to physical breadboard hardware using In-Circuit Serial Programming (ICSP) and a PICkit 3 or PICkit 4 programmer.

### Concept Explanation

#### The 5-Wire ICSP Interface
Microchip microcontrollers are flashed using an in-circuit protocol that only requires 5 physical signals:

| Pin # | Signal | PIC16F877A DIP Pin | Description |
|:-----:|:------:|:------------------:|:------------|
| **1** | **VPP / MCLR** | Pin 1 | High-Voltage Programming pulse (~13 V) / Reset |
| **2** | **VDD** | Pin 11, 32 | Target +5.0 V power rail |
| **3** | **VSS** | Pin 12, 31 | Ground reference |
| **4** | **PGD (ICSPDAT)** | Pin 40 (RB7) | Bi-directional serial data line |
| **5** | **PGC (ICSPCLK)** | Pin 39 (RB6) | Synchronous programming clock driven by programmer |
| **6** | *(PGM / LVP)* | Pin 36 (RB3) | Pull low to GND for High-Voltage ICSP; not used if LVP=OFF |

```
  PICkit 3/4 Header             PIC16F877A 40-Pin DIP
 ┌─────────────────┐             ┌─────────────────────┐
 │ 1. VPP / MCLR   │────────────►│ Pin 1  (MCLR)       │ (Pull-up 10k to VDD, NO capacitor!)
 │ 2. VDD (+5V)    │────────────►│ Pin 11, 32 (VDD)    │
 │ 3. VSS (GND)    │────────────►│ Pin 12, 31 (VSS)    │
 │ 4. PGD (ICSPDAT)│◄───────────►│ Pin 40 (RB7/PGD)    │ (Keep traces < 10 cm, no pull-ups)
 │ 5. PGC (ICSPCLK)│────────────►│ Pin 39 (RB6/PGC)    │ (No capacitors or loading)
 └─────────────────┘             └─────────────────────┘
```

#### Critical Hardware Bringup Checklist
> [!CAUTION]
> **Common Hardware Bringup Traps**:
> 1. **MCLR Capacitor**: Never place a decoupling capacitor (>10 nF) directly from MCLR to Ground when using ICSP! The PICkit must drive the MCLR voltage from 0 V to 13 V with a nanosecond rise time to enter programming mode. A capacitor slows the edge and causes "Device ID 0x000000 does not match target" errors.
> 2. **Dual VDD/VSS Pins**: PIC16F877A has two VDD pins (11 and 32) and two VSS pins (12 and 31). **Both pairs must be connected to power and ground**, each with a 100 nF ceramic decoupling capacitor placed as physically close to the DIP pins as possible.
> 3. **Crystal Oscillator Loading**: When using a 4 MHz or 20 MHz crystal on OSC1 (pin 13) and OSC2 (pin 14), connect two 22 pF ceramic disc capacitors from each pin to GND.

---

## Step 13: Capstone Projects

### Capstone 1: Closed-Loop PID Temperature Controller
- **System Architecture**:
  1. **Sensor Front-End**: 10k NTC thermistor in voltage divider on RA0/AN0. ADC samples at 10 Hz with 16-sample moving average filter.
  2. **Control Algorithm**: Discrete PID controller calculates heater control effort:
     $$u[k] = K_p \cdot e[k] + K_i \sum e[k] + K_d (e[k] - e[k-1])$$
  3. **Actuator**: CCP1 hardware PWM drives a MOSFET heater element at 1 kHz. Duty cycle modulated from 0% to 100%.
  4. **User Interface**: 16x2 character LCD shows Current Temp, Setpoint, and Output Power. Two pushbuttons (RB1, RB2) adjust setpoint with EEPROM retention.

### Capstone 2: Microcontroller-to-PC UART Telemetry Bridge
- **System Architecture**:
  1. Microcontroller polls 4 analog channels (Potentiometers / temperature / light) and 4 digital switch inputs.
  2. Encapsulates measurements into a framed binary or ASCII packet:
     `$TELEM,<A0>,<A1>,<A2>,<A3>,<SWITCH_MASK>*<CHECKSUM>\r\n`
  3. Transmits telemetry packet over UART at 19200 baud to a host PC running Python with `pyserial` and `matplotlib`.
  4. Accepts bidirectional commands: Host PC can transmit commands (`$CMD,SET_PWM,75*`) to control local PIC outputs.

---

## 🏁 Step 9–13 Verification Checklist

- [ ] Low-power sleep entered and verified; current consumption drops to microamps.
- [ ] Watchdog Timer properly configured with prescaler and tested for reset recovery.
- [ ] Interrupt Service Routine correctly identifies multiple interrupt sources without race conditions.
- [ ] Context registers properly saved and restored; interrupt flags cleared manually.
- [ ] Data EEPROM read/write routines verified with hardware unlock sequence.
- [ ] Target programmed on real physical breadboard with PICkit and verified with an oscilloscope or DMM.
- [ ] Capstone project completed with documentation, schematic, and register-level test logs.
