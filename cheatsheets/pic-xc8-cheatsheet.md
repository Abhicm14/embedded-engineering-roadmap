# ⚡ PIC XC8 Cheat Sheet

> Quick reference for PIC16F877A register-level development using MPLAB X IDE and the Microchip XC8 compiler.

---

## 📦 Configuration Bits (PIC16F877A)

```c
#include <xc.h>

// Crystal oscillator configuration (4 MHz - 20 MHz)
#pragma config FOSC = HS        // High-Speed Crystal/Resonator
#pragma config WDTE = OFF       // Watchdog Timer disabled (ON for production)
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low-Voltage ICSP disabled (releases RB3)
#pragma config CPD = OFF        // Data EEPROM Code Protection off
#pragma config CP = OFF         // Flash Program Code Protection off

#define _XTAL_FREQ 4000000      // Required for __delay_ms() & __delay_us()
```

---

## 🖥️ Key Registers Reference

| Register | Description | Access Syntax & Example | Notes |
|:---------|:------------|:------------------------|:------|
| **TRISx** | Data Direction Register | `TRISBbits.TRISB0 = 0;` | `0` = Output, `1` = Input (Power-on default is all `1`) |
| **PORTx** | Pin State Read/Write | `PORTBbits.RB0 = 1;` | Mid-range PIC16F writes directly to PORTx pins |
| **LATx** *(PIC18F)* | Output Latch Register | `LATBbits.LATB0 = 1;` | Available on PIC18F & PIC16F1xxx to prevent RMW issues |
| **OPTION_REG** | Timer0 & Pull-up Control | `OPTION_REGbits.nRBPU = 0;` | `nRBPU=0` enables PORTB pull-ups; `T0CS`, `PSA`, `PS2:PS0` |
| **INTCON** | Interrupt Controller | `INTCONbits.GIE = 1;` | Global (`GIE`), Peripheral (`PEIE`), Timer0 (`T0IE`, `T0IF`) |
| **PIE1 / PIR1** | Peripheral Interrupts | `PIE1bits.RCIE = 1;` | USART, ADC, MSSP, Timer1/2 enables (`PIE1`) and flags (`PIR1`) |
| **ADCON0** | ADC Control 0 | `ADCON0bits.ADON = 1;` | Channel select `CHS2:0`, start bit `GO_nDONE`, ADC enable `ADON` |
| **ADCON1** | ADC Port Configuration | `ADCON1 = 0b10001110;` | Justification `ADFM` (1=Right), Port pin configuration `PCFG3:0` |
| **PR2 / T2CON** | Timer2 & PWM Period | `PR2 = 249; T2CON = 0x05;`| Timebase for CCP1/CCP2 hardware PWM |
| **CCPR1L / CCP1CON** | PWM Duty & Mode | `CCP1CON = 0x0C;` | `0x0C` sets PWM mode; `CCPR1L` sets upper 8 bits of duty cycle |
| **TXSTA / RCSTA** | USART Control | `TXSTA = 0x24; RCSTA = 0x90;` | Baud high-speed `BRGH`, transmit enable `TXEN`, receive `CREN` |
| **SSPCON / SSPADD** | MSSP I2C & SPI Control | `SSPCON = 0x28; SSPADD = 9;` | I2C Master mode enable (`SSPEN=1`), 100 kHz clock divider |

---

## ⏱️ Timing & Delay Mechanisms

### Software Delays (Instruction-Cycle Loop)
```c
__delay_ms(100);  // Blocks CPU for 100 milliseconds (uses _XTAL_FREQ)
__delay_us(25);   // Blocks CPU for 25 microseconds
```

### Hardware Timer0 Delay (Non-Blocking Overflow)
```c
// At 4 MHz crystal: Fosc/4 = 1 MHz (1 us instruction cycle)
// OPTION_REG prescaler 1:256 -> 1 count = 256 us
OPTION_REGbits.T0CS = 0;    // Internal instruction cycle clock
OPTION_REGbits.PSA = 0;     // Prescaler to Timer0
OPTION_REGbits.PS2 = 1;
OPTION_REGbits.PS1 = 1;
OPTION_REGbits.PS0 = 1;     // 1:256

TMR0 = 0;                   // Overflow occurs every 256 * 256 us = 65.536 ms
```

