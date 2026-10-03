#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_CAPACITY (128U) /* Must be a power of 2 */

typedef struct {
    uint8_t buffer[RING_BUFFER_CAPACITY];
    volatile uint32_t head;
    volatile uint32_t tail;
} RingBuffer_t;

void RingBuffer_Init(RingBuffer_t *rb);
bool RingBuffer_Push(RingBuffer_t *rb, uint8_t byte);
bool RingBuffer_Pop(RingBuffer_t *rb, uint8_t *byte);
bool RingBuffer_IsEmpty(const RingBuffer_t *rb);
bool RingBuffer_IsFull(const RingBuffer_t *rb);
uint32_t RingBuffer_Available(const RingBuffer_t *rb);

#endif /* RING_BUFFER_H */
