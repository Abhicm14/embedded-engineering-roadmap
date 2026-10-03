/**
 * @file blink.c
 * @brief Pure register-level STM32F401/F411 LED blinker (PC13 onboard LED).
 * @note No HAL, no CMSIS dependencies. Directly dereferences memory-mapped registers.
 */

#include <stdint.h>

#define RCC_BASE        (0x40023800UL)
#define RCC_AHB1ENR     (*((volatile uint32_t *)(RCC_BASE + 0x30UL)))
#define RCC_GPIOC_EN    (1UL << 2)

#define GPIOC_BASE      (0x40020800UL)
#define GPIOC_MODER     (*((volatile uint32_t *)(GPIOC_BASE + 0x00UL)))
#define GPIOC_BSRR      (*((volatile uint32_t *)(GPIOC_BASE + 0x18UL)))

#define LED_PIN         13

static void SimpleDelay(volatile uint32_t count) {
    while (count--) {
        __asm__ volatile ("nop");
    }
}

int main(void) {
    /* 1. Enable Clock gating for GPIOC in RCC */
    RCC_AHB1ENR |= RCC_GPIOC_EN;

    /* 2. Configure PC13 as Output: Bits [27:26] = 01 */
    GPIOC_MODER &= ~(3UL << (LED_PIN * 2));
    GPIOC_MODER |=  (1UL << (LED_PIN * 2));

    while (1) {
        /* Set PC13 LOW (Turn ON active-low LED on Black Pill) */
        GPIOC_BSRR = (1UL << (LED_PIN + 16));
        SimpleDelay(500000);

        /* Set PC13 HIGH (Turn OFF active-low LED) */
        GPIOC_BSRR = (1UL << LED_PIN);
        SimpleDelay(500000);
    }

    return 0;
}
