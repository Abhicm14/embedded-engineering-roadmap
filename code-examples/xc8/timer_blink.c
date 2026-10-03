/**
 * @file timer_blink.c
 * @brief LED blinking using Timer0 hardware interrupt on PIC16F877A
 * 
 * Demonstrates:
 * - 8-bit Timer0 initialization via OPTION_REG
 * - Prescaler assignment to Timer0
 * - Global (GIE) and Peripheral/Timer0 (T0IE) interrupt configuration
 * - Non-blocking ISR-driven timing architecture
 * 
 * Hardware Connections:
 * - LED connected to RB0 (Pin 33) with current-limiting resistor to GND
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

volatile unsigned int overflow_counter = 0;

void __interrupt() isr(void) {
    // Check if Timer0 overflow triggered the interrupt
    if (INTCONbits.T0IF && INTCONbits.T0IE) {
        INTCONbits.T0IF = 0;        // Clear Timer0 interrupt flag in software!

        overflow_counter++;
        
        // At 4 MHz, Fosc/4 = 1 MHz (1 us instruction cycle)
        // Timer0 prescaler 1:256 -> 1 tick = 256 us
        // 256 counts to overflow -> 256 * 256 us = 65.536 ms per overflow
        // 8 overflows * 65.536 ms ~= 524 ms (~0.5 second)
        if (overflow_counter >= 8) {
            overflow_counter = 0;
            PORTBbits.RB0 ^= 1;     // Toggle LED pin state
        }
    }
}

void main(void) {
    // 1. Configure RB0 as output
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    // 2. Configure Timer0 in OPTION_REG
    // T0CS = 0 (Internal instruction cycle clock, Fosc/4)
    // PSA  = 0 (Prescaler assigned to Timer0 module)
    // PS2:PS0 = 111 (1:256 prescaler rate)
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;

    TMR0 = 0;                       // Reset initial timer counter

    // 3. Enable interrupts in INTCON register
    INTCONbits.T0IF = 0;            // Clear any pending flag
    INTCONbits.T0IE = 1;            // Enable Timer0 overflow interrupt
    INTCONbits.GIE = 1;             // Enable Global Interrupts

    while (1) {
        // CPU is free to execute other foreground tasks in main superloop
        // LED blinking is handled deterministically by the hardware Timer0 ISR
    }
}
