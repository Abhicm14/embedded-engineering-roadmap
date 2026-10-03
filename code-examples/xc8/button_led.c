/**
 * @file button_led.c
 * @brief Button-controlled LED with software debounce for PIC16F877A
 * 
 * Demonstrates:
 * - Digital input configuration on PORTB with internal weak pull-ups (OPTION_REG.nRBPU)
 * - Software state debouncing without blocking delays
 * - Digital output driving an active-high LED
 * 
 * Hardware Connections:
 * - LED on RB0 (Pin 33) with 330 ohm resistor to GND
 * - Pushbutton between RB1 (Pin 34) and GND (using internal pull-up)
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

#define DEBOUNCE_THRESHOLD_MS 20

void main(void) {
    // 1. Configure Port B direction: RB0 as output, RB1 as input
    TRISBbits.TRISB0 = 0;       // RB0 Output (LED)
    TRISBbits.TRISB1 = 1;       // RB1 Input  (Button)
    PORTBbits.RB0 = 0;          // Start with LED off

    // 2. Enable internal weak pull-ups on PORTB (OPTION_REG.nRBPU is active low)
    OPTION_REGbits.nRBPU = 0;   // 0 = PORTB pull-ups enabled

    unsigned char debounced_state = 1;  // Button released (pulled HIGH)
    unsigned char last_sample = 1;
    unsigned int  stable_count = 0;

    while (1) {
        unsigned char current_sample = PORTBbits.RB1; // Read physical pin

        if (current_sample == last_sample) {
            if (stable_count < DEBOUNCE_THRESHOLD_MS) {
                stable_count++;
                if (stable_count == DEBOUNCE_THRESHOLD_MS) {
                    // State has stabilized
                    debounced_state = current_sample;
                    
                    // Active low button pressed -> turn on LED
                    if (debounced_state == 0) {
                        PORTBbits.RB0 = 1;  // LED ON
                    } else {
                        PORTBbits.RB0 = 0;  // LED OFF
                    }
                }
            }
        } else {
            // Signal toggled, reset stabilization counter
            stable_count = 0;
            last_sample = current_sample;
        }

        __delay_ms(1); // 1 ms debounce sampling tick
    }
}
