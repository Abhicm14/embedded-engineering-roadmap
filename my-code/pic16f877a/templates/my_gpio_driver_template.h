/**
 * @file my_gpio_driver_template.h
 * @brief Starter template for your custom PIC16F877A GPIO driver.
 *
 * Refer to PIC16F877A Datasheet Section 3.0 "I/O Ports".
 */

#ifndef MY_GPIO_DRIVER_TEMPLATE_H
#define MY_GPIO_DRIVER_TEMPLATE_H

#include <xc.h>
#include <stdint.h>

typedef enum {
    PIN_DIRECTION_OUTPUT = 0,
    PIN_DIRECTION_INPUT  = 1
} PinDirection_t;

typedef enum {
    PIN_STATE_LOW  = 0,
    PIN_STATE_HIGH = 1
} PinState_t;

/**
 * @brief Initialize a GPIO pin direction.
 * @param pin Pin index (0 to 7)
 * @param dir PIN_DIRECTION_OUTPUT or PIN_DIRECTION_INPUT
 */
void My_GPIO_SetDirection(uint8_t pin, PinDirection_t dir);

/**
 * @brief Write state to an output pin.
 * @param pin Pin index (0 to 7)
 * @param state PIN_STATE_HIGH or PIN_STATE_LOW
 */
void My_GPIO_Write(uint8_t pin, PinState_t state);

/**
 * @brief Read state from an input pin.
 * @param pin Pin index (0 to 7)
 * @return PinState_t
 */
PinState_t My_GPIO_Read(uint8_t pin);

/**
 * @brief Toggle the state of an output pin.
 * @param pin Pin index (0 to 7)
 */
void My_GPIO_Toggle(uint8_t pin);

#endif /* MY_GPIO_DRIVER_TEMPLATE_H */
