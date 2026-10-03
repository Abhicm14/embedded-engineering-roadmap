# 🟡 Intermediate Steps: PIC Peripherals Mastery

> Steps 4-8 cover essential peripherals: ADC, PWM, UART, SPI/I2C, and LCD interfacing.

## Step 4: ADC & LCD Display

### Goal
Read analog voltage from a potentiometer (simulated via PICSimLab slider) and display the value on a character LCD.

### Prerequisites
- Completion of Beginner Steps
- Understanding of PIC16F877A ADC module
- Familiarity with LCD HD44780 timing (datasheet review recommended)

### Concept Explanation

#### ADC Successive Approximation
- PIC16F877A has 8-channel, 10-bit ADC
- Conversion time: Minimum 1.6µs (Tad), 11Tad per conversion
- **Result registers**: ADRESH (high byte), ADRESL (low byte)
- **Control registers**: ADCON0 (select channel, start conversion), ADCON1 (port config), ADCON2 (clock/justification)

#### LCD HD44780 Interface (4-bit mode)
- Saves 4 GPIO pins vs 8-bit mode
- Requires precise timing: Enable pulse >450ns
- Initialization sequence: Critical for proper operation
- **Commands**: 0x38 (8-bit init), 0x0C (display on, cursor off), 0x06 (increment cursor)
- **Data**: Send high nibble then low nibble with enable toggle

### Minimal XC8 Snippet
```c
#include <xc.h>

#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

// Define LCD pins
#define LCD_RS LATD0
#define LCD_EN LATD1
#define LCD_D4 LATD2
#define LCD_D5 LATD3
#define LCD_D6 LATD4
#define LCD_D7 LATD5
#define LCD_TRIS TRISD

void LCD_Init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Data(unsigned char data);
void LCD_String(const char *str);
void ADC_Init(void);
unsigned int ADC_Read(void);

void main(void) {
    unsigned int adc_value;
    char buffer[16];
    
    // Configure PORTD for LCD (lower 6 bits output)
    LCD_TRIS = 0x00;
    TRISA0 = 1;   // RA0/AN0 as input (potentiometer)
    
    LCD_Init();
    ADC_Init();
    
    LCD_Cmd(0x80);  // First line
    LCD_String("ADC Value:");
    
    while(1) {
        adc_value = ADC_Read();  // Read channel 0 (AN0)
        
        // Convert ADC value to string (0-1023)
        if (adc_value > 999) {
            buffer[0] = '1';
            buffer[1] = (adc_value / 100) % 10 + '0';
            buffer[2] = (adc_value / 10) % 10 + '0';
            buffer[3] = adc_value % 10 + '0';
            buffer[4] = '\0';
        } else if (adc_value > 99) {
            buffer[0] = '0';
            buffer[1] = (adc_value / 100) % 10 + '0';
            buffer[2] = (adc_value / 10) % 10 + '0';
            buffer[3] = adc_value % 10 + '0';
            buffer[4] = '\0';
        } else if (adc_value > 9) {
            buffer[0] = '0';
            buffer[1] = '0';
            buffer[2] = (adc_value / 10) % 10 + '0';
            buffer[3] = adc_value % 10 + '0';
            buffer[4] = '\0';
        } else {
            buffer[0] = '0';
            buffer[1] = '0';
            buffer[2] = '0';
            buffer[3] = adc_value % 10 + '0';
            buffer[4] = '\0';
        }
        
        LCD_Cmd(0xC0);  // Second line
        LCD_String(buffer);
        __delay_ms(200);
    }
}

void ADC_Init(void) {
    ADCON0 = 0x01;             // ADC on, Fosc/16
    ADCON1 = 0x80;             // Right justified, VDD reference
    ADCON2 = 0x92;             // Right justified, 12Tosc, Fosc/32
    TRISA0 = 1;                // AN0 as input
    ANSEL0 = 1;                // AN0 as analog
}

unsigned int ADC_Read(void) {
    GO_nDONE = 1;              // Start conversion
    while(GO_nDONE);           // Wait for completion
    return ((ADRESH << 8) + ADRESL);
}

// LCD Functions (4-bit mode)
void LCD_Init(void) {
    __delay_ms(20);            // Power-on delay
    LCD_Cmd(0x02);             // Return home
    LCD_Cmd(0x28);             // 4-bit mode, 2 lines, 5x8 font
    LCD_Cmd(0x0C);             // Display on, cursor off
    LCD_Cmd(0x06);             // Increment cursor
    LCD_Cmd(0x01);             // Clear display
    __delay_ms(2);
}

void LCD_Cmd(unsigned char cmd) {
    LCD_RS = 0;                // Command mode
    
    // High nibble
    LCD_D4 = (cmd & 0x10) >> 4;
    LCD_D5 = (cmd & 0x20) >> 5;
    LCD_D6 = (cmd & 0x40) >> 6;
    LCD_D7 = (cmd & 0x80) >> 7;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    
    // Low nibble
    LCD_D4 = (cmd & 0x01);
    LCD_D5 = (cmd & 0x02) >> 1;
    LCD_D6 = (cmd & 0x04) >> 2;
    LCD_D7 = (cmd & 0x08) >> 3;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_ms(2);
}

void LCD_Data(unsigned char data) {
    LCD_RS = 1;                // Data mode
    
    // High nibble
    LCD_D4 = (data & 0x10) >> 4;
    LCD_D5 = (data & 0x20) >> 5;
    LCD_D6 = (data & 0x40) >> 6;
    LCD_D7 = (data & 0x80) >> 7;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    
    // Low nibble
    LCD_D4 = (data & 0x01);
    LCD_D5 = (data & 0x02) >> 1;
    LCD_D6 = (data & 0x04) >> 2;
    LCD_D7 = (data & 0x08) >> 3;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
}

void LCD_String(const char *str) {
    while(*str) {
        LCD_Data(*str++);
    }
}
```

