#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer_t *rb) {
    if (rb == 0) return;
    rb->head = 0;
    rb->tail = 0;
}

bool RingBuffer_Push(RingBuffer_t *rb, uint8_t byte) {
    if (rb == 0) return false;
    uint32_t next_head = (rb->head + 1U) & (RING_BUFFER_CAPACITY - 1U);
    if (next_head == rb->tail) {
        return false; /* Buffer overflow */
    }
    rb->buffer[rb->head] = byte;
    rb->head = next_head;
    return true;
}

bool RingBuffer_Pop(RingBuffer_t *rb, uint8_t *byte) {
    if (rb == 0 || byte == 0) return false;
    if (rb->head == rb->tail) {
        return false; /* Buffer empty */
    }
    *byte = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1U) & (RING_BUFFER_CAPACITY - 1U);
    return true;
}

bool RingBuffer_IsEmpty(const RingBuffer_t *rb) {
    return (rb != 0) && (rb->head == rb->tail);
}

bool RingBuffer_IsFull(const RingBuffer_t *rb) {
    if (rb == 0) return false;
    return (((rb->head + 1U) & (RING_BUFFER_CAPACITY - 1U)) == rb->tail);
}

uint32_t RingBuffer_Available(const RingBuffer_t *rb) {
    if (rb == 0) return 0;
    return (rb->head - rb->tail) & (RING_BUFFER_CAPACITY - 1U);
}