---

## 📊 10-Bit ADC Quick Start

```c
void ADC_Init(void) {
    TRISAbits.TRISA0 = 1;        // RA0/AN0 as input
    ADCON1 = 0b10001110;        // Right-justified, AN0 analog, VDD/VSS ref
    ADCON0 = 0b01000001;        // Fosc/8 clock, Channel 0, ADC ON
}

unsigned int ADC_Read(void) {
    __delay_us(20);             // Acquisition capacitor charging time (Tacq >= 19.7 us)
    ADCON0bits.GO_nDONE = 1;    // Start conversion
    while (ADCON0bits.GO_nDONE);// Wait for completion
    return ((unsigned int)ADRESH << 8) | ADRESL; // 10-bit integer (0 - 1023)
}
```

---

## 🔁 Hardware PWM (CCP1 + Timer2)

```c
void PWM_Init_1kHz(void) {
    TRISCbits.TRISC2 = 0;       // RC2/CCP1 pin as output
    PR2 = 249;                  // Period: (249 + 1) * 4 * (1/4MHz) * 4 = 1000 us (1 kHz)
    CCP1CON = 0b00001100;       // PWM mode
    CCPR1L = 0;                 // 0% initial duty cycle
    T2CON = 0b00000101;         // Timer2 ON, Prescaler 1:4
}

void PWM_SetDuty(unsigned int duty_10bit) {
    CCPR1L = (unsigned char)(duty_10bit >> 2);
    CCP1CONbits.CCP1X = (duty_10bit >> 1) & 1;
    CCP1CONbits.CCP1Y = duty_10bit & 1;
}
```

---

## 📡 USART Serial Communication (9600 Baud @ 4 MHz)

```c
void UART_Init(void) {
    TRISCbits.TRISC6 = 0;       // RC6 = TX Output
    TRISCbits.TRISC7 = 1;       // RC7 = RX Input
    SPBRG = 25;                 // 9600 baud (BRGH=1 high speed, 0.16% error)
    TXSTA = 0b00100100;         // TXEN=1, BRGH=1
    RCSTA = 0b10010000;         // SPEN=1, CREN=1
}

void UART_Write(char c) {
    while (!PIR1bits.TXIF);     // Wait until TXREG is ready
    TXREG = c;
}

char UART_Read(void) {
    if (RCSTAbits.OERR) {       // Overrun error recovery
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    while (!PIR1bits.RCIF);     // Wait until RCREG has data
    return RCREG;
}
```

---

## ⚡ Interrupt Handling Template

```c
void __interrupt() isr(void) {
    // Timer0 Overflow
    if (INTCONbits.T0IF && INTCONbits.T0IE) {
        INTCONbits.T0IF = 0;    // Must clear in software!
        // Handle timer event...
    }
    
    // UART Receive Byte
    if (PIR1bits.RCIF && PIE1bits.RCIE) {
        char rx = RCREG;        // Reading RCREG clears RCIF
        // Handle serial byte...
    }
}
```

---

## 💡 Top Engineering Pitfalls & Tips

1. **Read-Modify-Write (RMW) on PORTx**:
   - On mid-range PIC16F devices (which lack LAT registers), executing consecutive bit instructions (`PORTBbits.RB0 = 1; PORTBbits.RB1 = 1;`) rapidly on pins driving capacitive loads can cause the first pin to read back as `0` during the second instruction.
   - **Solution**: Maintain a software shadow variable (e.g. `unsigned char shadow_portb;`), update bits in the shadow variable, and write the full byte `PORTB = shadow_portb;`.
2. **Always Clear Interrupt Flags in Software**:
   - Hardware does not automatically clear peripheral flags like `T0IF` or `INTF`. Failing to clear the flag causes the CPU to re-enter the ISR perpetually upon return.
3. **MCLR Programming Pin Decoupling**:
   - Never place a bypass capacitor directly on the MCLR pin during ICSP programming. The PICkit requires sub-microsecond edge transitions to enter programming mode.
