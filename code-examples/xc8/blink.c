/**
 * @file blink.c
 * @brief LED blinking example for PIC16F877A using MPLAB X and XC8
 * 
 * Demonstrates:
 * - Microchip configuration bits setup for external crystal
 * - Pure register-level GPIO direction (TRISB) and output (PORTB)
 * - Software delay function __delay_ms()
 * 
 * Hardware Connections:
 * - LED anode connected to RB0 (Pin 33) through a 330 ohm current-limiting resistor
 * - LED cathode connected to GND (VSS, Pin 12 or 31)
 * - 4 MHz crystal on OSC1/OSC2 (Pins 13 & 14) with dual 22 pF capacitors to GND
 */

#include <xc.h>

// Configuration bits for PIC16F877A with external crystal oscillator
#pragma config FOSC = HS        // High-Speed Crystal/Resonator (4MHz - 20MHz)
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low-Voltage ICSP disabled (high-voltage programming)
#pragma config CPD = OFF        // Data EEPROM code protection disabled
#pragma config CP = OFF         // Program Flash memory code protection disabled

#define _XTAL_FREQ 4000000      // 4 MHz crystal frequency for compiler delay macros

void main(void) {
    // TRISB register: 0 = Output, 1 = Input
    TRISBbits.TRISB0 = 0;       // Configure RB0 as digital output
    PORTBbits.RB0 = 0;          // Initialize pin state to LOW (LED OFF)

    while (1) {
        PORTBbits.RB0 = 1;      // Pin HIGH: LED ON
        __delay_ms(500);        // 500 ms software delay
        
        PORTBbits.RB0 = 0;      // Pin LOW: LED OFF
        __delay_ms(500);        // 500 ms software delay
    }
}
