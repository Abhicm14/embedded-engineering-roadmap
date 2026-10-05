/**
 * @file blink.ino
 * @brief AVR ATmega328P Hardware Timer1 CTC Interrupt Blinky (Bypassing Arduino delay())
 *
 * Demonstrates:
 * 1. Timer1 configuration in CTC (Clear Timer on Compare Match) mode.
 * 2. 1024 prescaler calculation for exact 1 Hz / 0.5s toggle rates.
 * 3. Atomic hardware output toggle inside an Interrupt Service Routine (ISR).
 * 4. Leaving loop() non-blocking for background tasks.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// On ATmega328P (Arduino Uno), PB5 maps to Digital Pin 13
#define LED_PIN_BIT   PORTB5

// CPU Frequency = 16 MHz, Prescaler = 1024
// Target interrupt rate = 2 Hz (toggling every 0.5s gives 1 Hz complete blink cycle)
// OCR1A = (F_CPU / (Prescaler * Target_Freq)) - 1
// OCR1A = (16,000,000 / (1024 * 2)) - 1 = 7812 - 1 = 7811
#define TIMER1_COMPARE_VAL  7811

/**
 * @brief Timer1 Compare Match A Interrupt Service Routine
 */
ISR(TIMER1_COMPA_vect) {
    // 1-cycle atomic hardware pin toggle using AVR PIN register write
    PINB = (1 << PINB5);
}

void setup() {
    // Step 1: Configure PB5 (Digital Pin 13) as Output
    DDRB |= (1 << DDB5);

    // Step 2: Ensure LED starts in OFF state
    PORTB &= ~(1 << PORTB5);

    // Step 3: Disable global interrupts while configuring Timer1
    cli();

    // Step 4: Reset Timer1 Control Registers
    TCCR1A = 0; // Normal port operation, OC1A/OC1B disconnected
    TCCR1B = 0;
    TCNT1  = 0; // Clear timer counter value

    // Step 5: Set Compare Match Value for 2 Hz toggle (1 Hz full blink)
    OCR1A = TIMER1_COMPARE_VAL;

    // Step 6: Configure CTC Mode (Clear Timer on Compare Match)
    // CTC Mode 4: WGM13=0, WGM12=1, WGM11=0, WGM10=0 (WGM12 is in TCCR1B)
    TCCR1B |= (1 << WGM12);

    // Step 7: Set Prescaler to 1024 (CS12=1, CS11=0, CS10=1)
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Step 8: Enable Timer1 Output Compare A Match Interrupt
    TIMSK1 |= (1 << OCIE1A);

    // Step 9: Re-enable global interrupts
    sei();

    Serial.begin(115200);
    Serial.println("Timer1 CTC Interrupt Blinky Initialized (Non-Blocking)");
}

void loop() {
    // The superloop is completely free! No blocking delay() calls.
    // Background sensor reading, telemetry, or state machines run uninterrupted.
}
