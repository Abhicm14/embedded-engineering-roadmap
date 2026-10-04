/**
 * @file my_gpio_driver_template.c
 * @brief Implementation template for your custom PIC16F877A GPIO driver.
 *
 * Refer to:
 * - Datasheet Section 3.2: PORTB and TRISB Register
 * - Datasheet Section 2.2.2.2: OPTION_REG (for internal pull-ups)
 */

#include "my_gpio_driver_template.h"

void My_GPIO_SetDirection(uint8_t pin, PinDirection_t dir) {
    // TODO: Write code to set TRISB register bit for 'pin'
    // Hint: If dir == PIN_DIRECTION_INPUT, set bit in TRISB: TRISB |= (1 << pin);
    // Hint: If dir == PIN_DIRECTION_OUTPUT, clear bit in TRISB: TRISB &= ~(1 << pin);
}

void My_GPIO_Write(uint8_t pin, PinState_t state) {
    // TODO: Write code to update PORTB register bit for 'pin'
    // Hint: If state == PIN_STATE_HIGH, set bit in PORTB: PORTB |= (1 << pin);
    // Hint: If state == PIN_STATE_LOW, clear bit in PORTB: PORTB &= ~(1 << pin);
}

PinState_t My_GPIO_Read(uint8_t pin) {
    // TODO: Write code to read PORTB bit for 'pin'
    // Hint: Check if (PORTB & (1 << pin)) is non-zero
    return PIN_STATE_LOW;
}

void My_GPIO_Toggle(uint8_t pin) {
    // TODO: Write code to toggle PORTB bit for 'pin'
    // Hint: Use bitwise XOR (^=) on PORTB
}
