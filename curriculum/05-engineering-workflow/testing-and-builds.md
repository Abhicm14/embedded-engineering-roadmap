# 🧪 Embedded Testing, TDD & Automated CI Pipelines

> Unit testing C firmware off-target with Unity/CMock and automated verification in GitHub Actions.

---

## 1. Why Test Firmware Off-Target?

Testing firmware exclusively by flashing physical hardware and watching an LED or oscilloscope is too slow and does not scale:
- You cannot easily test edge cases (e.g. sensor returning corrupt CRC or battery voltage dropping to 2.9V).
- **Off-Target Unit Testing:** By mocking silicon register writes, your C algorithms (circular buffers, state machines, packet parsers) can be compiled with standard `gcc` on a PC and executed in milliseconds!

---

## 2. Unit Testing with the Unity Framework

```c
#include "unity.h"
#include "circular_buffer.h"

static CircularBuffer_t s_cb;

void setUp(void) {
    CircularBuffer_Init(&s_cb);
}

void tearDown(void) {}

void test_Buffer_PushPopSingleByte(void) {
    TEST_ASSERT_TRUE(CircularBuffer_Push(&s_cb, 0x42));
    uint8_t out = 0;
    TEST_ASSERT_TRUE(CircularBuffer_Pop(&s_cb, &out));
    TEST_ASSERT_EQUAL_HEX8(0x42, out);
}

void test_Buffer_OverflowProtection(void) {
    // Fill to capacity
    for (int i = 0; i < 15; i++) {
        CircularBuffer_Push(&s_cb, (uint8_t)i);
    }
    // 16th push must fail safely
    TEST_ASSERT_FALSE(CircularBuffer_Push(&s_cb, 0xFF));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Buffer_PushPopSingleByte);
    RUN_TEST(test_Buffer_OverflowProtection);
    return UNITY_END();
}
```

---

## 3. GitHub Actions CI for Embedded Firmware

Every pull request should trigger automated compilation with `-Wall -Wextra -Werror` to prevent syntax bugs from entering the main codebase.
