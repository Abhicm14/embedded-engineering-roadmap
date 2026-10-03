/**
 * @file adc_voltage.c
 * @brief Read analog voltage from potentiometer and display on HD44780 LCD
 * 
 * Demonstrates:
 * - 10-bit Successive Approximation ADC configuration (ADCON0, ADCON1)
 * - Analog pin multiplexing and acquisition timing delay
 * - 4-bit HD44780 LCD character display driver
 * - Integer-to-string voltage scaling without heavy floating point
 * 
 * Hardware Connections:
 * - Potentiometer wiper to RA0/AN0 (Pin 2)
 * - LCD RS: RD2, EN: RD3, Data (D4-D7): RD4-RD7 (Standard PICSimLab Board 1 mapping)
 * - LCD VDD to 5V, VSS to GND, RW to GND
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

// LCD Pin Definitions on PORTD
#define LCD_RS PORTDbits.RD2
#define LCD_EN PORTDbits.RD3
#define LCD_D4 PORTDbits.RD4
#define LCD_D5 PORTDbits.RD5
#define LCD_D6 PORTDbits.RD6
#define LCD_D7 PORTDbits.RD7

void LCD_PulseEnable(void) {
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(50);
}

void LCD_SendNibble(unsigned char nibble) {
    LCD_D4 = (nibble >> 0) & 1;
    LCD_D5 = (nibble >> 1) & 1;
    LCD_D6 = (nibble >> 2) & 1;
    LCD_D7 = (nibble >> 3) & 1;
    LCD_PulseEnable();
}

void LCD_Command(unsigned char cmd) {
    LCD_RS = 0;                     // 0 = Command mode
    LCD_SendNibble(cmd >> 4);       // Send high nibble
    LCD_SendNibble(cmd & 0x0F);     // Send low nibble
    if (cmd == 0x01 || cmd == 0x02) {
        __delay_ms(2);              // Clear display and Home require ~1.64 ms
    }
}

void LCD_Char(char data) {
    LCD_RS = 1;                     // 1 = Data mode
    LCD_SendNibble(data >> 4);      // Send high nibble
    LCD_SendNibble(data & 0x0F);    // Send low nibble
}

void LCD_Print(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_Init(void) {
    TRISD = 0x00;                   // PORTD as outputs for LCD
    PORTD = 0x00;
    __delay_ms(20);                 // Power-up stabilization delay

    // HD44780 4-bit initialization sequence
    LCD_RS = 0;
    LCD_SendNibble(0x03);
    __delay_ms(5);
    LCD_SendNibble(0x03);
    __delay_us(150);
    LCD_SendNibble(0x03);
    LCD_SendNibble(0x02);           // Switch to 4-bit mode

    LCD_Command(0x28);              // 4-bit mode, 2 lines, 5x8 font
    LCD_Command(0x0C);              // Display ON, cursor OFF
    LCD_Command(0x06);              // Entry mode: auto-increment
    LCD_Command(0x01);              // Clear display
}

void ADC_Init(void) {
    // 1. Configure RA0 as analog input
    TRISAbits.TRISA0 = 1;

    // 2. Configure ADCON1:
    // ADFM  = 1 (Right justified: 10-bit result in ADRESH:ADRESL)
    // ADCS2 = 0
    // PCFG3:PCFG0 = 1110 (AN0 analog, AN1-AN7 digital, Vref+=VDD, Vref-=VSS)
    ADCON1 = 0b10001110;

    // 3. Configure ADCON0:
    // ADCS1:ADCS0 = 01 (Fosc/8 conversion clock: 2 us Tad at 4 MHz)
    // CHS2:CHS0   = 000 (Channel 0 / AN0 selected)
    // ADON        = 1 (Turn on ADC module)
    ADCON0 = 0b01000001;
}

unsigned int ADC_Read(void) {
    __delay_us(20);                 // Wait for acquisition capacitor to charge (Tacq >= 19.7 us)
    ADCON0bits.GO_nDONE = 1;        // Start conversion
    while (ADCON0bits.GO_nDONE);    // Wait for conversion completion (hardware clears bit)
    return ((unsigned int)ADRESH << 8) | ADRESL; // 10-bit result (0 to 1023)
}

void main(void) {
    LCD_Init();
    ADC_Init();

    LCD_Command(0x80);              // Line 1
    LCD_Print("PIC16F877A ADC");

    while (1) {
        unsigned int raw_adc = ADC_Read();
        
        // Convert 10-bit ADC (0-1023) to millivolts (0-5000 mV with 5.0V VDD)
        // mV = (raw_adc * 5000) / 1023
        unsigned long mv = ((unsigned long)raw_adc * 5000UL) / 1023UL;
        unsigned int volts = (unsigned int)(mv / 1000);
        unsigned int decimals = (unsigned int)(mv % 1000);

        // Display on Line 2
        LCD_Command(0xC0);
        LCD_Print("Volt: ");
        LCD_Char((char)('0' + volts));
        LCD_Char('.');
        LCD_Char((char)('0' + (decimals / 100)));
        LCD_Char((char)('0' + ((decimals / 10) % 10)));
        LCD_Char((char)('0' + (decimals % 10)));
        LCD_Print(" V   ");

        __delay_ms(250);            // 4 Hz display refresh rate
    }
}
