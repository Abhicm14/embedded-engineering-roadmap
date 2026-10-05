# 🟡 Intermediate Steps: PIC Peripherals Mastery

> Steps 4–8 cover essential microcontroller peripherals: 10-bit Successive Approximation ADC, Hardware PWM via CCP1, USART Serial Communications, Hardware I2C Master, and Keypad Matrix Interfacing.

---

## 🧭 Foundational Prerequisites

Before attempting peripheral driver development, ensure you have completed:
1. [**Foundational Prerequisites (`PREREQUISITES.md`)**](../PREREQUISITES.md): Number systems, bitwise manipulation, open-drain vs push-pull, and ISR rules.
2. [**Beginner Steps (`beginner.md`)**](beginner.md): GPIO direction, hardware delays, and Timer0 interrupt flow.
3. [**XC8 Step-by-Step Code Construction Guide**](../code-examples/xc8/README.md): Detailed step-by-step assembly guides for all peripheral drivers.
4. [**PIC16F87XA Datasheet (DS39582C)**](../resources/datasheets-and-reference.md#4-microchip-pic-silicon--compiler-references): Sections 8 (CCP), 9 (MSSP), 10 (USART), 11 (ADC).

---

## Step 4: 10-Bit ADC & 16x2 Character LCD Display

### Goal
Read analog voltage from a potentiometer on RA0/AN0, convert the 10-bit reading to millivolts without floating-point arithmetic, and format the output on an HD44780 16x2 character LCD in 4-bit mode.

### Concept Explanation

> 💡 **In Plain English:**  
> Computers only understand pure 0 and 1, but the real world isn't black-and-white—it's colorful and continuous! A volume knob, a temperature sensor, or a light detector can sit at 2.37 Volts.  
> An **ADC (Analog-to-Digital Converter)** is like a digital measuring tape. It measures the physical voltage between 0V and 5V and turns it into a number between 0 and 1023 (10-bit resolution). If the knob is at 0V, you read 0. If it's halfway at 2.5V, you read ~512. If turned all the way to 5V, you read 1023!

#### Successive Approximation Register (SAR) ADC
- PIC16F877A includes an 8-channel, 10-bit SAR analog-to-digital converter.
- **Conversion Clock ($T_{AD}$)**: Minimum $1.6\,\mu\text{s}$ required by silicon physics. At $F_{OSC} = 4\text{ MHz}$, selecting $F_{OSC}/8$ gives $T_{AD} = 2\,\mu\text{s}$ (safe).
- **Acquisition Delay ($T_{ACQ}$)**: The internal holding capacitor ($C_{HOLD} = 120\text{ pF}$) requires at least $19.7\,\mu\text{s}$ to charge to the input voltage level through the source impedance before setting the `GO_nDONE` bit.
- **Registers**:
  - `ADCON0`: Clock select (`ADCS1:0`), channel select (`CHS2:0`), start bit (`GO_nDONE`), and module enable (`ADON`).
  - `ADCON1`: Result formatting (`ADFM = 1` for right-justified) and port configuration (`PCFG3:0` to select which pins are analog vs digital).

#### HD44780 4-Bit Nibble Protocol
- Sending an 8-bit command or ASCII character requires splitting the byte into an upper 4-bit nibble and a lower 4-bit nibble.
- Strobe `EN` high for $> 450\text{ ns}$ to latch each nibble into the LCD display controller.

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Define Pin Mappings**
   Define `LCD_RS` as `PORTDbits.RD2`, `LCD_EN` as `PORTDbits.RD3`, and `LCD_D4:D7` as `PORTDbits.RD4:RD7` (standard PICSimLab Board 1 mapping).
2. **Step 2: Configure ADC Registers in `ADC_Init()`**
   - In `ADCON1`, set `ADFM = 1` (right-justified) and `PCFG3:PCFG0 = 1110` (only AN0 is analog, rest digital, $V_{REF+} = V_{DD}$, $V_{REF-} = V_{SS}$).
   - In `ADCON0`, set `ADCS1:0 = 01` ($F_{OSC}/8$), `CHS2:0 = 000` (AN0), and `ADON = 1`.
   - Set `TRISAbits.TRISA0 = 1` (Input).
3. **Step 3: Implement Acquisition and Read Function**
   In `ADC_Read()`: Wait acquisition delay `__delay_us(20);`, set `ADCON0bits.GO_nDONE = 1;`, poll `while(ADCON0bits.GO_nDONE);`, and return `((unsigned int)ADRESH << 8) | ADRESL;`.
4. **Step 4: Scale Millivolts Without Floating-Point Math**
   Compute millivolts using integer math: `unsigned long mv = ((unsigned long)raw * 5000UL) / 1023UL;`. Extract volts (`mv / 1000`) and decimals (`mv % 1000`).
5. **Step 5: Output to LCD**
   Send command `0xC0` to move cursor to line 2, and print characters using `LCD_Char((char)('0' + digit));`.

---

### Annotated Reference Implementation
*Standalone File:* [`code-examples/xc8/adc_voltage.c`](../code-examples/xc8/adc_voltage.c)

```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

#define LCD_RS PORTDbits.RD2
#define LCD_EN PORTDbits.RD3
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7

void LCD_PulseEnable(void) {
    LCD_EN = 1; __delay_us(5); LCD_EN = 0; __delay_us(50);
}

void LCD_SendNibble(unsigned char n) {
    LCD_D4 = (n >> 0) & 1; LCD_D5 = (n >> 1) & 1;
    LCD_D6 = (n >> 2) & 1; LCD_D7 = (n >> 3) & 1;
    LCD_PulseEnable();
}

void LCD_Command(unsigned char cmd) {
    LCD_RS = 0;
    LCD_SendNibble(cmd >> 4);
    LCD_SendNibble(cmd & 0x0F);
    if (cmd == 0x01 || cmd == 0x02) __delay_ms(2);
}

void LCD_Char(char data) {
    LCD_RS = 1;
    LCD_SendNibble(data >> 4);
    LCD_SendNibble(data & 0x0F);
}

void LCD_Print(const char *str) {
    while (*str) LCD_Char(*str++);
}

void LCD_Init(void) {
    TRISD = 0x00; PORTD = 0x00; __delay_ms(20);
    LCD_RS = 0;
    LCD_SendNibble(0x03); __delay_ms(5);
    LCD_SendNibble(0x03); __delay_us(150);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02); // 4-bit mode
    LCD_Command(0x28); LCD_Command(0x0C); LCD_Command(0x06); LCD_Command(0x01);
}

void ADC_Init(void) {
    TRISAbits.TRISA0 = 1;
    ADCON1 = 0b10001110; // Right-justified, AN0 analog, VDD/VSS ref
    ADCON0 = 0b01000001; // Fosc/8, Channel 0, ADC ON
}

unsigned int ADC_Read(void) {
    __delay_us(20);              // Acquisition delay (Tacq >= 19.7 us)
    ADCON0bits.GO_nDONE = 1;     // Start conversion
    while (ADCON0bits.GO_nDONE); // Wait for hardware to clear bit
    return ((unsigned int)ADRESH << 8) | ADRESL;
}

void main(void) {
    LCD_Init();
    ADC_Init();
    LCD_Command(0x80);
    LCD_Print("PIC16F877A ADC");

    while (1) {
        unsigned int raw = ADC_Read();
        unsigned long mv = ((unsigned long)raw * 5000UL) / 1023UL;
        unsigned int volts = (unsigned int)(mv / 1000);
        unsigned int decimals = (unsigned int)(mv % 1000);

        LCD_Command(0xC0);
        LCD_Print("Volt: ");
        LCD_Char((char)('0' + volts));
        LCD_Char('.');
        LCD_Char((char)('0' + (decimals / 100)));
        LCD_Char((char)('0' + ((decimals / 10) % 10)));
        LCD_Char((char)('0' + (decimals % 10)));
        LCD_Print(" V   ");

        __delay_ms(250);
    }
}
```

---

## Step 5: Hardware PWM via CCP1

### Goal
Generate a hardware-timed, flicker-free 1 kHz Pulse Width Modulation (PWM) signal on RC2 using the Capture/Compare/PWM (CCP1) module and Timer2.

### Concept Explanation

> 💡 **In Plain English:**  
> *How do you dim an LED or slow down a DC fan motor if the chip can only output 0V or 5V?*  
> You flick the switch ON and OFF thousands of times a second!  
> If the switch is ON for 50% of the time and OFF for 50% of the time, your human eyes see half-brightness, and a motor spins at half-speed! That is **PWM (Pulse Width Modulation)**: the percentage of time the signal stays ON is called the **Duty Cycle**.

#### CCP Hardware Architecture & PWM Period
- The CCP1 hardware module on pin **RC2 (Pin 17)** operates independently of the CPU core once initialized.
- **Timebase:** Driven by **Timer2**.
- **Period Register (`PR2`):**
  $$\text{PWM Period} = (PR2 + 1) \times 4 \times T_{OSC} \times (\text{TMR2 Prescaler})$$
- At $F_{OSC} = 4\text{ MHz}$ ($T_{OSC} = 0.25\,\mu\text{s}$) with Timer2 Prescaler = 4:
  $$1000\,\mu\text{s} = (PR2 + 1) \times 4 \times 0.25\,\mu\text{s} \times 4 = (PR2 + 1) \times 4\,\mu\text{s} \implies PR2 = 249$$
- **10-Bit Duty Cycle Resolution:**
  - Upper 8 bits are loaded into `CCPR1L`.
  - Lower 2 bits are loaded into `CCP1CON<5:4>` (`CCP1X` and `CCP1Y`).

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Configure Output Pin Direction**
   Set `TRISCbits.TRISC2 = 0;` (RC2 is physical CCP1 output).
2. **Step 2: Load the Period Register**
   Set `PR2 = 249;` to set frequency to 1.0 kHz.
3. **Step 3: Enable PWM Mode in `CCP1CON`**
   Set `CCP1CON = 0b00001100;` (Bits 3:0 = 1100 sets PWM mode).
4. **Step 4: Configure and Enable Timer2**
   Set `T2CON = 0b00000101;` (`T2CKPS1:0 = 01` sets 1:4 prescaler, `TMR2ON = 1` starts timer).
5. **Step 5: Write the Duty Cycle Setter**
   In `PWM1_Set_Duty(unsigned int duty)`:
   - Write upper 8 bits: `CCPR1L = (unsigned char)(duty >> 2);`.
   - Write lower 2 bits: `CCP1CONbits.CCP1X = (duty >> 1) & 1;` and `CCP1CONbits.CCP1Y = duty & 1;`.
6. **Step 6: Create Fading Loop in `main()`**
   Sweep duty from 0 to 1000 in increments of 10 with a 15 ms delay between steps.

---

### Annotated Reference Implementation
*Standalone File:* [`code-examples/xc8/pwm_dimmer.c`](../code-examples/xc8/pwm_dimmer.c)

```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void PWM1_Init_1kHz(void) {
    TRISCbits.TRISC2 = 0;       // RC2/CCP1 as output
    PR2 = 249;                  // 1 kHz period at 4 MHz with 1:4 prescaler
    CCP1CON = 0b00001100;       // PWM mode
    CCPR1L = 0;                 // Start with 0% duty
    T2CON = 0b00000101;         // Timer2 ON, Prescaler 1:4
}

void PWM1_Set_Duty(unsigned int duty) {
    if (duty > 1000) duty = 1000;
    CCPR1L = (unsigned char)(duty >> 2);
    CCP1CONbits.CCP1X = (duty >> 1) & 1;
    CCP1CONbits.CCP1Y = duty & 1;
}

void main(void) {
    PWM1_Init_1kHz();
    while (1) {
        for (unsigned int d = 0; d <= 1000; d += 10) {
            PWM1_Set_Duty(d);
            __delay_ms(15);
        }
        for (int d = 1000; d >= 0; d -= 10) {
            PWM1_Set_Duty((unsigned int)d);
            __delay_ms(15);
        }
    }
}
```

---

## Step 6: Full-Duplex USART Serial Communication

### Goal
Configure the Universal Synchronous Asynchronous Receiver Transmitter (USART) for 9600 baud, 8-N-1 communication with a host PC, handling receiver overrun errors.

### Concept Explanation

> 💡 **In Plain English:**  
> Imagine you and a friend are in different rooms, communicating with flashlights through a window using Morse code!  
> You must both agree beforehand on how fast you will flash (the **Baud Rate**, e.g. 9600 flashes per second). When you want to send a letter, you start with a flash (Start bit), send 8 flashes for the character bits, and finish with a pause (Stop bit). That is **UART**!

#### Hardware Architecture & Baud Generation
- Pin **RC6** is physical USART TX (Pin 25, Output).
- Pin **RC7** is physical USART RX (Pin 26, Input).
- **High-Speed Baud Formula (`BRGH = 1`):**
  $$\text{Baud} = \frac{F_{OSC}}{16 \times (SPBRG + 1)} \implies SPBRG = \frac{4000000}{16 \times 9600} - 1 = 25.04 \approx 25$$
  Actual Baud = $9615.38$ baud ($+0.16\%$ error rate, well within the $\pm 2\%$ RS-232 tolerance window).
- **Overrun Error (`OERR`):** If a third byte arrives before `RCREG` is read, hardware halts reception. Firmware must reset `CREN` to clear the error.

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Set Pin Directions**
   `TRISCbits.TRISC6 = 0;` (TX output) and `TRISCbits.TRISC7 = 1;` (RX input).
2. **Step 2: Load Baud Rate Register**
   Set `SPBRG = 25;`.
3. **Step 3: Configure Transmit Control Register (`TXSTA`)**
   Set `TXSTA = 0b00100100;` (`TXEN = 1` enables transmitter, `BRGH = 1` enables high-speed multiplier).
4. **Step 4: Configure Receive Control Register (`RCSTA`)**
   Set `RCSTA = 0b10010000;` (`SPEN = 1` enables serial port, `CREN = 1` enables continuous receiver).
5. **Step 5: Write Transmit and Receive Drivers**
   - Transmit: Poll `while(!PIR1bits.TXIF);`, then write `TXREG = c;`.
   - Receive: If `RCSTAbits.OERR` is set, clear via `RCSTAbits.CREN = 0; RCSTAbits.CREN = 1;`. Then poll `while(!PIR1bits.RCIF);` and return `RCREG;`.

---

### Annotated Reference Implementation
*Standalone File:* [`code-examples/xc8/uart_echo.c`](../code-examples/xc8/uart_echo.c)

```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void UART_Init(void) {
    TRISCbits.TRISC6 = 0;       // RC6 = TX
    TRISCbits.TRISC7 = 1;       // RC7 = RX
    SPBRG = 25;                 // 9600 baud at 4 MHz (BRGH=1)
    TXSTA = 0b00100100;         // TXEN=1, BRGH=1
    RCSTA = 0b10010000;         // SPEN=1, CREN=1
}

void UART_Write(char c) {
    while (!PIR1bits.TXIF);
    TXREG = c;
}

void UART_Print(const char *str) {
    while (*str) UART_Write(*str++);
}

char UART_Read(void) {
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;     // Reset receiver on overrun
        RCSTAbits.CREN = 1;
    }
    while (!PIR1bits.RCIF);     // Wait for character
    return RCREG;               // Reading RCREG clears RCIF flag
}

void main(void) {
    UART_Init();
    __delay_ms(100);
    UART_Print("\r\nPIC16F877A UART Ready\r\n> ");

    while (1) {
        char ch = UART_Read();
        UART_Write(ch);         // Echo back
        if (ch == '\r') {
            UART_Write('\n');
            UART_Print("> ");
        }
    }
}
```

---

## Step 7: Hardware I2C Master Digital Sensor Interface

### Goal
Configure the Master Synchronous Serial Port (MSSP) as an I2C bus master to query a digital thermal sensor (Microchip TC74 / LM75) at 100 kHz.

### Concept Explanation

> 💡 **In Plain English:**  
> Imagine a classroom with 1 teacher (Master) and 30 students (Sensors/Slaves). Everyone is connected by one shared megaphone wire.  
> When the teacher speaks: *'Student #48, what is your temperature reading?'*  
> All other students stay quiet. Only Student #48 speaks up and answers! That is **I2C**: two wires (Clock and Data) shared by dozens of sensor chips, where each chip has its own unique address number!

#### MSSP I2C Protocol Engine
- Pin **RC3** is serial clock (`SCL`, Pin 18).
- Pin **RC4** is serial data (`SDA`, Pin 23).
- **Open-Drain Requirement:** Both pins must have physical $4.7\text{ k}\Omega$ pull-up resistors to 5V, and `TRISCbits` must be set to `1` (Inputs) to let open-drain hardware control bus lines.
- **Clock Divider Register (`SSPADD`):**
  $$\text{Clock} = \frac{F_{OSC}}{4 \times (SSPADD + 1)} \implies SSPADD = \frac{4000000}{4 \times 100000} - 1 = 9$$
- **Bus Idle Check:** Before initiating a Start, Stop, or Byte transfer, firmware must verify that the bus is not busy:
  ```c
  while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_nW));
  ```

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Set Pin Directions for I2C Open-Drain**
   Set `TRISCbits.TRISC3 = 1;` and `TRISCbits.TRISC4 = 1;`.
2. **Step 2: Configure Clock Rate**
   Set `SSPADD = 9;` for 100 kHz standard mode.
3. **Step 3: Configure Control Registers**
   Set `SSPSTATbits.SMP = 1;` (Standard speed slew rate) and `SSPCON = 0b00101000;` (`SSPEN = 1`, `SSPM3:0 = 1000` for Master mode).
4. **Step 4: Implement Bus Control Functions**
   Write `I2C_WaitIdle()`, `I2C_Start()`, `I2C_RepeatedStart()`, `I2C_Stop()`, `I2C_Write()`, and `I2C_Read()`.
5. **Step 5: Write Sensor Query Routine**
   Start $\to$ Send `(0x48 << 1) | 0` (Write) $\to$ Send command `0x00` $\to$ Repeated Start $\to$ Send `(0x48 << 1) | 1` (Read) $\to$ Read byte with NACK $\to$ Stop.

---

### Annotated Reference Implementation
*Standalone File:* [`code-examples/xc8/i2c_temp.c`](../code-examples/xc8/i2c_temp.c)

---

## Step 8: 4x4 Matrix Keypad Scanning

### Goal
Interface a 16-key matrix keypad using 8 GPIO pins (4 rows, 4 columns) with minimal pin count using row-scanning and internal weak pull-ups.

### Concept Explanation

> 💡 **In Plain English:**  
> If you have a 16-key keypad (4 rows and 4 columns), connecting 16 separate wires would eat up almost all the pins on your microcontroller!  
> Instead, we arrange the switches in a **grid (matrix)** of 4 rows and 4 columns, requiring only 8 wires.  
> The microcontroller powers Row 1 and checks the 4 columns: did a button click? Then it powers Row 2 and checks again. It scans the grid so fast (thousands of times a second) that it catches your finger press instantly!

#### Matrix Multiplexing Principle
Instead of dedicating 16 individual microcontroller pins to 16 buttons, a matrix organizes switches at the intersections of 4 output rows and 4 input columns:
```
           Col 0 (RB4)   Col 1 (RB5)   Col 2 (RB6)   Col 3 (RB7)  [Inputs with Pull-Ups]
               │             │             │             │
  Row 0 (RB0) ─┼──[ '1' ]────┼──[ '2' ]────┼──[ '3' ]────┼──[ 'A' ]
               │             │             │             │
  Row 1 (RB1) ─┼──[ '4' ]────┼──[ '5' ]────┼──[ '6' ]────┼──[ 'B' ]
               │             │             │             │
  Row 2 (RB2) ─┼──[ '7' ]────┼──[ '8' ]────┼──[ '9' ]────┼──[ 'C' ]
               │             │             │             │
  Row 3 (RB3) ─┼──[ '*' ]────┼──[ '0' ]────┼──[ '#' ]────┼──[ 'D' ]
 [Outputs]
```

- Rows are driven sequentially LOW one by one while keeping other rows HIGH.
- Columns are read. If a button in the active row is pressed, that column line is pulled LOW.

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Set Port Directions**
   Configure lower nibble `RB0:RB3` as outputs (Rows) and upper nibble `RB4:RB7` as inputs (Columns): `TRISB = 0xF0;`.
2. **Step 2: Enable Internal Pull-Ups**
   Clear `OPTION_REGbits.nRBPU = 0;` so columns idle at logic HIGH without external resistors.
3. **Step 3: Define Character Keymap**
   Declare a $4 \times 4$ constant lookup array `const char keymap[4][4] = {{'1','2','3','A'}, ...};`.
4. **Step 4: Scan Rows Sequentially**
   Drive Row 0 LOW (`PORTB = 0xFE;`). Delay $10\,\mu\text{s}$ for line stabilization. Read columns `(PORTB >> 4) & 0x0F`. If a bit is 0, return the corresponding keymap character. Repeat for Rows 1, 2, and 3.

---

## 🏁 Intermediate Verification Checklist

- [ ] 10-Bit ADC verified: Voltage scales linearly from 0.00 V to 5.00 V on LCD.
- [ ] CCP1 PWM verified: 1 kHz frequency confirmed on oscilloscope/PICSimLab.
- [ ] USART serial echo verified at 9600 baud with no dropped bytes.
- [ ] I2C transactions verified: Start, ACK, data read, and Stop frames observed cleanly.
- [ ] 4x4 Keypad verified: All 16 keys detect reliably without ghosting.
- [ ] Proceed to [**Advanced Steps (Steps 9–13)**](advanced.md).
