# 📖 How to Read Any Silicon Datasheet & Write Custom Peripheral Drivers

> **A Complete Practical Guide for the Microchip PIC16F877A & PICSimLab Simulation**  
> *"Engineers do not guess register bits or copy code from forums. They open the silicon datasheet, extract the electrical contract, calculate clock constants, and construct drivers line by line."*

---

## 🧭 Which Markdown Files in This Repository Will Help You?

To guide you from an absolute beginner to writing your own complete peripheral drivers, follow this exact document roadmap:

| Stage | Guide / File | What It Teaches You |
| :---: | :--- | :--- |
| **0** | [**`PREREQUISITES.md`**](../PREREQUISITES.md) | Ohm's law, pull-up resistors, memory-mapped I/O, `volatile` rules, and the 7-Step Code Construction Workflow. |
| **1** | [**`curriculum/01-c-embedded-c/syntax-and-pointers.md`**](../curriculum/01-c-embedded-c/syntax-and-pointers.md) | Double pointers (`**p`), function pointer callbacks, array pointers (`(*p)[10]`), and register dereferencing. |
| **2** | [**`pic-mplab-xc8/datasheet-driver-guide.md`**](datasheet-driver-guide.md) | **THIS GUIDE:** Universal technique to read any datasheet and write drivers for every PIC peripheral. |
| **3** | [**`pic-mplab-xc8/beginner.md`**](beginner.md) | Step-by-step construction for GPIO Output (LEDs), GPIO Input (Buttons), and Timer0. |
| **4** | [**`pic-mplab-xc8/intermediate.md`**](intermediate.md) | Step-by-step construction for ADC, PWM, UART, I2C, and Matrix Keypad. |
| **5** | [**`pic-mplab-xc8/advanced.md`**](advanced.md) | Step-by-step construction for Sleep/WDT, Interrupt demuxing, and EEPROM storage. |
| **6** | [**`cheatsheets/pic-xc8-cheatsheet.md`**](../cheatsheets/pic-xc8-cheatsheet.md) | Fast register lookup table for all PIC16F877A registers and bitfields. |
| **7** | [**`my-code/pic16f877a/`**](../my-code/pic16f877a/README.md) | **Your Personal Workspace:** Dedicated folder with starter templates to write and upload your own code! |

---

## 🔍 The Universal 7-Step Method to Read ANY Silicon Datasheet

Every microcontroller datasheet (Microchip, STMicroelectronics, NXP, TI, Espressif) follows a standardized structure. Follow these 7 steps to extract exactly what you need without getting overwhelmed by 500+ pages:

```
  ┌────────────────────────────────────────────────────────────────────────┐
  │ 1. Feature Summary & Block Diagram: Bus architecture & clock limits    │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 2. Pinout & Pin Multiplexing: Locate which physical pin has alternate fn│
  ├────────────────────────────────────────────────────────────────────────┤
  │ 3. Electrical Characteristics: Voltage rails, max source/sink currents │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 4. Special Function Register (SFR) Map: Register names, offsets, banks │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 5. Peripheral Functional Section: Numbered initialization checklists   │
  ├────────────────────────────────────────────────────────────────────────┤
  │ 6. Mathematical Formulas: Timer rollovers, PWM duty, Baud rate divisors│
  ├────────────────────────────────────────────────────────────────────────┤
  │ 7. Silicon Errata Sheet: Hardware silicon bugs & mandatory workarounds │
  └────────────────────────────────────────────────────────────────────────┘
```

### Step 1: Check the Feature Summary & Device Overview (Page 1–5)
- **What to look for:** Flash memory size ($8\text{K words}$ for PIC16F877A), SRAM size ($368\text{ bytes}$), EEPROM size ($256\text{ bytes}$), maximum oscillator frequency ($20\text{ MHz}$), and operating voltage ($2.0\text{V} - 5.5\text{V}$).
- **Core takeaway:** This tells you the hardware boundaries before writing a single line.

