/**
 * @file pwm_dimmer.c
 * @brief LED brightness fading using CCP1 hardware PWM module on PIC16F877A
 * 
 * Demonstrates:
 * - Capture/Compare/PWM (CCP1) peripheral in PWM mode
 * - Timer2 configuration as the PWM timebase (PR2, T2CON)
 * - 10-bit duty cycle modulation (CCPR1L and CCP1CONbits)
 * 
 * Hardware Connections:
 * - LED anode connected to RC2/CCP1 (Pin 17) via 330 ohm resistor to GND
 * - At 4 MHz Fosc, generates a flicker-free 1 kHz PWM signal
 */

#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

void PWM1_Init_1kHz(void) {
    // 1. Configure CCP1 pin (RC2) as output
    TRISCbits.TRISC2 = 0;

    // 2. Set PWM Period for 1.0 kHz frequency:
    // Period = (PR2 + 1) * 4 * Tosc * (TMR2 Prescale Value)
    // At 4 MHz, Tosc = 0.25 us. With Prescale = 4:
    // 1000 us = (PR2 + 1) * 4 * 0.25 us * 4 = (PR2 + 1) * 4 us -> PR2 = 249
    PR2 = 249;

    // 3. Configure CCP1CON for PWM mode:
    // Bits 3:0 = 1100 (PWM mode)
    CCP1CON = 0b00001100;

    // 4. Clear initial duty cycle
    CCPR1L = 0;

    // 5. Configure and start Timer2:
    // T2CKPS1:T2CKPS0 = 01 (Prescaler is 4)
    // TMR2ON = 1 (Turn on Timer2)
    T2CON = 0b00000101;
}

void PWM1_Set_Duty(unsigned int duty_10bit) {
    // Clamp duty cycle to maximum 10-bit count ((PR2 + 1) * 4 = 1000)
    if (duty_10bit > 1000) {
        duty_10bit = 1000;
    }

    // Upper 8 bits go into CCPR1L
    CCPR1L = (unsigned char)(duty_10bit >> 2);

    // Lower 2 bits go into CCP1CON<5:4>
    CCP1CONbits.CCP1X = (duty_10bit >> 1) & 1;
    CCP1CONbits.CCP1Y = duty_10bit & 1;
}

void main(void) {
    PWM1_Init_1kHz();

    while (1) {
        // Ramp brightness UP from 0% to 100%
        for (unsigned int duty = 0; duty <= 1000; duty += 10) {
            PWM1_Set_Duty(duty);
            __delay_ms(15);
        }

        __delay_ms(100);

        // Ramp brightness DOWN from 100% to 0%
        for (int duty = 1000; duty >= 0; duty -= 10) {
            PWM1_Set_Duty((unsigned int)duty);
            __delay_ms(15);
        }

        __delay_ms(200);
    }
}