### PICSimLab Procedure
1. **Select LCD board**: Boards → PIC16F877A → LCD + Keypad + Potentiometer
2. **Wire connections**: Confirm RA0 → Potentiometer, PORTD → LCD
3. **Build and load**: Create project, compile, debug in PICSimLab
4. **Adjust pot**: Use virtual potentiometer slider and observe LCD updates

### Expected Result
- LCD displays "ADC Value:" on first line
- Numeric value (0-1023) updates on second line as potentiometer changes
- No garbled characters (indicates proper timing)

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| Blank LCD | Initialization failed | Add 20ms delay before init |
| Garbled chars | LCD timing off | Verify enable pulse width |
| Always reads 1023 | Analog input not configured | Set ANSEL0 = 1 |
| Stale readings | Missing acquisition delay | Add delay before GO_nDONE |
| Wrong channel | Channel bits not cleared | Clear CHS[2:0] before setting |

### 🔗 Next Step
[Step 5: PWM via CCP](#step-5-pwm-via-ccp)

> **Register Focus**: ADCON0, ADCON1, ADCON2 for ADC; TRISD, LATD for LCD; ANSEL for analog functions.

---

## Step 5: PWM via CCP

### Goal
Generate PWM signals using the Capture/Compare/PWM (CCP) module for LED dimming or motor speed control.

### Prerequisites
- Completion of Step 4
- Understanding of CCP module operation
- Familiarity with Timer2 and PWM timing

### Concept Explanation

#### CCP Module (PIC16F877A)
- Capture/Compare/PWM (CCP1 and CCP2 modules)
- Configurable in PWM mode using CCPxCON registers
- 10-bit resolution for PWM duty cycle (CCPR1L:CCPxCON<5:4>)
- Controlled by Timer2 or Timer1
- Configurable output polarity and enable

#### PWM Timing Formula
```
PWM_Frequency = Fosc / (4 * (PR2+1) * (TMR2_Prescaler))
Duty_Cycle = (CCPR1L << 2) / (4 * (PR2+1))  // 10-bit resolution
```

### Minimal XC8 Snippet
```c
#include <xc.h>

#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void main(void) {
    // Configure RB0 as output for LED (CCP1 output)
    TRISB0 = 0;
    RB0 = 0;
    
    // Set PWM period
    // For 1kHz PWM: PR2 = (Fosc/4/1000) - 1 = (4000000/4/1000) - 1 = 999
    // Use 1:16 prescaler for easier PR2 calculation
    PR2 = 0x3C;                // PWM period = 1kHz with 1:16 prescaler
    
    // Configure CCP1 for PWM
    CCP1CON = 0x0C;            // PWM mode
    CCPR1L = 0x00;             // 0% duty cycle
    
    // Configure Timer2
    T2CON = 0x07;              // Timer2 on, 1:16 prescaler
    
    // Configure RC1 (LCD backlight PWM)
    TRISC1 = 0;                // RC1 as output
    PR1 = 0x3C;                // Same period
    CCP2CON = 0x0C;            // PWM mode for CCP2
    CCPR2L = 0x00;             // 0% duty cycle initially
    T1CON = 0x00;              // Timer1 on, 1:16 prescaler
    
    while(1) {
        // Gradually increase LED brightness
        for(int duty = 0; duty <= 0xFF; duty++) {
            CCPR1L = duty >> 2;  // Set duty cycle (0-100%)
            __delay_ms(10);
        }
        
        // Gradually decrease LED brightness
        for(int duty = 0xFF; duty >= 0; duty--) {
            CCPR1L = duty >> 2;  // Set duty cycle (0-100%)
            __delay_ms(10);
        }
    }
}
```

### PICSimLab Procedure
1. **Select PWM board**: Boards → PIC16F877A → LCD + Keypad + Potentiometer
2. **Wire LED**: Connect LED to RB0 (CCP1 output)
3. **Build and debug**: Create project and load into PICSimLab
4. **Observe PWM**: Use oscilloscope in PICSimLab to see PWM waveform

### Expected Result
- LED brightness varies smoothly from 0% to 100% and back
- PWM frequency should be stable at configured rate (e.g., 1kHz)
- Use PICSimLab's built-in oscilloscope to visualize PWM signal

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| PWM doesn't work | CCP1CON not set to PWM mode | Set CCP1CON = 0x0C |
| Wrong frequency | Incorrect PR2 value | Calculate PR2 = (Fosc/4/1000/prescale) - 1 |
| LED always on/off | CCPR1L not set correctly | Set CCPR1L = duty >> 2 |
| No output | Timer2 not enabled | Set T2CON = 0x07 |

### 🔗 Next Step
[Step 6: UART Communication](#step-6-uart-communication)

> **Register Focus**: CCP1CON, CCPR1L for PWM; PR2, T2CON for timing; TRISB0/RC1 for outputs.

---

## Step 6: UART Communication

### Goal
Implement UART (Universal Asynchronous Receiver/Transmitter) for serial communication at 9600 baud.

### Prerequisites
- Completion of Steps 1-5
- Understanding of UART framing (start/stop bits)
- Familiarity with PIC16F877A's EUSART module

### Concept Explanation

#### PIC16F877A EUSART Module
- Enhanced UART with reception and transmission buffers
- Controlled by TXSTA and RCSTA registers
- Baud rate set via SPBRG register
- Interrupt-driven operation available

#### Baud Rate Calculation
```
Baud Rate = Fosc / (64 * (SPBRG + 1))  // High speed mode
Baud Rate = Fosc / (16 * (SPBRG + 1))  // Low speed mode
```
For 4MHz oscillator and 9600 baud (high speed mode):
```
SPBRG = (4000000 / (64 * 9600)) - 1 = 6
```

### Minimal XC8 Snippet
```c
#include <xc.h>

#pragma config FOSC = INTOSCIO, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void UART_Init(void);
void UART_Write(char data);
char UART_Read(void);

void main(void) {
    char received_char;
    
    // Initialize UART with 9600 baud
    UART_Init();
    
    while(1) {
        // Check if data is available
        if (PIR1bits.RCIF) {
            // Read received character
            received_char = UART_Read();
            
            // Echo back the character
            UART_Write(received_char);
        }
    }
}

void UART_Init(void) {
    // Set baud rate for 9600 with 4MHz oscillator
    // SPBRG = (Fosc/(64 * Baud)) - 1 = (4000000/(64*9600)) - 1 = 6
    SPBRG = 6;                 // Baud rate 9600
    
    // Configure UART (8-bit, asynchronous, continuous receive)
    TXSTA = 0b00100000;        // Enable transmission, 8-bit, high speed
    RCSTA = 0b10010000;         // Enable reception, 8-bit, continuous
    
    // Configure pins
    TRISC6 = 0;                // TX pin as output (RC6)
    TRISC7 = 1;                // RX pin as input (RC7)
    
    // Enable interrupts
    PIE1bits.RCIE = 1;         // Enable receive interrupt
    INTCONbits.GIE = 1;         // Enable global interrupts
}

void UART_Write(char data) {
    // Wait for transmit buffer to be empty
    while (TXSTA & 0x20) {   // TXIF flag (bit 5)
        ;
    }
    
    // Write data to transmit buffer
    TXREG = data;
}

char UART_Read(void) {
    // Wait for data to be received
    while (!(PIR1bits.RCIF)) {
        ;
    }
    
    // Read data from receive buffer
    return RCREG;
}
```

### PICSimLab Procedure
1. **Select UART board**: Boards → PIC16F877A → LCD + Keypad + Potentiometer
2. **Connect UART**: Use virtual terminal in PICSimLab
3. **Build and debug**: Create project and load into PICSimLab
4. **Test communication**: Send characters via terminal and observe echo

### Expected Result
- Received characters are transmitted back (echo)
- UART communication at 9600 baud works correctly
- No data loss during transmission

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| Baud rate incorrect | Wrong SPBRG value | Use SPBRG = 6 for 9600 baud @ 4MHz |
| No echo | Transmission not enabled | Set TXSTA = 0b00100000 |
| Characters garbled | Wrong UART configuration | Ensure 8N1 (8-bit, no parity, 1 stop) |
| No data received | Receive interrupt not enabled | Enable PIE1bits.RCIE and INTCONbits.GIE |

### 🔗 Next Step
[Step 7: SPI & I2C Communication](#step-7-spi-i2c-communication)

> **Register Focus**: TXSTA, RCSTA, SPBRG for UART; TRISC6/RC7 for pins.

---

## Step 7: SPI & I2C Communication

### Goal
Implement SPI and I2C protocols for communication with external devices.

### Prerequisites
- Completion of Steps 1-6
- Understanding of synchronous vs asynchronous communication
- Basic knowledge of SPI and I2C protocols

### Concept Explanation

#### SPI (Serial Peripheral Interface)
- Full-duplex, synchronous serial communication
- Master-slave architecture
- Uses MOSI, MISO, SCK, and SS lines
- 4 clock phases (Mode 0, 1, 2, 3)

#### I2C (Inter-Integrated Circuit)
- Half-duplex, synchronous serial communication
- Multi-master, multi-slave architecture
- Uses SDA (data) and SCL (clock) lines
- Built-in addressing and data framing

### Minimal XC8 Snippets
```c
// I2C Initialization
void I2C_Init(void) {
    // Set I2C module to master mode
    SSPCON1 = 0b00110000;      // I2C Master, clock = Fosc/(4 * (SSPADD + 1))
    SSPADD = 0x0F;              // Set clock rate for 100kHz
    
    // Configure pins
    TRISC3 = 1;                // SDA as input (RC3)
    TRISC4 = 1;                // SCL as input (RC4)
}

void I2C_Start(void) {
    // Send start condition
    SSPCON2bits.SEN = 1;       // Start condition enable
    while (SSPCON2bits.SEN);  // Wait for start condition to complete
}

void I2C_Stop(void) {
    // Send stop condition
    SSPCON2bits.PEN = 1;       // Stop condition enable
    while (SSPCON2bits.PEN);  // Wait for stop condition to complete
}

void I2C_Write(char data) {
    // Write data to I2C bus
    SSPBUF = data;              // Load data into buffer
    while (!SSPSTATbits.BF);   // Wait for buffer to be full
}

char I2C_Read(char ack) {
    // Read data from I2C bus
    char data = SSPBUF;         // Read data from buffer
    
    // Send ACK or NACK
    if (ack) {
        SSPCON2bits.ACKDT = 0; // ACK
        SSPCON2bits.ACKEN = 1; // Acknowledge sequence
    } else {
        SSPCON2bits.ACKDT = 1; // NACK
        SSPCON2bits.ACKEN = 1; // Acknowledge sequence
    }
    
    while (SSPCON2bits.ACKEN); // Wait for acknowledge
    
    return data;
}

// SPI Initialization
void SPI_Init(void) {
    // Set SPI module to master mode
    SSPCON1 = 0b00100011;      // SPI Master, clock = Fosc/(4 * (SSPADD + 1))
    SSPADD = 0x0F;              // Set clock rate for 1MHz
    
    // Configure pins
    TRISC5 = 0;                // SCK as output (RC5)
    TRISC6 = 0;                // MOSI as output (RC6)
    TRISC7 = 1;                // MISO as input (RC7)
}

void SPI_Write(char data) {
    // Write data to SPI bus
    SSPBUF = data;              // Load data into buffer
    while (!SSPSTATbits.BF);   // Wait for buffer to be full
}

char SPI_Read(void) {
    // Read data from SPI bus
    while (!SSPSTATbits.BF);   // Wait for buffer to be full
    return SSPBUF;              // Read data from buffer
}
```

### PICSimLab Procedure
1. **Select I2C/SPI board**: Boards → PIC16F877A → LCD + Keypad + Potentiometer
2. **Connect devices**: Use built-in I2C sensor (e.g., temperature sensor)
3. **Build and debug**: Create project and load into PICSimLab
4. **Monitor communication**: Use PICSimLab's I2C/SPI monitor

### Expected Result
- I2C communication works with external sensors (e.g., temperature sensor)
- SPI communication works with external devices (e.g., memory, ADC)
- Data is correctly transmitted and received

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| I2C not responding | Wrong slave address | Check I2C slave address format (7-bit or 10-bit) |
| SPI not working | Wrong SPI mode | Set correct SPI mode (Mode 0: CPOL=0, CPHA=0) |
| Data corruption | Wrong clock polarity | Set correct clock polarity and phase |
| Hardware not connected | Wrong pin connections | Verify SDA/SCL and MOSI/MISO connections |

### 🔗 Next Step
[Step 8: Character LCD & Keypad Interface](#step-8-character-lcd--keypad-interface)

> **Register Focus**: SSPCON1, SSPADD for SPI/I2C timing; TRISC3-4 for I2C; TRISC5-7 for SPI.

---

## Step 8: Character LCD & Keypad Interface

### Goal
Interface with a character LCD (HD44780) and 4x4 keypad using GPIO.

### Prerequisites
- Completion of Steps 1-7
- Understanding of LCD timing and keypad scanning
- Experience with GPIO control

### Concept Explanation

#### HD44780 Character LCD (4-bit mode)
- 16x2 or 20x4 character display
- 5x8 dot character matrix
- Communicates via 4-bit data bus (D4-D7) + control lines (RS, E, R/W)
- Requires precise timing for command/data transmission

#### 4x4 Keypad Scanning
- 4 rows + 4 columns = 8 GPIO pins
- Scan rows sequentially to detect column presses
- Debounce keypad inputs
- Map key positions to characters

### Minimal XC8 Snippets
```c
// LCD Functions (4-bit mode)
void LCD_Init(void) {
    __delay_ms(20);            // Power-on delay
    LCD_Cmd(0x02);             // Return home
    LCD_Cmd(0x28);             // 4-bit mode, 2 lines, 5x8 font
    LCD_Cmd(0x0C);             // Display on, cursor off
    LCD_Cmd(0x06);             // Increment cursor
    LCD_Cmd(0x01);             // Clear display
    __delay_ms(2);
}

void LCD_Cmd(unsigned char cmd) {
    LCD_RS = 0;                // Command mode
    
    // High nibble
    LCD_D4 = (cmd & 0x10) >> 4;
    LCD_D5 = (cmd & 0x20) >> 5;
    LCD_D6 = (cmd & 0x40) >> 6;
    LCD_D7 = (cmd & 0x80) >> 7;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    
    // Low nibble
    LCD_D4 = (cmd & 0x01);
    LCD_D5 = (cmd & 0x02) >> 1;
    LCD_D6 = (cmd & 0x04) >> 2;
    LCD_D7 = (cmd & 0x08) >> 3;
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_ms(2);
}

void LCD_String(const char *str) {
    while(*str) {
        LCD_Data(*str++);
    }
}

// Keypad Scanning
char Read_Keypad(void) {
    char row, col;
    
    // Define row and column pins
    #define ROW1 PORTA0
    #define ROW2 PORTA1
    #define ROW3 PORTA2
    #define ROW4 PORTA3
    #define COL1 PORTB0
    #define COL2 PORTB1
    #define COL2 PORTB2
    #define COL3 PORTB3
    
    // Scan rows
    TRISA = 0b11110000;       // Rows as inputs
    TRISB = 0b11110000;       // Columns as inputs
    
    if (ROW1 == 0) { col = 0; }
    if (ROW2 == 0) { col = 1; }
    if (ROW3 == 0) { col = 2; }
    if (ROW4 == 0) { col = 3; }
    
    return col;
}
```

### PICSimLab Procedure
1. **Select LCD+Keypad board**: Boards → PIC16F877A → LCD + Keypad + Potentiometer
2. **Connect LCD**: Use built-in LCD on the board
3. **Connect keypad**: Use built-in keypad on the board
4. **Build and debug**: Create project and load into PICSimLab
5. **Test interface**: Enter characters via keypad and see on LCD

### Expected Result
- LCD displays characters entered via keypad
- Keypad scan detects button presses correctly
- Character display works on LCD

### Common Pitfalls
| Issue | Cause | Solution |
|-------|-------|----------|
| LCD not responding | Wrong initialization sequence | Use correct initialization sequence |
| Keypad not detecting presses | Wrong pin configuration | Set correct pins as inputs/outputs |
| Characters not showing | Timing issues | Ensure proper delays for LCD timing |
| Ghosting on keypad | Row/column scanning issues | Implement proper scan timing |

### 🔗 Next Step
[Advanced Steps](./advanced.md)

> **Register Focus**: TRISD, LATD for LCD control; PORTA, PORTB for keypad scanning; GPIO timing.

---

## Summary & Checklist

### Beginner Section Checklist
- [ ] Create first MPLAB X project for PIC16F877A
- [ ] Configure `#pragma config` bits correctly
- [ ] Implement LED blink with `__delay_ms()`
- [ ] Measure timing with oscilloscope (simulated/virtual)
- [ ] Understand TRIS vs PORT vs LAT registers
- [ ] Implement button debouncing in firmware
- [ ] Use Timer0 with overflow interrupt for precise timing
- [ ] Replace software delays with hardware timer ISR

### Intermediate Section Checklist
- [ ] Read analog voltage via ADC and display on LCD
- [ ] Generate PWM signals using CCP module
- [ ] Implement UART serial communication at 9600 baud
- [ ] Communicate with I2C temperature sensor (simulated)
- [ ] Interface 4-bit character LCD in simulation
- [ ] Scan 4x4 keypad matrix using GPIO
- [ ] Troubleshoot common peripheral configuration issues

### Advanced Section Checklist
- [ ] Implement sleep modes and watchdog timer
- [ ] Design interrupt-driven architecture with context saving
- [ ] Use analog comparators for zero-crossing detection
- [ ] Store calibration data in internal EEPROM
- [ ] Program real PIC16F877A via PICkit and ICSP
- [ ] Build capstone: PID temperature controller OR UART-to-Python bridge
- [ ] Document register-level understanding in project report

### Code Examples Included
- [`blink.c`](../code-examples/xc8/blink.c) - Basic LED blink
- [`button_led.c`](../code-examples/xc8/button_led.c) - Button-controlled LED with debounce
- [`timer_blink.c`](../code-examples/xc8/timer_blink.c) - Timer0 interrupt-driven blinking
- [`adc_voltage.c`](../code-examples/xc8/adc_voltage.c) - ADC with LCD display
- [`pwm_dimmer.c`](../code-examples/xc8/pwm_dimmer.c) - PWM LED dimmer
- [`uart_echo.c`](../code-examples/xc8/uart_echo.c) - UART echo functionality
- [`i2c_temp.c`](../code-examples/xc8/i2c_temp.c) - I2C communication with sensor
- [`lcd_hello.c`](../code-examples/xc8/lcd_hello.c) - LCD initialization and display

### Project References
- **[Project 1: GPIO Control Board](../projects/beginner.md#project-1-gpio-control-board-bare-metal-systick-fsm)**
- **[Project 2: UART Command Console](../projects/beginner.md#project-2-uart-command-console-ring-buffer--shell)**
- **[Project 3: Sensor Data Logger](../projects/intermediate.md#project-3-sensor-data-logger-i2cspiadctimers)**
- **[Project 4: PWM Motor Controller](../projects/intermediate.md#project-4-pwm-fanmotor-controller-pwmadcinterrupts)**
- **[Project 5: FreeRTOS Environmental Monitor](../projects/intermediate.md#project-5-freertos-environmental-monitor-rtosconcurrency)**
- **[Project 6: Connected IoT Node](../projects/advanced.md#project-6-connected-iot-node-networkingiotsecurityota)**
- **[Project 7: TinyML Edge Device](../projects/advanced.md#project-7-tinyml-edge-device-tinymloptimization)**

> **Remember**: The goal isn't to memorize register addresses—it's to understand how to *find* and *interpret* them in any datasheet. This skill transfers across all microcontroller architectures.