### Step 2: Pinout and Pin Multiplexing Tables (Section 1.0)
- In microcontrollers, pins are shared (**multiplexed**) across multiple internal peripherals.
- *Example from PIC16F877A Datasheet Table 1-1:*
  - **Pin 17:** `RC2/CCP1` $\rightarrow$ Can be GPIO Port C Pin 2 **OR** Capture/Compare/PWM Module 1.
  - **Pin 18:** `RC3/SCK/SCL` $\rightarrow$ Can be GPIO **OR** SPI Clock **OR** I2C Clock.
  - **Pin 25:** `RC6/TX/CK` $\rightarrow$ Can be GPIO **OR** USART Asynchronous Transmit.
  - **Pin 26:** `RC7/RX/DT` $\rightarrow$ Can be GPIO **OR** USART Asynchronous Receive.
- **Rule:** Before configuring a peripheral, identify the exact physical pins it controls.

### Step 3: Electrical Characteristics & Absolute Maximum Ratings (Section 17.0)
- **Never exceed Absolute Maximum Ratings**, or you will permanently damage the chip:
  - $V_{DD}$ to $V_{SS}$: $-0.3\text{V}$ to $+7.5\text{V}$.
  - Maximum output current sunk/sourced by any single I/O pin: **$25\text{ mA}$**.
  - Maximum output current sunk/sourced by all ports combined: **$200\text{ mA}$**.
  - Logic Input High threshold ($V_{IH}$): Minimum $2.0\text{V}$ for TTL, $0.8 \times V_{DD}$ for Schmitt Trigger.
  - Logic Input Low threshold ($V_{IL}$): Maximum $0.8\text{V}$ for TTL, $0.2 \times V_{DD}$ for Schmitt Trigger.

### Step 4: Memory Organization & Special Function Registers (Section 2.0)
- Understand the memory banking architecture:
  - In the 8-bit PIC16F877A, RAM is divided into **4 Banks (Bank 0 to Bank 3)**.
  - `PORTB` is in **Bank 0** (`0x06`).
  - `TRISB` (Direction register) is in **Bank 1** (`0x86`).
  - When writing assembly or low-level C, the compiler handles banking, but you must know which register controls what!

### Step 5: Read the Peripheral Chapter's Numbered Checklist
Manufacturers almost always provide a **step-by-step initialization sequence** in the datasheet!
*Example: Section 10.0 (USART), subsection 10.1.1 "Configuring Transmit":*
> 1. Initialize `SPBRG` register for the appropriate baud rate.
> 2. Enable the asynchronous serial port by clearing bit `SYNC` and setting bit `SPEN`.
> 3. If interrupts are desired, set enable bit `TXIE`.
> 4. If 9-bit transmission is desired, set bit `TX9`.
> 5. Enable transmission by setting bit `TXEN`.
> 6. Put data into `TXREG` to start transmission.

**This is your exact driver recipe!** You don't need to guess; just convert that numbered list directly into C code.

### Step 6: Extract Mathematical Formulas
Every peripheral documentation includes formulas to calculate timing values:
- **Timer0 Rollover:** $T_{overflow} = \frac{4}{F_{osc}} \times \text{Prescaler} \times (256 - \text{TMR0})$
- **PWM Frequency:** $F_{PWM} = \frac{F_{osc}}{4 \times (\text{PR2} + 1) \times \text{TMR2 Prescaler}}$
- **USART Baud Rate (High Speed BRGH=1):** $\text{Baud} = \frac{F_{osc}}{16 \times (\text{SPBRG} + 1)} \implies \text{SPBRG} = \frac{F_{osc}}{16 \times \text{Baud}} - 1$

### Step 7: Consult the Silicon Errata Sheet
Before spending hours debugging strange peripheral behavior, search for `"PIC16F877A Errata"`. Silicon manufacturers publish known silicon hardware errata and workarounds (e.g. read-modify-write timing restrictions, I2C bus collision flag edge cases).

---

## 🛠️ Complete Peripheral Blueprint for the PIC16F877A

Below is the complete inventory of peripherals available on the PIC16F877A (and simulated on the PICSimLab PICGenios / Breadboard):

