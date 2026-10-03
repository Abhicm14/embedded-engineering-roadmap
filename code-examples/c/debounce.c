/**
 * @file debounce.c
 * @brief Non-blocking software switch debounce state machine in C99.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef enum {
    BTN_RELEASED,
    BTN_DEBOUNCE_PRESS,
    BTN_PRESSED,
    BTN_DEBOUNCE_RELEASE
} DebounceState_t;

typedef struct {
    DebounceState_t state;
    uint32_t        timer_ms;
    uint32_t        debounce_threshold_ms;
    bool            press_event_latched;
} ButtonDebouncer_t;

void ButtonDebouncer_Init(ButtonDebouncer_t *btn, uint32_t threshold_ms) {
    btn->state = BTN_RELEASED;
    btn->timer_ms = 0;
    btn->debounce_threshold_ms = threshold_ms;
    btn->press_event_latched = false;
}

void ButtonDebouncer_Update(ButtonDebouncer_t *btn, bool raw_pin_is_low, uint32_t current_time_ms) {
    switch (btn->state) {
        case BTN_RELEASED:
            if (raw_pin_is_low) {
                btn->timer_ms = current_time_ms;
                btn->state = BTN_DEBOUNCE_PRESS;
            }
            break;

        case BTN_DEBOUNCE_PRESS:
            if ((current_time_ms - btn->timer_ms) >= btn->debounce_threshold_ms) {
                if (raw_pin_is_low) {
                    btn->state = BTN_PRESSED;
                    btn->press_event_latched = true; /* Single pulse event */
                } else {
                    btn->state = BTN_RELEASED;
                }
            }
            break;

        case BTN_PRESSED:
            if (!raw_pin_is_low) {
                btn->timer_ms = current_time_ms;
                btn->state = BTN_DEBOUNCE_RELEASE;
            }
            break;

        case BTN_DEBOUNCE_RELEASE:
            if ((current_time_ms - btn->timer_ms) >= btn->debounce_threshold_ms) {
                if (!raw_pin_is_low) {
                    btn->state = BTN_RELEASED;
                } else {
                    btn->state = BTN_PRESSED;
                }
            }
            break;
    }
}

bool ButtonDebouncer_ConsumePress(ButtonDebouncer_t *btn) {
    bool event = btn->press_event_latched;
    btn->press_event_latched = false;
    return event;
}

int main(void) {
    printf("=== Running Button Debounce State Machine Tests ===\n");

    ButtonDebouncer_t btn;
    ButtonDebouncer_Init(&btn, 20); // 20ms debounce threshold

    /* Simulate contact bouncing: 0ms LOW, 5ms HIGH, 10ms LOW */
    ButtonDebouncer_Update(&btn, true, 0);
    assert(btn.state == BTN_DEBOUNCE_PRESS);

    ButtonDebouncer_Update(&btn, false, 5); // Bounced back HIGH!
    ButtonDebouncer_Update(&btn, false, 25); // Debounce expires while HIGH -> Glitch rejected
    assert(btn.state == BTN_RELEASED);
    assert(ButtonDebouncer_ConsumePress(&btn) == false);

    /* Valid press: stays LOW for > 20ms */
    ButtonDebouncer_Update(&btn, true, 30);
    ButtonDebouncer_Update(&btn, true, 55); // 25ms elapsed LOW
    assert(btn.state == BTN_PRESSED);
    assert(ButtonDebouncer_ConsumePress(&btn) == true);
    assert(ButtonDebouncer_ConsumePress(&btn) == false); // Latch cleared

    printf("Button Debounce Tests PASSED successfully!\n");
    return 0;
}
