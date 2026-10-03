#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define FIFO_SIZE 16 /* Power of 2 */

typedef struct {
    uint8_t  buffer[FIFO_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} LockFreeFIFO_t;

static void FIFO_Init(LockFreeFIFO_t *f) {
    f->head = 0;
    f->tail = 0;
}

static bool FIFO_Push(LockFreeFIFO_t *f, uint8_t byte) {
    uint32_t next = (f->head + 1U) & (FIFO_SIZE - 1U);
    if (next == f->tail) {
        return false; /* Overflow */
    }
    f->buffer[f->head] = byte;
    f->head = next;
    return true;
}

static bool FIFO_Pop(LockFreeFIFO_t *f, uint8_t *byte) {
    if (f->head == f->tail) {
        return false; /* Underflow */
    }
    *byte = f->buffer[f->tail];
    f->tail = (f->tail + 1U) & (FIFO_SIZE - 1U);
    return true;
}

int main(void) {
    printf("=== Testing Lock-Free UART FIFO Buffer ===\n");

    LockFreeFIFO_t fifo;
    FIFO_Init(&fifo);

    /* Push 15 bytes (capacity - 1) */
    for (uint8_t i = 1; i <= 15; i++) {
        bool ok = FIFO_Push(&fifo, i);
        assert(ok == true);
    }

    /* 16th push must fail (full) */
    assert(FIFO_Push(&fifo, 99) == false);

    /* Pop all bytes and verify order */
    for (uint8_t i = 1; i <= 15; i++) {
        uint8_t val = 0;
        bool ok = FIFO_Pop(&fifo, &val);
        assert(ok == true);
        assert(val == i);
    }

    /* Empty pop must fail */
    uint8_t dummy = 0;
    assert(FIFO_Pop(&fifo, &dummy) == false);

    printf("All FIFO buffer assertions PASSED!\n");
    return 0;
}