```
                       PIC16F877A Silicon Peripherals
 ┌────────────────────────────────────────────────────────────────────────┐
 │ 1. GPIO Ports (PORTA, PORTB, PORTC, PORTD, PORTE)                      │
 │ 2. Mechanical Inputs & Weak Pull-ups (OPTION_REG.nRBPU)                │
 │ 3. Hardware Timers:                                                    │
 │    • Timer0: 8-bit with 1:2 to 1:256 prescaler                         │
 │    • Timer1: 16-bit with 1:1 to 1:8 prescaler & external 32kHz crystal │
 │    • Timer2: 8-bit with PR2 period match, postscaler, PWM timebase     │
 │ 4. 10-Bit Successive Approximation ADC (8 Analog Channels, AN0-AN7)    │
 │ 5. Capture / Compare / PWM (CCP1 on RC2, CCP2 on RC1)                  │
 │ 6. USART: Full-Duplex Serial Communication (TX on RC6, RX on RC7)      │
 │ 7. MSSP - Master Synchronous Serial Port:                              │
 │    • SPI Mode (Master / Slave: SCK, SDO, SDI, SS)                      │
 │    • I2C Mode (Master / Slave 7-bit/10-bit: SCL, SDA)                  │
 │ 8. 4-Bit HD44780 Alphanumeric Liquid Crystal Display (LCD on PORTD)    │
 │ 9. 4x4 Matrix Keypad Scanning                                          │
 │ 10. Internal Non-Volatile Data EEPROM (256 Bytes)                      │
 │ 11. Core System: Watchdog Timer (WDT), Power-Down (SLEEP), Reset/BOR   │
 └────────────────────────────────────────────────────────────────────────┘
```

---

## 📝 Step-by-Step Driver Construction Walkthroughs

### 1. Digital Output Driver (LEDs)
- **Datasheet Section:** Section 3.0 "I/O Ports"
- **Hardware Pins:** `RB0` - `RB7` (connected to 8 LEDs on PICGenios board).
- **Registers Needed:**
  - `TRISB`: Tri-state direction register (`0` = Output, `1` = Input).
  - `PORTB`: Port data latch (`1` = $5\text{V}$, `0` = $0\text{V}$).
- **How to construct the driver:**
  ```c
  void LED_Init(void) {
      TRISBbits.TRISB0 = 0; // Pin RB0 configured as digital output
      PORTBbits.RB0 = 0;    // Initial state: LOW (LED OFF)
  }

  void LED_Set(uint8_t state) {
      if (state) {
          PORTBbits.RB0 = 1; // Turn LED ON
      } else {
          PORTBbits.RB0 = 0; // Turn LED OFF
      }
  }

  void LED_Toggle(void) {
      PORTBbits.RB0 ^= 1;    // Toggle state
  }
  ```

---

### 2. Digital Input with Internal Weak Pull-Ups (Pushbuttons)
- **Datasheet Section:** Section 3.2 "PORTB and TRISB Register" & Section 2.2.2.2 "OPTION_REG"
- **Hardware Pins:** `RB0` (active-low pushbutton to GND).
- **Registers Needed:**
  - `TRISBbits.TRISB0 = 1`: Configure pin as input.
  - `OPTION_REGbits.nRBPU = 0`: Enable PORTB weak pull-up resistors globally!
  - `PORTBbits.RB0`: Read logic level ($1$ when released, $0$ when pressed).
- **How to construct the driver:**
  ```c
  void Button_Init(void) {
      TRISBbits.TRISB0 = 1;       // Configure RB0 as input
      OPTION_REGbits.nRBPU = 0;   // Enable PORTB internal pull-ups (Bit 7 active LOW)
  }

  uint8_t Button_IsPressed(void) {
      // Button is active-low: returns 1 if button is pressed (pin is 0)
      if (PORTBbits.RB0 == 0) {
          __delay_ms(20); // Debounce delay
          if (PORTBbits.RB0 == 0) {
              return 1;
          }
      }
      return 0;
  }
  ```

---

