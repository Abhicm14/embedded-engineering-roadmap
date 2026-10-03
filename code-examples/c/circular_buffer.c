/**
 * @file circular_buffer.c
 * @brief Lock-free, power-of-two FIFO Circular Ring Buffer implementation in C99.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#define CB_CAPACITY (16U) /* Must be a power of 2 */

typedef struct {
    uint8_t buffer[CB_CAPACITY];
    volatile uint32_t head; /* Written by Producer */
    volatile uint32_t tail; /* Read by Consumer */
} CircularBuffer_t;

void CircularBuffer_Init(CircularBuffer_t *cb) {
    if (!cb) return;
    cb->head = 0;
    cb->tail = 0;
}

bool CircularBuffer_Push(CircularBuffer_t *cb, uint8_t byte) {
    if (!cb) return false;
    uint32_t next_head = (cb->head + 1U) & (CB_CAPACITY - 1U);
    if (next_head == cb->tail) {
        return false; /* Buffer is full */
    }
    cb->buffer[cb->head] = byte;
    cb->head = next_head;
    return true;
}

bool CircularBuffer_Pop(CircularBuffer_t *cb, uint8_t *byte) {
    if (!cb || !byte) return false;
    if (cb->head == cb->tail) {
        return false; /* Buffer is empty */
    }
    *byte = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1U) & (CB_CAPACITY - 1U);
    return true;
}

bool CircularBuffer_IsEmpty(const CircularBuffer_t *cb) {
    return cb && (cb->head == cb->tail);
}

uint32_t CircularBuffer_Count(const CircularBuffer_t *cb) {
    if (!cb) return 0;
    return (cb->head - cb->tail) & (CB_CAPACITY - 1U);
}

int main(void) {
    printf("=== Running Circular Buffer Unit Tests ===\n");

    CircularBuffer_t cb;
    CircularBuffer_Init(&cb);
    assert(CircularBuffer_IsEmpty(&cb) == true);

    /* Push up to capacity - 1 (15 items) */
    for (uint8_t i = 1; i <= 15; i++) {
        assert(CircularBuffer_Push(&cb, i) == true);
    }

    /* 16th item push must fail */
    assert(CircularBuffer_Push(&cb, 99) == false);
    assert(CircularBuffer_Count(&cb) == 15);

    /* Pop all items and verify order */
    for (uint8_t i = 1; i <= 15; i++) {
        uint8_t val = 0;
        assert(CircularBuffer_Pop(&cb, &val) == true);
        assert(val == i);
    }

    assert(CircularBuffer_IsEmpty(&cb) == true);
    printf("Circular Buffer Tests PASSED successfully!\n");
    return 0;
}
