#include "stm32f401xe.h"
#include <stdbool.h>

/* Global millisecond tick counter updated by SysTick ISR */
static volatile uint32_t s_ticks = 0;

/* Button Debounce Finite State Machine States */
typedef enum {
    BTN_STATE_RELEASED,
    BTN_STATE_DEBOUNCE_PRESS,
    BTN_STATE_PRESSED,
    BTN_STATE_DEBOUNCE_RELEASE
} ButtonState_t;

/* SysTick Interrupt Handler - Fires every 1 ms */
void SysTick_Handler(void) {
    s_ticks++;
}

/* Returns elapsed milliseconds since boot */
static uint32_t GetTick(void) {
    return s_ticks;
}

/* Simple non-blocking millisecond delay */
static void DelayMs(uint32_t ms) {
    uint32_t start = GetTick();
    while ((GetTick() - start) < ms) {
        __asm__ volatile ("nop");
    }
}

/* Hardware Initialization */
static void System_Init(void) {
    /* 1. Enable Clocks for GPIOA (Button) and GPIOC (LED) in RCC */
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN);

    /* 2. Configure PC13 as Output (Bits [27:26] = 01) */
    GPIOC->MODER &= ~(3UL << (13 * 2));
    GPIOC->MODER |=  (1UL << (13 * 2));

    /* Set PC13 Output Type to Push-Pull (Bit 13 = 0) */
    GPIOC->OTYPER &= ~(1UL << 13);

    /* Set PC13 Speed to Medium (Bits [27:26] = 01) */
    GPIOC->OSPEEDR &= ~(3UL << (13 * 2));
    GPIOC->OSPEEDR |=  (1UL << (13 * 2));

    /* Turn off LED initially (Active LOW: write 1 to high register BSRR) */
    GPIOC->BSRR = (1UL << 13);

    /* 3. Configure PA0 as Input with Pull-Up (Bits [1:0] = 00 in MODER, 01 in PUPDR) */
    GPIOA->MODER &= ~(3UL << (0 * 2));
    GPIOA->PUPDR &= ~(3UL << (0 * 2));
    GPIOA->PUPDR |=  (1UL << (0 * 2)); // Pull-up

    /* 4. Configure SysTick for 1ms tick using 16 MHz internal default HSI clock */
    /* 16,000,000 Hz / 1,000 Hz = 16,000 cycles per tick */
    SysTick->LOAD = (16000UL - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = (SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT | SysTick_CTRL_CLKSOURCE);
}

/* Toggle Onboard LED (PC13) */
static void LED_Toggle(void) {
    if (GPIOC->ODR & (1UL << 13)) {
        GPIOC->BSRR = (1UL << (13 + 16)); /* Reset PC13 (Turn ON) */
    } else {
        GPIOC->BSRR = (1UL << 13);        /* Set PC13 (Turn OFF) */
    }
}

/* Read raw button state (Active LOW: 0 = pressed, 1 = released) */
static bool Button_IsRawPressed(void) {
    return ((GPIOA->IDR & (1UL << 0)) == 0);
}

int main(void) {
    System_Init();

    ButtonState_t btn_state = BTN_STATE_RELEASED;
    uint32_t state_timer = 0;
    const uint32_t DEBOUNCE_TIME_MS = 25;

    while (1) {
        /* Run Button Finite State Machine */
        switch (btn_state) {
            case BTN_STATE_RELEASED:
                if (Button_IsRawPressed()) {
                    state_timer = GetTick();
                    btn_state = BTN_STATE_DEBOUNCE_PRESS;
                }
                break;

            case BTN_STATE_DEBOUNCE_PRESS:
                if ((GetTick() - state_timer) >= DEBOUNCE_TIME_MS) {
                    if (Button_IsRawPressed()) {
                        /* Verified valid button press: Toggle LED */
                        LED_Toggle();
                        btn_state = BTN_STATE_PRESSED;
                    } else {
                        btn_state = BTN_STATE_RELEASED;
                    }
                }
                break;

            case BTN_STATE_PRESSED:
                if (!Button_IsRawPressed()) {
                    state_timer = GetTick();
                    btn_state = BTN_STATE_DEBOUNCE_RELEASE;
                }
                break;

            case BTN_STATE_DEBOUNCE_RELEASE:
                if ((GetTick() - state_timer) >= DEBOUNCE_TIME_MS) {
                    if (!Button_IsRawPressed()) {
                        btn_state = BTN_STATE_RELEASED;
                    } else {
                        btn_state = BTN_STATE_PRESSED;
                    }
                }
                break;

            default:
                btn_state = BTN_STATE_RELEASED;
                break;
        }

        /* 1 ms pacing delay */
        DelayMs(1);
    }

    return 0;
}