### 3. Hardware Timer0 Periodic Interrupt Driver
- **Datasheet Section:** Section 5.0 "Timer0 Module"
- **Clock Source:** Internal Instruction Cycle ($F_{osc} / 4 = 4\text{ MHz} / 4 = 1\text{ MHz} \implies T_{inst} = 1\,\mu\text{s}$).
- **Prescaler:** $1:256 \implies \text{Tick} = 256\,\mu\text{s}$. Overflow every $256 \times 256\,\mu\text{s} = 65.536\text{ ms}$.
- **Registers Needed:**
  - `OPTION_REG`: Set `T0CS = 0` (Timer mode), `PSA = 0` (Assign prescaler to Timer0), `PS<2:0> = 111` ($1:256$).
  - `INTCON`: Set `GIE = 1` (Global Interrupt Enable), `TMR0IE = 1` (Timer0 Overflow Interrupt Enable).
  - `INTCONbits.TMR0IF`: Interrupt flag (must be cleared in software inside ISR).
- **How to construct the driver:**
  ```c
  void Timer0_Init(void) {
      OPTION_REGbits.T0CS = 0;   // Internal instruction cycle clock
      OPTION_REGbits.PSA  = 0;   // Prescaler assigned to Timer0
      OPTION_REGbits.PS   = 0b111; // 1:256 prescaler rate
      TMR0 = 0;                  // Clear timer register
      INTCONbits.TMR0IF = 0;     // Clear overflow flag
      INTCONbits.TMR0IE = 1;     // Enable Timer0 interrupt
      INTCONbits.GIE    = 1;     // Enable Global interrupts
  }
  ```

---

### 4. 10-Bit Analog-to-Digital Converter (ADC) Driver
- **Datasheet Section:** Section 11.0 "Analog-to-Digital Converter (A/D) Module"
- **Hardware Pins:** `RA0/AN0` (connected to potentiometer on PICSimLab).
- **Registers Needed:**
  - `TRISAbits.TRISA0 = 1`: Pin must be configured as input.
  - `ADCON1`:
    - `ADFM = 1` (Right-justified result: 10-bit value in `ADRESH:ADRESL`).
    - `PCFG<3:0> = 1110` (Configure `AN0` as analog input, all other pins digital).
  - `ADCON0`:
    - `ADCS<1:0> = 01` (ADC conversion clock $F_{osc}/8$, satisfying $T_{AD} \ge 1.6\,\mu\text{s}$).
    - `CHS<2:0> = 000` (Select Channel 0 / `AN0`).
    - `ADON = 1` (Turn on ADC power).
    - `GO/nDONE = 1` (Start conversion; hardware clears to 0 when finished).
- **How to construct the driver:**
  ```c
  void ADC_Init(void) {
      TRISAbits.TRISA0 = 1;       // RA0 as input
      ADCON1bits.ADFM  = 1;       // Right-justified result
      ADCON1bits.PCFG  = 0b1110;  // AN0 analog, VDD/VSS references
      ADCON0bits.ADCS  = 0b01;    // Fosc / 8 conversion clock
      ADCON0bits.CHS   = 0b000;   // Select Channel 0 (AN0)
      ADCON0bits.ADON  = 1;       // Power on ADC module
  }

  uint16_t ADC_ReadChannel0(void) {
      __delay_us(25);             // Acquisition time (capacitor charging)
      ADCON0bits.GO_nDONE = 1;    // Start conversion
      while (ADCON0bits.GO_nDONE);// Wait until conversion completes (GO drops to 0)
      return (uint16_t)((ADRESH << 8) | ADRESL); // Return 10-bit raw ADC count
  }
  ```

---

### 5. Hardware PWM Driver (CCP1 Module)
- **Datasheet Section:** Section 8.3 "PWM Mode"
- **Hardware Pins:** `RC2/CCP1` (Pin 17).
- **Timebase:** Driven by **Timer2**.
- **Registers Needed:**
  - `TRISCbits.TRISC2 = 0`: Pin RC2 must be configured as output.
  - `PR2`: Period register ($PR2 = \frac{F_{osc}}{4 \times F_{pwm} \times \text{T2 Prescaler}} - 1$). For $1\text{ kHz}$ at $4\text{ MHz}$ with $1:4$ prescaler: $PR2 = 249$.
  - `CCP1CON`: Set `CCP1M<3:0> = 1100` (PWM mode).
  - `CCPR1L` and `CCP1CON<5:4>`: Store the 10-bit duty cycle value.
  - `T2CON`: Set prescaler $1:4$ and `TMR2ON = 1`.
