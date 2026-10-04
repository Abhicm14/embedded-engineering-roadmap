# 🔌 PIC16F877A XC8 Code Construction Guide

> **Core Philosophy:** *Never copy-paste embedded code. Understand the electrical signal, register bit, timing diagram, and construction sequence behind every line before typing it into your editor.*

---

## 🧭 Pre-Coding Foundations

Before writing any XC8 C firmware for the PIC16F877A, you must review:
1. [**Foundational Prerequisites (`PREREQUISITES.md`)**](../../PREREQUISITES.md): Number systems, bitwise masks, electrical pull-ups, and the `volatile` qualifier.
2. [**PIC Track Installation Guide**](../../pic-mplab-xc8/installation.md): MPLAB X IDE, XC8 compiler, and PICSimLab simulator setup.
3. [**PIC16F87XA Silicon Datasheet (DS39582C)**](../../resources/datasheets-and-reference.md#4-microchip-pic-silicon--compiler-references): Section 3 (I/O Ports), Section 4 (Timer0), Section 8 (CCP), Section 9 (MSSP), Section 10 (USART), Section 11 (ADC).
4. [**PIC XC8 Cheatsheet**](../../cheatsheets/pic-xc8-cheatsheet.md): Register syntax, configuration pragmas, and timing formulas.

---

## 📐 The 7-Step Firmware Construction Workflow

Every example in this directory was constructed following this disciplined engineering process:

```
  ┌─────────────────────────────────────────────────────────────────────────┐
  │ Step 1: Establish the Electrical & Hardware Contract                    │
  │         Identify physical pins, voltage rails, and current constraints. │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 2: Set Microcontroller Configuration Pragmas                       │
  │         Select oscillator mode, disable WDT/LVP, enable PWRT and BOREN. │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 3: Define Clock Frequency Macro for Compiler Delays                │
  │         Specify #define _XTAL_FREQ for accurate __delay_ms() cycles.    │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 4: Configure Data Direction Registers (TRISx)                     │
  │         Set 0 for output, 1 for input (Remember: 0 = Out, 1 = In).      │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 5: Configure Peripheral Control & Status Registers                 │
  │         Derive clock prescalers, baud rate divisors, and channel masks. │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 6: Write Driver Functions & Interrupt Service Routines (ISRs)      │
  │         Handle hardware flags, clear flags in software, avoid delays.   │
  ├─────────────────────────────────────────────────────────────────────────┤
  │ Step 7: Verify via Simulation (PICSimLab) & Lab Instruments             │
  │         Check waveforms on virtual oscilloscope or physical logic probe.│
  └─────────────────────────────────────────────────────────────────────────┘
```

---

## 🛠️ Step-by-Step Construction Guides for All 8 Examples

---

### 1. LED Blinker (`blink.c`)
- **Runnable Source:** [`blink.c`](blink.c)
- **Prerequisites to Learn First:**
  - Microchip `#pragma config` bit definitions for high-speed crystal (`FOSC = HS`).
  - The electrical rule: LED anode must have a $330\,\Omega$ resistor to prevent excessive current draw from the MCU pin ($I_{max} = 25\text{ mA}$).
  - Difference between `TRISB` (direction) and `PORTB` (voltage state).
- **How to Construct the Code Step-by-Step:**
  1. **Configure Hardware Fuses:** Write `#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF`.
  2. **Define Master Clock:** Write `#define _XTAL_FREQ 4000000` (4 MHz crystal).
  3. **Configure Pin Direction:** Inside `main()`, set `TRISBbits.TRISB0 = 0` to configure pin RB0 as a digital output.
  4. **Set Initial State:** Write `PORTBbits.RB0 = 0` to ensure the LED starts in the OFF state.
  5. **Construct Non-terminating Superloop:** Inside `while(1)`, write `PORTBbits.RB0 = 1`, call `__delay_ms(500)`, set `PORTBbits.RB0 = 0`, and call `__delay_ms(500)`.

---

### 2. Pushbutton with Debounce (`button_led.c`)
- **Runnable Source:** [`button_led.c`](button_led.c)
- **Prerequisites to Learn First:**
  - Mechanical switch contact bounce physics (5 ms to 20 ms of high-frequency electrical oscillation upon contact closure).
  - Floating inputs vs internal weak pull-up resistors on `PORTB` controlled by `OPTION_REGbits.nRBPU`.
- **How to Construct the Code Step-by-Step:**
  1. **Configure Direction:** Set `TRISBbits.TRISB0 = 0` (LED output) and `TRISBbits.TRISB1 = 1` (Button input).
  2. **Enable Internal Pull-ups:** Set `OPTION_REGbits.nRBPU = 0` (Active-low bit in `OPTION_REG` enables internal weak pull-ups on all PORTB input pins).
  3. **Design State Variables:** Declare `debounced_state = 1` (unpressed), `last_sample = 1`, and `stable_count = 0`.
  4. **Build Non-blocking Debounce Sampler:** In the superloop, sample `PORTBbits.RB1` every 1 ms.
  5. **Filter Noise:** If the current sample matches `last_sample`, increment `stable_count`. When `stable_count` reaches 20 ms, confirm the new state and update `PORTBbits.RB0`. If a transition occurs, reset `stable_count = 0`.

---

### 3. Hardware Timer0 Interrupt Blinker (`timer_blink.c`)
- **Runnable Source:** [`timer_blink.c`](timer_blink.c)
- **Prerequisites to Learn First:**
  - Why software delays (`__delay_ms`) waste 100% of CPU cycles and prevent multitasking.
  - Instruction cycle clock: $F_{CY} = \frac{F_{OSC}}{4} = 1\text{ MHz}$ at 4 MHz ($T_{CY} = 1\,\mu\text{s}$).
  - 8-bit timer rollover arithmetic ($0\text{xFF} \to 0\text{x00}$).
- **How to Construct the Code Step-by-Step:**
  1. **Configure Timer0 in `OPTION_REG`:**
     - Set `T0CS = 0` (select internal instruction cycle clock $F_{OSC}/4$).
     - Set `PSA = 0` (assign prescaler to Timer0, not WDT).
     - Set `PS2:PS0 = 111` (prescaler ratio 1:256).
     - Each timer tick = $1\,\mu\text{s} \times 256 = 256\,\mu\text{s}$. Overflow time = $256 \times 256\,\mu\text{s} = 65.536\text{ ms}$.
  2. **Enable Interrupts in `INTCON`:** Set `INTCONbits.T0IF = 0`, `INTCONbits.T0IE = 1`, and `INTCONbits.GIE = 1`.
  3. **Write the Interrupt Service Routine:**
     - Use `void __interrupt() isr(void)`.
     - Check `if (INTCONbits.T0IF && INTCONbits.T0IE)`.
     - **Crucial Step:** Clear `INTCONbits.T0IF = 0` immediately in software.
     - Increment `overflow_counter`. When it reaches 8 ($8 \times 65.536\text{ ms} \approx 524\text{ ms}$), toggle `PORTBbits.RB0 ^= 1` and reset the counter.

---

### 4. 10-Bit ADC Voltmeter & 16x2 LCD (`adc_voltage.c`)
- **Runnable Source:** [`adc_voltage.c`](adc_voltage.c)
- **Prerequisites to Learn First:**
  - Successive Approximation Register (SAR) ADC conversion principles.
  - Acquisition time requirement: $T_{ACQ} \ge 19.7\,\mu\text{s}$ for the internal sampling capacitor ($C_{HOLD} = 120\text{ pF}$) to settle.
  - HD44780 LCD controller 4-bit nibble protocol timing (RS, EN strobe pulses).
- **How to Construct the Code Step-by-Step:**
  1. **Configure Analog Pins in `ADCON1`:**
     - Set `ADFM = 1` (Right-justified result: 6 MSBs in `ADRESH`, 8 LSBs in `ADRESL`).
     - Set `PCFG3:PCFG0 = 1110` (Configure RA0/AN0 as analog input, all other pins as digital, $V_{REF+} = V_{DD}$, $V_{REF-} = V_{SS}$).
  2. **Configure ADC Clock in `ADCON0`:**
     - Set `ADCS1:ADCS0 = 01` (Select $F_{OSC}/8$ clock; at 4 MHz, $T_{AD} = 2\,\mu\text{s}$, which satisfies $T_{AD} \ge 1.6\,\mu\text{s}$).
     - Set `CHS2:CHS0 = 000` (Select Channel 0 / AN0).
     - Set `ADON = 1` (Power on ADC module).
  3. **Implement ADC Read Function:**
     - Wait acquisition delay: `__delay_us(20)`.
     - Start conversion: `ADCON0bits.GO_nDONE = 1`.
     - Poll until hardware clears bit: `while(ADCON0bits.GO_nDONE);`.
     - Combine registers: `return ((unsigned int)ADRESH << 8) | ADRESL;`.
  4. **Convert to Millivolts Without Floating-Point:**
     - Calculate $mV = \frac{\text{raw} \times 5000}{1023}$ using 32-bit unsigned math (`unsigned long`).
     - Extract integer volts ($mV / 1000$) and decimals ($mV \% 1000$).
  5. **Display on LCD:** Send command `0xC0` (second line) and format ASCII digits using `LCD_Char('0' + digit)`.

---

### 5. Hardware PWM Dimmer (`pwm_dimmer.c`)
- **Runnable Source:** [`pwm_dimmer.c`](pwm_dimmer.c)
- **Prerequisites to Learn First:**
  - Capture/Compare/PWM (CCP) hardware module architecture.
  - Timer2 prescaler and period register `PR2` relationship with PWM frequency:
    $$\text{PWM Period} = (PR2 + 1) \times 4 \times T_{OSC} \times (\text{TMR2 Prescale})$$
- **How to Construct the Code Step-by-Step:**
  1. **Calculate PR2 for Desired Frequency:**
     - For $1\text{ kHz}$ at $F_{OSC} = 4\text{ MHz}$ ($T_{OSC} = 0.25\,\mu\text{s}$) with Timer2 Prescaler = 4:
       $$1000\,\mu\text{s} = (PR2 + 1) \times 4 \times 0.25\,\mu\text{s} \times 4 \implies PR2 = \frac{1000}{4} - 1 = 249$$
  2. **Configure Pin Direction:** Set `TRISCbits.TRISC2 = 0` (RC2 is physical CCP1 output).
  3. **Load Period:** Set `PR2 = 249`.
  4. **Configure CCP1CON:** Set `CCP1CON = 0b00001100` (Selects PWM mode).
  5. **Configure & Start Timer2:** Set `T2CON = 0b00000101` (Prescaler = 4, `TMR2ON = 1`).
  6. **Write Duty Cycle Setter:**
     - Upper 8 bits of 10-bit duty cycle go into `CCPR1L = duty >> 2`.
     - Lower 2 bits go into `CCP1CONbits.CCP1X` and `CCP1CONbits.CCP1Y`.
  7. **Ramp Brightness:** In the superloop, sweep duty from 0 to 1000 with smooth delay increments.

---

### 6. Full-Duplex USART Echo (`uart_echo.c`)
- **Runnable Source:** [`uart_echo.c`](uart_echo.c)
- **Prerequisites to Learn First:**
  - RS-232 / UART asynchronous frame format: 1 Start bit, 8 Data bits, No parity, 1 Stop bit (8-N-1).
  - Baud rate error calculation formula:
    $$SPBRG = \frac{F_{OSC}}{16 \times \text{Baud}} - 1 \quad (\text{with } BRGH = 1)$$
- **How to Construct the Code Step-by-Step:**
  1. **Configure Pin Directions:** Set `TRISCbits.TRISC6 = 0` (TX output) and `TRISCbits.TRISC7 = 1` (RX input).
  2. **Calculate & Set Baud Rate:** At 4 MHz and 9600 baud:
     $$SPBRG = \frac{4000000}{16 \times 9600} - 1 = 25.04 \implies SPBRG = 25 \quad (+0.16\% \text{ error})$$
  3. **Configure Transmit Register `TXSTA`:** Set `TXSTA = 0b00100100` (`TXEN = 1`, `BRGH = 1`, `SYNC = 0`).
  4. **Configure Receive Register `RCSTA`:** Set `RCSTA = 0b10010000` (`SPEN = 1` enables serial port, `CREN = 1` enables continuous receive).
  5. **Implement Transmit:** Wait until `PIR1bits.TXIF == 1`, then write byte to `TXREG`.
  6. **Implement Receive with Overrun Recovery:**
     - Check `if (RCSTAbits.OERR) { RCSTAbits.CREN = 0; RCSTAbits.CREN = 1; }` to reset receiver on buffer overrun.
     - Wait until `PIR1bits.RCIF == 1`.
     - Read and return `RCREG` (reading automatically clears the `RCIF` flag!).

---

### 7. Hardware I2C Master Digital Sensor (`i2c_temp.c`)
- **Runnable Source:** [`i2c_temp.c`](i2c_temp.c)
- **Prerequisites to Learn First:**
  - I2C bus specifications: Open-drain lines (SDA, SCL) with physical $4.7\text{ k}\Omega$ pull-up resistors.
  - Master Synchronous Serial Port (MSSP) state machine and bus idle checking.
  - I2C protocol framing: Start condition $\to$ 7-bit Address + R/W bit $\to$ ACK $\to$ Register pointer $\to$ Repeated Start $\to$ Read data $\to$ NACK $\to$ Stop condition.
- **How to Construct the Code Step-by-Step:**
  1. **Configure Pin Direction:** Set `TRISCbits.TRISC3 = 1` (SCL) and `TRISCbits.TRISC4 = 1` (SDA) as inputs per Microchip I2C specification.
  2. **Calculate Clock Divider `SSPADD`:** For 100 kHz standard mode at 4 MHz:
     $$SSPADD = \frac{F_{OSC}}{4 \times \text{Clock}} - 1 = \frac{4000000}{4 \times 100000} - 1 = 9$$
  3. **Configure MSSP Registers:**
     - Set `SSPSTATbits.SMP = 1` (Slew rate disabled for 100 kHz).
     - Set `SSPCON = 0b00101000` (`SSPEN = 1`, `SSPM3:SSPM0 = 1000` for I2C Master mode).
  4. **Implement Bus Idle Wait:**
     `while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_nW));` waits until all active bus events (SEN, RSEN, PEN, RCEN, ACKEN) finish.
  5. **Write Transaction Functions:** Construct `I2C_Start()`, `I2C_RepeatedStart()`, `I2C_Stop()`, `I2C_Write()`, and `I2C_Read()`.
  6. **Implement Sensor Query:** Transmit device address `(0x48 << 1) | 0`, write command pointer `0x00`, repeated start with `(0x48 << 1) | 1`, read signed temperature byte with NACK, and send Stop condition.

---

### 8. Alphanumeric LCD Driver (`lcd_hello.c`)
- **Runnable Source:** [`lcd_hello.c`](lcd_hello.c)
- **Prerequisites to Learn First:**
  - Hitachi HD44780 industry-standard LCD controller command set.
  - 4-bit bus interface: Transmitting an 8-bit command or character requires splitting it into high nibble then low nibble with an enable (`EN`) pulse strobe.
  - Display DDRAM memory addresses: Line 1 starts at `0x80`, Line 2 starts at `0xC0`.
- **How to Construct the Code Step-by-Step:**
  1. **Define Port Mapping:** Map `LCD_RS` to `RD2`, `LCD_EN` to `RD3`, and `LCD_D4:D7` to `RD4:RD7`.
  2. **Set Direction:** Write `TRISD = 0x00` (all output).
  3. **Implement Nibble Strobe:**
     - Put 4 data bits onto `RD4:RD7`.
     - Set `LCD_EN = 1`, delay $5\,\mu\text{s}$, set `LCD_EN = 0`, delay $50\,\mu\text{s}$.
  4. **Implement Strict Power-Up Initialization:**
     - Wait $> 15\text{ ms}$ for internal LCD power-on reset.
     - Send three consecutive `0x03` nibbles with required delays ($5\text{ ms}$, $150\,\mu\text{s}$).
     - Send `0x02` nibble to switch controller into 4-bit mode.
     - Send function set command `0x28` (4-bit, 2 lines, 5x8 font).
     - Send display control command `0x0C` (Display ON, cursor OFF).
     - Send clear display command `0x01` (requires $> 1.64\text{ ms}$ delay).
  5. **Implement Text Printing:** Construct `LCD_Char()`, `LCD_Print()`, and `LCD_SetCursor(row, col)`.