- **How to construct the driver:**
  ```c
  void PWM1_Init(void) {
      TRISCbits.TRISC2 = 0;       // RC2 as PWM output
      PR2 = 249;                  // Period for 1 kHz PWM at 4 MHz
      CCP1CONbits.CCP1M = 0b1100; // CCP1 in PWM mode
      CCPR1L = 0;                 // 0% initial duty cycle
      CCP1CONbits.CCP1X = 0;
      CCP1CONbits.CCP1Y = 0;
      T2CONbits.T2CKPS  = 0b01;   // Timer2 prescaler 1:4
      T2CONbits.TMR2ON  = 1;      // Start Timer2
  }

  void PWM1_SetDuty(uint16_t duty_10bit) {
      if (duty_10bit > 1000) duty_10bit = 1000;
      CCPR1L = (uint8_t)(duty_10bit >> 2);
      CCP1CONbits.CCP1X = (duty_10bit >> 1) & 0x01;
      CCP1CONbits.CCP1Y = duty_10bit & 0x01;
  }
  ```

---

### 6. Full-Duplex USART Driver (UART Serial)
- **Datasheet Section:** Section 10.0 "Addressable Universal Synchronous Asynchronous Receiver Transmitter (USART)"
- **Hardware Pins:** `RC6/TX` (Pin 25), `RC7/RX` (Pin 26).
- **Baud Rate Calculation:** For $9600\text{ baud}$ at $4\text{ MHz}$ with high-speed mode (`BRGH=1`):
  $$SPBRG = \frac{4\,000\,000}{16 \times 9600} - 1 = 26.04 - 1 = 25 \implies \text{Actual: } 9615\text{ baud (0.16% error)}$$
- **Registers Needed:**
  - `TRISCbits.TRISC6 = 0` (TX output), `TRISCbits.TRISC7 = 1` (RX input).
  - `SPBRG = 25`.
  - `TXSTAbits.BRGH = 1` (High-speed baud rate generator).
  - `TXSTAbits.TXEN = 1` (Enable transmitter circuitry).
  - `RCSTAbits.SPEN = 1` (Enable serial port pins).
  - `RCSTAbits.CREN = 1` (Enable continuous receiver).
  - `PIR1bits.TXIF`: Transmit buffer empty flag (read-only).
  - `PIR1bits.RCIF`: Receive buffer full flag (read-only).
- **How to construct the driver:**
  ```c
  void UART_Init(void) {
      TRISCbits.TRISC6 = 0;       // RC6 (TX) as output
      TRISCbits.TRISC7 = 1;       // RC7 (RX) as input
      SPBRG = 25;                 // 9600 baud at 4 MHz
      TXSTAbits.BRGH = 1;         // High speed
      TXSTAbits.SYNC = 0;         // Asynchronous mode
      RCSTAbits.SPEN = 1;         // Enable serial port
      TXSTAbits.TXEN = 1;         // Enable transmitter
      RCSTAbits.CREN = 1;         // Enable continuous receive
  }

  void UART_WriteByte(uint8_t byte) {
      while (!PIR1bits.TXIF);     // Wait until transmit buffer is empty
      TXREG = byte;               // Write byte to transmit register
  }

  uint8_t UART_ReadByte(void) {
      if (RCSTAbits.OERR) {       // Overrun error recovery
          RCSTAbits.CREN = 0;
          RCSTAbits.CREN = 1;
      }
      while (!PIR1bits.RCIF);     // Wait until character is received
      return RCREG;               // Read byte from receive register
  }
  ```

---

### 7. Master I2C Driver (MSSP Module)
- **Datasheet Section:** Section 9.0 "Master Synchronous Serial Port (MSSP) Module"
- **Hardware Pins:** `RC3/SCL` (Pin 18), `RC4/SDA` (Pin 23).
- **Clock Calculation:** For $100\text{ kHz}$ Standard Mode I2C at $F_{osc} = 4\text{ MHz}$:
  $$SSPADD = \frac{F_{osc}}{4 \times F_{SCL}} - 1 = \frac{4\,000\,000}{4 \times 100\,000} - 1 = 10 - 1 = 9$$
- **Registers Needed:**
  - `TRISCbits.TRISC3 = 1` and `TRISCbits.TRISC4 = 1` (I2C pins must be set as inputs; MSSP module controls open-drain pull-down automatically).
  - `SSPCONbits.SSPM = 0b1000` (I2C Master Mode, clock = $F_{osc} / (4 \times (SSPADD + 1))$).
  - `SSPCONbits.SSPEN = 1` (Enable MSSP module).
  - `SSPCON2`: Control bits `SEN` (Start), `RSEN` (Repeated Start), `PEN` (Stop), `RCEN` (Receive Enable), `ACKDT` / `ACKEN` (Acknowledge sequence).
  - `SSPSTATbits.R_nW`: Indicates if a bus transmit is currently in progress.
- **How to construct the driver:**
  ```c
  void I2C_Master_Init(void) {
      TRISCbits.TRISC3 = 1;       // SCL input (open-drain)
      TRISCbits.TRISC4 = 1;       // SDA input (open-drain)
      SSPCON  = 0b00101000;       // SSPEN = 1, I2C Master Mode (SSPM = 1000)
      SSPCON2 = 0x00;
      SSPADD  = 9;                // 100 kHz clock at 4 MHz Fosc
      SSPSTAT = 0x80;             // Slew rate disabled for 100 kHz
  }

  static void I2C_Wait(void) {
      // Wait for Start, Stop, Read, or Transmit activity to complete
      while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_nW));
  }

  void I2C_Master_Start(void) {
      I2C_Wait();
      SSPCON2bits.SEN = 1;        // Initiate Start condition
  }

  void I2C_Master_Stop(void) {
      I2C_Wait();
      SSPCON2bits.PEN = 1;        // Initiate Stop condition
  }

  uint8_t I2C_Master_Write(uint8_t data) {
      I2C_Wait();
      SSPBUF = data;              // Load data into buffer to transmit
      I2C_Wait();
      return !SSPCON2bits.ACKSTAT;// Returns 1 if ACK received (ACKSTAT == 0)
  }
  ```

---

## 💻 How to Verify Your Drivers in PICSimLab Simulation

1. **Build in MPLAB X:** Press `Clean and Build Project` (Hammer & Brush icon). Verify output shows: `BUILD SUCCESSFUL`.
2. **Open PICSimLab:**
   - Go to `Board` $\rightarrow$ Select `Board 1: PICGenios` (or `Breadboard`).
   - Processor: Select `PIC16F877A`. Clock: Set to `4 MHz`.
3. **Load Hex File:**
   - Go to `File` $\rightarrow$ `Load Hex`.
   - Select your compiled `.hex` file from `<project>/dist/default/production/<project>.production.hex`.
4. **Interactive Hardware Testing:**
   - **LEDs:** Watch the 8 LEDs on PORTB toggle in real-time.
   - **ADC:** Adjust Potentiometer 1 (`AN0`) and observe the analog voltage change.
   - **UART Terminal:** Go to `Modules` $\rightarrow$ `Serial Terminal` or connect PuTTY to the virtual serial loopback COM port at 9600 baud.
   - **Oscilloscope:** Go to `Tools` $\rightarrow$ `Oscilloscope` and connect Channel 1 to `RC2/CCP1` to view your PWM duty cycle waveform!

---

## 🚀 Ready to Write Your Own Drivers?

Create your files in our dedicated workspace folder:
👉 [**`my-code/pic16f877a/`**](../my-code/pic16f877a/README.md)

Start with `gpio_driver_template.c`, open the PIC16F877A datasheet to Section 3.0, and write your first custom driver from scratch!
